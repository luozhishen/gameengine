#include <ZionBase.h>

#include "PatchApply.h"
#include "bzlib.h"
#include <direct.h>

extern "C" int bspatch(_U8* old, off_t oldsize, FILE* f, _U8** new_out, off_t* newsize_out);

namespace JPatch
{

	static void* ReadFile(FILE* fp, size_t size, size_t offset)
	{
		void* mem;
		if(!fp) return NULL;
		if(size==0) return NULL;
		if(fseek(fp, (long)offset, SEEK_SET)<0) return NULL;
		mem = malloc(size);
		if(!mem) return NULL;
		if(fread(mem, 1, size, fp)!=size)
		{
			free(mem);
			return NULL;
		}
		return mem;
	}

	static bool GetMD5(FILE* fp, Zion::String& md5_v)
	{
		Zion::CHashMD5 md5;
		fseek(fp, 0, SEEK_SET);
		char buf[2000];
		while(!feof(fp))
		{
			size_t s = fread(buf, 1, sizeof(buf), fp);
			if(s>0)
			{
				md5.Update(buf, (_U32)s);
			}
			else
			{
				return false;
			}
		}
		md5.Final(md5_v);
		return true;
	}

	bool ApplyPatch(const char* patch_file, const char* m_path, const char* p_path)
	{
		FILE* patch = fopen(patch_file, "rb");
		int bzerror;
		FILE* tmp_file = NULL;
		FILE* src_file = NULL;
		BZFILE* bzs = NULL;
		BZFILE* bzf = NULL;

		if(!patch)
		{
			printf("open patch failed!\n");
			goto error;
		}

		char header[16];
		fread(header, 1, 16, patch);
		if(memcmp(header, "JPATCH10", 8)!=0)
		{
			printf("patch file invalid file header\n");
			goto error;
		}
		unsigned int ctrl_offset = (unsigned int)(*((unsigned int*)(header+8 )));
		unsigned int data_offset = (unsigned int)(*((unsigned int*)(header+12)));

		bzf = BZ2_bzReadOpen(&bzerror, patch, 0, 0, NULL, 0);
		if(!bzf)
		{
			printf("error in BZ2_bzReadOpen(%d)\n", bzerror);
			goto error;
		}

		for(;;)
		{
			unsigned short len;
			char fname[500], src_md5[60], dst_md5[60];
			BZ2_bzRead(&bzerror, bzf, &len, sizeof(len));
			if(bzerror!=BZ_OK)
			{
				printf("error in BZ2_bzRead(%d) @1\n", bzerror);
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
					printf("error in BZ2_bzRead(%d) @2\n", bzerror);
					goto error;
				}
			}
			fname[len] = '\0';

			if(fname[0]=='*')
			{
				if(fname[1]=='\0')
				{
					break;
				}
				else
				{
					Zion::String del_filename = Zion::StringFormat("%s%s", m_path, fname);
					if(_unlink(del_filename.c_str())!=0 && errno!=ENOENT)
					{
						printf("failed to delete %s\n", del_filename.c_str());
						goto error;
					}
					continue;
				}
			}

			BZ2_bzRead(&bzerror, bzf, &len, sizeof(len));
			if(bzerror!=BZ_OK)
			{
				printf("error in BZ2_bzRead(%d) @3\n", bzerror);
				goto error;
			}
			if(len>=sizeof(src_md5))
			{
				goto error;
			}
			BZ2_bzRead(&bzerror, bzf, &src_md5, len);
			if(bzerror!=BZ_OK)
			{
				printf("error in BZ2_bzRead(%d) @4\n", bzerror);
				goto error;
			}
			src_md5[len] = '\0';

			BZ2_bzRead(&bzerror, bzf, &len, sizeof(len));
			if(bzerror!=BZ_OK)
			{
				printf("error in BZ2_bzRead(%d) @5\n", bzerror);
				goto error;
			}
			if(len>=sizeof(dst_md5))
			{
				printf("error in BZ2_bzRead(%d) @6\n", bzerror);
				goto error;
			}
			BZ2_bzRead(&bzerror, bzf, &dst_md5, len);
			if(bzerror!=BZ_OK)
			{
				printf("error in BZ2_bzRead(%d) @7\n", bzerror);
				goto error;
			}
			dst_md5[len] = '\0';

			Zion::String dst_filename = Zion::StringFormat("%s%s", m_path, fname);
			Zion::String tmp_filename = Zion::StringFormat("%s.t", dst_filename.c_str());
			bool skip = false;

			if(src_md5[0]!='\0')
			{
				src_file = fopen(dst_filename.c_str(), "rb");
				if(!src_file)
				{
					if(p_path)
					{
						Zion::String p_filename = Zion::StringFormat("%s%s", p_path, fname);
						src_file = fopen(p_filename.c_str(), "rb");
					}
					if(src_file==NULL)
					{
						printf("can't open %s\n", fname);
						goto error;
					}
				}

				Zion::String md5;
				if(!GetMD5(src_file, md5))
				{
					printf("error in GetMD5\n");
					goto error;
				}
				if(md5==dst_md5)
				{
					skip = true;
				}
				else
				{
					if(src_md5[0]!='\0' && md5!=src_md5)
					{
						printf("invalid local file %s\n", fname);
						goto error;
					}
				}
			}

			if(!skip)
			{
				//if(!Zion::MakeDirectory(tmp_filename.c_str()))
				//{
				//	printf("error in MakeDirectory\n");
				//	goto error;
				//}

				tmp_file = fopen(tmp_filename.c_str(), "wb");
				if(!tmp_file)
				{
					printf("failed to open %s\n", tmp_filename.c_str());
					goto error;
				}

				printf("file %s\n", fname);
			}
			else
			{
				printf("file %s skip\n", fname);
			}

			for(;;)
			{
				unsigned char code;
				BZ2_bzRead(&bzerror, bzf, &code, sizeof(code));
				if(bzerror!=BZ_OK)
				{
					printf("error in BZ2_bzRead(%d) @8\n", bzerror);
					goto error;
				}
				if(code==OPERATOR_END)
					break;

				if(code==OPERATOR_APPEND)
				{
					_U32 size, offset;
					BZ2_bzRead(&bzerror, bzf, &size, sizeof(size));
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @A1\n", bzerror);
						goto error;
					}
					BZ2_bzRead(&bzerror, bzf, &offset, sizeof(offset));
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @A2\n", bzerror);
						goto error;
					}
					printf("- append %d %d\n", size, offset);

					if(!skip)
					{
						fseek(patch, offset+data_offset, SEEK_SET);
						bzs = BZ2_bzReadOpen(&bzerror, patch, 0, 0, NULL, 0);
						if(!bzs)
						{
							printf("error in BZ2_bzRead(%d) @A3\n", bzerror);
							goto error;
						}
						while(size>0)
						{
							char mem[1000];
							size_t rsize = size>sizeof(mem)?sizeof(mem):size;
							BZ2_bzRead(&bzerror, bzs, mem, (int)rsize);
							if(bzerror!=BZ_OK && size!=(_U32)rsize && bzerror!=BZ_STREAM_END)
							{
								printf("error in BZ2_bzRead(%d) @A4\n", bzerror);
								goto error;
							}
							if(fwrite(mem, 1, rsize, tmp_file)!=rsize)
							{
								printf("failed to write tmp_file\n");
								goto error;
							}
							size -= (_U32)rsize;
						}
						BZ2_bzReadClose(&bzerror, bzs);
						bzs = NULL;
						if(bzerror!=BZ_OK)
						{
							printf("error in BZ2_bzReadClose @A\n", bzerror);
							goto error;
						}
					}
					continue;
				}

