#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include "patch.h"
#include "patch_apply.h"
#include "bzip2\bzlib.h"
#include <direct.h>
#include "md5.h"

extern "C" int bspatch(u_char* old, off_t oldsize, FILE* f, u_char** new_out, off_t* newsize_out);

static void* ReadFile(FILE* fp, size_t size, size_t offset)
{
	void* mem;
	if(!fp) return NULL;
	if(size==0) return NULL;
	if(fseek(fp, offset, SEEK_SET)<0) return NULL;
	mem = malloc(size);
	if(!mem) return NULL;
	if(fread(mem, 1, size, fp)!=size)
	{
		free(mem);
		return NULL;
	}
	return mem;
}

static void MakeDir_P(char* path)
{
	size_t size = strlen(path);
	for(size_t i=2; i<size; i++)
	{
		if(path[i]!='\\' && path[i]!='/') continue;
		path[i] = '\0';
		mkdir(path);
		path[i] = '/';
	}
}

static bool GetMD5(FILE* fp, char* md5)
{
	return true;
}

bool ApplyPatch(const char* patch_file, const char* path)
{
	FILE* patch = fopen(patch_file, "rb");
	int bzerror;
	FILE* tmp_file = NULL;
	FILE* src_file = NULL;
	BZFILE* bzs = NULL;
	BZFILE* bzf = NULL;

	if(!patch)
	{
		goto error;
	}

	char header[16];
	fread(header, 1, 16, patch);
	if(memcmp(header, "JPATCH10", 8)!=0)
	{
		goto error;
	}
	unsigned int ctrl_offset = (unsigned int)(*((unsigned int*)(header+8 )));
	unsigned int data_offset = (unsigned int)(*((unsigned int*)(header+12)));

	bzs = NULL;
	bzf = BZ2_bzReadOpen(&bzerror, patch, 0, 0, NULL, 0);
	if(!bzf)
	{
		goto error;
	}

	for(;;)
	{
		unsigned short len;
		char fname[500], src_md5[60], dst_md5[60];
		BZ2_bzRead(&bzerror, bzf, &len, sizeof(len));
		if(bzerror!=BZ_OK)
		{
			goto error;
		}
		BZ2_bzRead(&bzerror, bzf, &fname, len);
		if(bzerror!=BZ_OK)
		{
			if(bzerror==BZ_STREAM_END)
			{
				break;
			}
			else
			{
				goto error;
			}
		}
		fname[len] = '\0';

		BZ2_bzRead(&bzerror, bzf, &len, sizeof(len));
		if(bzerror!=BZ_OK)
		{
			goto error;
		}
		if(len>=sizeof(src_md5))
		{
			goto error;
		}
		BZ2_bzRead(&bzerror, bzf, &src_md5, len);
		if(bzerror!=BZ_OK)
		{
			goto error;
		}
		src_md5[len] = '\0';

		BZ2_bzRead(&bzerror, bzf, &len, sizeof(len));
		if(bzerror!=BZ_OK)
		{
			goto error;
		}
		if(len>=sizeof(dst_md5))
		{
			goto error;
		}
		BZ2_bzRead(&bzerror, bzf, &dst_md5, len);
		if(bzerror!=BZ_OK)
		{
			goto error;
		}
		dst_md5[len] = '\0';

		char dst_filename[400];
		sprintf(dst_filename, "%s%s", path, fname);
		char tmp_filename[400];
		sprintf(tmp_filename, "%s.tmp", dst_filename);
		bool skip = false;

		src_file = fopen(dst_filename, "rb");
		if(src_file)
		{
			char md5[60];
			if(!GetMD5(src_file, md5))
			{
				goto error;
			}
			if(strcmp(md5, dst_md5)==0)
			{
				skip = true;
			}
			else
			{
				if(src_md5[0]!='\0' && strcmp(md5, src_md5)!=0)
				{
					goto error;
				}
			}
		}
		else
		{
			if(src_md5[0]!='\0')
			{
				goto error;
			}
			MakeDir_P(tmp_filename);
			tmp_file = fopen(tmp_filename, "wb");
			if(!tmp_file)
			{
				goto error;
			}
		}

		for(;;)
		{
			unsigned char code;
			BZ2_bzRead(&bzerror, bzf, &code, sizeof(code));
			if(bzerror!=BZ_OK)
			{
				goto error;
			}
			if(code==OPERATOR_END)
				break;

			if(code==OPERATOR_APPEND)
			{
				printf("%s\n", fname);
				unsigned int size, offset;
				BZ2_bzRead(&bzerror, bzf, &size, sizeof(size));
				if(bzerror!=BZ_OK)
				{
					goto error;
				}
				BZ2_bzRead(&bzerror, bzf, &offset, sizeof(offset));
				if(bzerror!=BZ_OK)
				{
					goto error;
				}
				printf("- append %d %d\n", size, offset);

				if(!skip)
				{
					fseek(patch, offset+data_offset, SEEK_SET);
					bzs = BZ2_bzReadOpen(&bzerror, patch, 0, 0, NULL, 0);
					if(!bzs)
					{
						goto error;
					}
					while(size>0)
					{
						char mem[1000];
						size_t rsize = size>sizeof(mem)?sizeof(mem):size;
						BZ2_bzRead(&bzerror, bzs, mem, rsize);
						if(bzerror!=BZ_OK && size!=rsize && bzerror!=BZ_STREAM_END)
						{
							goto error;
						}
						if(fwrite(mem, 1, rsize, tmp_file)!=rsize)
						{
							goto error;
						}
						size -= rsize;
					}
					BZ2_bzReadClose(&bzerror, bzs);
					bzs = NULL;
					if(bzerror!=BZ_OK)
					{
						goto error;
					}
				}
				continue;
			}

			if(code==OPERATOR_PATCH)
			{
				printf("%s\n", fname);
				unsigned int offset, size, patch_offset;
				BZ2_bzRead(&bzerror, bzf, &offset, sizeof(offset));
				if(bzerror!=BZ_OK)
				{
					goto error;
				}
				BZ2_bzRead(&bzerror, bzf, &size, sizeof(size));
				if(bzerror!=BZ_OK)
				{
					goto error;
				}
				BZ2_bzRead(&bzerror, bzf, &patch_offset, sizeof(patch_offset));
				if(bzerror!=BZ_OK)
				{
					goto error;
				}

				if(!skip)
				{
					void* mem;
					mem = ReadFile(src_file, size, offset);
					if(!mem)
					{
						goto error;
					}

					fseek(patch, patch_offset+data_offset, SEEK_SET);
					u_char* newp;
					off_t newsize;
					if(bspatch((u_char*)mem, (off_t)size, patch, &newp, &newsize)!=0)
					{
						free(mem);
						goto error;
					}
					if(fwrite(newp, 1, newsize, tmp_file)!=newsize)
					{
						free(newp);
						free(mem);
						goto error;
					}
					free(newp);
					free(mem);
				}

				continue;
			}

			if(code==OPERATOR_COPY)
			{
				unsigned int size, offset;
				BZ2_bzWrite(&bzerror, bzf, &size, 4);
				if(bzerror!=BZ_OK)
				{
					goto error;
				}
				BZ2_bzWrite(&bzerror, bzf, &offset, 4);
				if(bzerror!=BZ_OK)
				{
					goto error;
				}

				fseek(src_file, offset, SEEK_SET);
				while(size>0)
				{
					char data[10*1024];
					unsigned int s = size;
					if(s>sizeof(data)) s = sizeof(data);
					if(fread(data, 1, s, src_file)!=s)
					{
						goto error;
					}
					if(fwrite(data, 1, s, tmp_file)!=s)
					{
						goto error;
					}
					size -= s;
				}

				continue;
			}

			goto error;
		}

		if(src_file)
		{
			fclose(src_file);
			src_file = NULL;
		}
		fclose(tmp_file);
		tmp_file = NULL;

		if(_unlink(dst_filename)!=0 && errno!=ENOENT)
			goto error;
		if(rename(tmp_filename, dst_filename)!=0)
			goto error;
	}

	BZ2_bzReadClose(&bzerror, bzf);
	fclose(patch);
	return true;

error:
	if(bzs) BZ2_bzReadClose(&bzerror, bzs);
	if(bzf) BZ2_bzReadClose(&bzerror, bzf);
	if(patch) fclose(patch);
	if(tmp_file) fclose(tmp_file);
	if(src_file) fclose(src_file);
	return false;
}
