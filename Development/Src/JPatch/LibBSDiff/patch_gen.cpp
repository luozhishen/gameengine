#include <map>
#include <list>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include "utils.h"
#include "bundle.h"
#include "patch.h"
#include "patch_gen.h"
#include "bzip2\bzlib.h"
#include <direct.h>

static bool ReadFile(const char* file, void*& mem, size_t& size)
{
	FILE* fp = fopen(file, "rb");
	if(!fp) return false;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	mem = malloc(size);
	fread(mem, 1, size, fp);
	fclose(fp);
	return true;
}

static bool ReadFile(CFile& file, void*& mem, size_t& size)
{
	std::string name = file._Bundle->_BaseDir + file._Name;
	return ReadFile(name.c_str(), mem, size);
}

class CBundlePatcher
{
public:
	CBundlePatcher()
	{
		_patch = NULL;
		_bzpatch = NULL;
		_stub = NULL;
	}

	~CBundlePatcher()
	{
		Close();
	}

	bool Open(const char* patch_file)
	{
		Close();
		_patch = fopen(patch_file, "wb");
		_stubname = patch_file;
		_stubname += ".stub";
		_stub = fopen(_stubname.c_str(), "wb");
		if(_patch==NULL || _stub==NULL) return false;
		// [HEADER]
		// 0	8 "JPATCH10"
		// 8	4 ctrl block offset
		// 12	4 data block offset
		// [FILE]
		// header
		// ctrl block
		// data block
		if(fwrite("JPATCH10", 1, 8, _patch)!=8) return false;
		if(fwrite("JPATCH10", 1, 8, _patch)!=8) return false;
		int bzerror;
		_bzpatch = BZ2_bzWriteOpen(&bzerror, _patch, 9, 0, 0);
		if(_bzpatch==NULL) return false;
		_Op = OPERATOR_NA;
		return true;
	}

	bool Close(bool success=false)
	{
		void* mem;
		size_t size, offset_ctrl, offset_data;
		int bzerror;
		if(_bzpatch) BZ2_bzWriteClose(&bzerror, _bzpatch, 0, NULL, NULL);
		if(_stub) fclose(_stub);

		if(success)
		{
			//
			if(!ReadFile(_stubname.c_str(), mem, size)) return false;
			fseek(_patch, 0, SEEK_END);
			offset_data = ftell(_patch);
			fwrite(mem, 1, size, _patch);
			free(mem);

			//
			offset_ctrl = 16;
			fseek(_patch, 8, SEEK_SET);
			fwrite(&offset_ctrl, 1, 4, _patch);
			fwrite(&offset_data, 1, 4, _patch);
			if(_unlink(_stubname.c_str())!=0) return false;
		}

		if(_patch) fclose(_patch);
		_patch = NULL;
		_bzpatch = NULL;
		_stub = NULL;
		return true;
	}

	bool DelFile(const std::string& file)
	{
		if(!WriteOp()) return false;
		printf("file %s delete\n", file.c_str());
		unsigned short len = file.size();
		int bzerror;
		BZ2_bzWrite(&bzerror, _bzpatch, &len, sizeof(len));
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, "*", 1);
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, (void*)file.c_str(), len);
		if(bzerror!=BZ_OK) return false;
		return true;
	}

	bool AddFile(const std::string& file, const std::string src_md5, const std::string dst_md5)
	{
		if(!WriteOp()) return false;
		printf("file %s\n", file.c_str());

		int bzerror;
		unsigned short len = file.size();
		BZ2_bzWrite(&bzerror, _bzpatch, &len, sizeof(len));
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, (void*)file.c_str(), len);
		if(bzerror!=BZ_OK) return false;

		len = (unsigned short)src_md5.size();
		BZ2_bzWrite(&bzerror, _bzpatch, &len, sizeof(len));
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, (void*)src_md5.c_str(), len);
		if(bzerror!=BZ_OK) return false;

		len = (unsigned short)dst_md5.size();
		BZ2_bzWrite(&bzerror, _bzpatch, &len, sizeof(len));
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, (void*)dst_md5.c_str(), len);
		if(bzerror!=BZ_OK) return false;

		return true;
	}

	bool AddOperatorAppend(size_t size, size_t _patch_offset)
	{
		if(!WriteOp()) return false;

		printf("-append %d %d\n", size, _patch_offset);

		int bzerror;
		unsigned char code = OPERATOR_APPEND;
		BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, &size, 4);
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, &_patch_offset, 4);
		if(bzerror!=BZ_OK) return false;
		return true;
	}

	bool AddOperatorCopy(size_t size, size_t offset)
	{
		if(_Op!=OPERATOR_COPY)
		{
			if(!WriteOp()) return false;
			_size = 0;
			_offset = offset;
			_Op = OPERATOR_COPY;
		}
		else
		{
			if(_size+_offset!=offset)
			{
				if(!WriteOp()) return false;
				_size = 0;
				_offset = offset;
			}
		}

		_size += size;
		return true;
	}

	bool AddOperatorPatch(size_t offset, size_t size, size_t _patch_offset)
	{
		if(!WriteOp()) return false;

		printf("-patch %d %d %d\n", offset, size, _patch_offset);
		int bzerror;
		unsigned char code = OPERATOR_PATCH;
		BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, &offset, 4);
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, &size, 4);
		if(bzerror!=BZ_OK) return false;
		BZ2_bzWrite(&bzerror, _bzpatch, &_patch_offset, 4);
		if(bzerror!=BZ_OK) return false;
		return true;
	}

	bool AddOperatorEnd()
	{
		if(!WriteOp()) return false;
		printf("-end\n");
		int bzerror;
		unsigned char code = OPERATOR_END;
		BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
		return true;
	}

	bool WriteOp()
	{
		int bzerror;
		unsigned char code = _Op;
		switch(_Op)
		{
		case OPERATOR_NA:
			break;
		case OPERATOR_APPEND:	// size, patch
			BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_size, 4);
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_patch_offset, 4);
			if(bzerror!=BZ_OK) return false;
			break;
		case OPERATOR_COPY:		// size, offset
			printf("-copy %d %d\n", _size, _offset);
			BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_size, 4);
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_offset, 4);
			if(bzerror!=BZ_OK) return false;
			break;
		case OPERATOR_PATCH:	// offset, size, patch
			BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_offset, 4);
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_size, 4);
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_patch_offset, 4);
			if(bzerror!=BZ_OK) return false;
			break;
		default: return false;
		}
		_Op = OPERATOR_NA;
		return true;
	}

	std::string _stubname;
	FILE* _patch;
	BZFILE* _bzpatch;
	FILE* _stub;
	PATCH_OPERATOR _Op;
	size_t _size, _offset, _patch_offset;
};