				if(code==OPERATOR_PATCH)
				{
					unsigned int offset, size, patch_offset;
					BZ2_bzRead(&bzerror, bzf, &offset, sizeof(offset));
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @P1\n", bzerror);
						goto error;
					}
					BZ2_bzRead(&bzerror, bzf, &size, sizeof(size));
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @P2\n", bzerror);
						goto error;
					}
					BZ2_bzRead(&bzerror, bzf, &patch_offset, sizeof(patch_offset));
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @P3\n", bzerror);
						goto error;
					}
					printf("- patch %d %d %d\n", offset, size, patch_offset);

					if(!skip)
					{
						void* mem;
						mem = ReadFile(src_file, size, offset);
						if(!mem)
						{
							printf("failed to read source file @P\n");
							goto error;
						}

						fseek(patch, patch_offset+data_offset, SEEK_SET);
						_U8* newp;
						off_t newsize;
						if(bspatch((_U8*)mem, (off_t)size, patch, &newp, &newsize)!=0)
						{
							printf("error in bspatch\n");
							free(mem);
							goto error;
						}
						if(fwrite(newp, 1, newsize, tmp_file)!=newsize)
						{
							printf("failed to write tmp_file @P\n");
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
					BZ2_bzRead(&bzerror, bzf, &size, 4);
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @C3\n", bzerror);
						goto error;
					}
					BZ2_bzRead(&bzerror, bzf, &offset, 4);
					if(bzerror!=BZ_OK)
					{
						printf("error in BZ2_bzRead(%d) @C3\n", bzerror);
						goto error;
					}
					printf("- copy %d %d\n", size, offset);

					if(!skip)
					{
						fseek(src_file, offset, SEEK_SET);
						while(size>0)
						{
							char data[10*1024];
							unsigned int s = size;
							if(s>sizeof(data)) s = sizeof(data);
							if(fread(data, 1, s, src_file)!=s)
							{
								printf("failed to read source file @C\n", bzerror);
								goto error;
							}
							if(fwrite(data, 1, s, tmp_file)!=s)
							{
								printf("failed to write temp file @C\n", bzerror);
								goto error;
							}
							size -= s;
						}
					}

					continue;
				}

				printf("invalid opcode\n");
				goto error;
			}

			if(src_file)
			{
				fclose(src_file);
				src_file = NULL;
			}

			if(!skip)
			{
				fclose(tmp_file);
				tmp_file = NULL;

				if(_unlink(dst_filename.c_str())!=0 && errno!=ENOENT)
				{
					printf("failed to delete %s\n", dst_filename.c_str());
					goto error;
				}
				if(rename(tmp_filename.c_str(), dst_filename.c_str())!=0)
				{
					printf("failed to rename %s %s\n", dst_filename.c_str(), tmp_filename.c_str());
					goto error;
				}
			}
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

}