extern "C" int bsdiff(u_char* old, off_t oldsize, u_char* newp, off_t newsize, FILE* pf);

bool GenPatch(const char* patch_file, CBundle& Origin, CBundle& Lastest)
{
	CBundlePatcher patcher;

	if(!patcher.Open(patch_file)) return false;

	std::map<std::string, CFile>::iterator i;
	for(i=Origin._Files.begin(); i!=Origin._Files.end(); i++)
	{
		if(Lastest._Files.find(i->first)==Lastest._Files.end())
		{
			if(!patcher.DelFile(i->first)) return false;
		}
	}

	for(i=Lastest._Files.begin(); i!=Lastest._Files.end(); i++)
	{
		if(Origin._Files.find(i->first)==Origin._Files.end())
		{
			if(!patcher.AddFile(i->first, "", i->second._MD5)) return false;
			void* mem;
			size_t size, offset;
			if(!ReadFile(i->second, mem, size)) return false;
			offset = ftell(patcher._stub);
			if(!patcher.AddOperatorAppend(size, offset)) return false;
			int bzerror;
			BZFILE* bzf = BZ2_bzWriteOpen(&bzerror, patcher._stub, 9, 0, 0);
			if(bzf==NULL)
			{
				free(mem);
				return false;
			}
			BZ2_bzWrite(&bzerror, bzf, mem, size);
			free(mem);
			if(bzerror!=BZ_OK)
			{
				BZ2_bzWriteClose(&bzerror, bzf, 0, NULL, NULL);
				return false;
			}
			unsigned int inbytes, outbytes;
			BZ2_bzWriteClose(&bzerror, bzf, 0, &inbytes, &outbytes);
			if(bzerror!=BZ_OK)
			{
				return false;
			}
			if(!patcher.AddOperatorEnd()) return false;
		}
		else
		{
			CFile& srcf = Origin._Files[i->first];
			CFile& dstf = i->second;
			if(srcf._MD5!=dstf._MD5)
			{
				if(!patcher.AddFile(i->first, srcf._MD5, dstf._MD5)) return false;
				void* src = NULL;
				void* dst = NULL;
				size_t src_size, dst_size, offset;

				for(size_t i=0; i<dstf._Sections.size(); i++)
				{
					offset = ftell(patcher._stub);

					CSection* s = srcf.GetSectionByMD5(dstf._Sections[i]._MD5);
					if(s==NULL)
					{
						s = srcf.GetSectionByName(dstf._Sections[i]._Name);
					}

					if(s)
					{
						if(s->_MD5==dstf._Sections[i]._MD5)
						{
							// copy
							patcher.AddOperatorCopy(s->_Size, s->_Offset);
						}
						else
						{
							if(src==NULL && !ReadFile(srcf, src, src_size)) return false;
							if(dst==NULL && !ReadFile(dstf, dst, dst_size)) return false;

							// patch
							patcher.AddOperatorPatch(s->_Offset, s->_Size, offset);
							bsdiff((u_char*)src+s->_Offset, s->_Size, (u_char*)dst+dstf._Sections[i]._Offset, dstf._Sections[i]._Size, patcher._stub);
						}
					}
					else
					{
						if(dst==NULL && !ReadFile(dstf, dst, dst_size)) return false;

						// append
						if(!patcher.AddOperatorAppend(dstf._Sections[i]._Size, offset)) return false;
						int bzerror;
						BZFILE* bzf = BZ2_bzWriteOpen(&bzerror, patcher._stub, 9, 0, 0);
						if(bzf==NULL) return false;
						BZ2_bzWrite(&bzerror, bzf, (char*)dst+dstf._Sections[i]._Offset, dstf._Sections[i]._Size);
						if(bzerror!=BZ_OK)
						{
							BZ2_bzWriteClose(&bzerror, bzf, 0, NULL, NULL);
							return false;
						}
						unsigned int inbytes, outbytes;
						BZ2_bzWriteClose(&bzerror, bzf, 0, &inbytes, &outbytes);
						if(bzerror!=BZ_OK)
						{
							return false;
						}
					}
				}

				if(!patcher.AddOperatorEnd()) return false;
				if(src) free(src);
				if(dst) free(dst);
			}
		}
	}

	if(!patcher.DelFile("")) return false;
	if(!patcher.Close(true)) return false;

	return true;
}
