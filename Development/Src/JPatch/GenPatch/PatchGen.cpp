#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <bzlib.h>
#include <direct.h>

#include <ZionBase.h>

#include <Bundle.h>
#include <PatchApply.h>
#include "PatchGen.h"

#include "bzlib.h"

extern "C" int bsdiff(u_char* old, off_t oldsize, u_char* newp, off_t newsize, FILE* pf);

namespace JPatch
{

	static bool JReadFile(const char* file, void*& mem, _U32& size)
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

	static bool JReadFile(CFile& file, void*& mem, _U32& size)
	{
		Zion::String name = file._Bundle->_BaseDir + file._Name;
		return JReadFile(name.c_str(), mem, size);
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
			_U32 size, offset_ctrl, offset_data;
			int bzerror;
			if(_bzpatch) BZ2_bzWriteClose(&bzerror, _bzpatch, 0, NULL, NULL);
			if(_stub) fclose(_stub);

			if(success)
			{
				//
				if(!JReadFile(_stubname.c_str(), mem, size)) return false;
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

		bool DelFile(const Zion::String& file)
		{
			if(!WriteOp()) return false;
			printf("file %s delete\n", file.c_str());
			_U16 len = (_U16)file.size()+1;
			int bzerror;
			BZ2_bzWrite(&bzerror, _bzpatch, &len, sizeof(len));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, "*", 1);
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, (void*)file.c_str(), len);
			if(bzerror!=BZ_OK) return false;
			return true;
		}

		bool AddFile(const Zion::String& file, const Zion::String& src_md5, const Zion::String& dst_md5)
		{
			if(!WriteOp()) return false;
			printf("file %s\n", file.c_str());

			int bzerror;
			_U16 len = (_U16)file.size();
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

		bool AddOperatorAppend(_U32 size, _U32 _patch_offset)
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

		bool AddOperatorCopy(_U32 size, _U32 offset)
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

		bool AddOperatorPatch(_U32 offset, _U32 size, _U32 _patch_offset)
		{
			if(!WriteOp()) return false;

			printf("-patch %d %d %d\n", offset, size, _patch_offset);
			int bzerror;
			unsigned char code = OPERATOR_PATCH;
			BZ2_bzWrite(&bzerror, _bzpatch, &code, sizeof(code));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &offset, sizeof(offset));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &size, sizeof(size));
			if(bzerror!=BZ_OK) return false;
			BZ2_bzWrite(&bzerror, _bzpatch, &_patch_offset, sizeof(_patch_offset));
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
			_U8 code = (_U8)_Op;
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

		Zion::String _stubname;
		FILE* _patch;
		BZFILE* _bzpatch;
		FILE* _stub;
		PATCH_OPERATOR _Op;
		_U32 _size, _offset, _patch_offset;
	};

	void ShowObjectDiff(const void* mem1, const void* mem2, _U32 size)
	{
		_U32 start = (_U32)-1;
		_U32 len = 0;
		for(_U32 i=0; i<size; i++)
		{
			if(*((const _U8*)mem1+i)==*((const _U8*)mem2+i))
			{
				if(start!=(_U32)-1)
				{
					printf("[DIFF-INFO] DATA %u %u\n", start, len);
					start = (_U32)-1;
				}
			}
			else
			{
				if(start==(_U32)-1)
				{
					start = i;
					len = 1;
				}
				else
				{
					len++;
				}
			}
		}
		if(start!=(_U32)-1)
		{
			printf("[DIFF-INFO] DATA %u %u\n", start, len);
		}
	}

	bool GenPatch(const char* patch_file, CBundle& Origin, CBundle& Lastest, bool showdiff)
	{
		CBundlePatcher patcher;

		if(!patcher.Open(patch_file)) return false;

		Zion::Map<Zion::String, CFile>::iterator i;
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
				_U32 size, offset;
				if(!JReadFile(i->second, mem, size)) return false;
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
					_U32 src_size = 0, dst_size = 0, offset;

					for(_U32 section_i=0; section_i<dstf._Sections.size(); section_i++)
					{
						offset = ftell(patcher._stub);

						CSection* dst_section = &dstf._Sections[section_i];
						CSection* src_section = srcf.GetSectionByName(dst_section->_Name);
						if(src_section==NULL)
						{
							CSection* src_section = srcf.GetSectionByMD5(dst_section->_MD5);
						}
						else
						{
							if(src_section->_MD5!=dst_section->_MD5)
							{
								src_section = srcf.GetSectionByMD5(dst_section->_MD5);
								if(src_section==NULL)
								{
									src_section = srcf.GetSectionByName(dst_section->_Name);
								}
							}
						}

						if(src_section)
						{
							if(src_section->_MD5==dstf._Sections[section_i]._MD5)
							{
								// copy
								patcher.AddOperatorCopy(src_section->_Size, src_section->_Offset);
							}
							else
							{
								if(src==NULL && !JReadFile(srcf, src, src_size)) return false;
								if(dst==NULL && !JReadFile(dstf, dst, dst_size)) return false;


								if(src_section->_Size==dstf._Sections[section_i]._Size && showdiff)
								{
									printf("[DIFF-INFO] OBJECT %s %s %u %u %u\n", i->first.c_str(), src_section->_Name.c_str(), src_section->_Offset, dstf._Sections[section_i]._Offset, src_section->_Size);
									ShowObjectDiff((u_char*)src+src_section->_Offset, (u_char*)dst+dstf._Sections[section_i]._Offset, src_section->_Size);
								}

								// patch
								patcher.AddOperatorPatch(src_section->_Offset, src_section->_Size, offset);
								bsdiff((u_char*)src+src_section->_Offset, src_section->_Size, (u_char*)dst+dstf._Sections[section_i]._Offset, dstf._Sections[section_i]._Size, patcher._stub);
							}
						}
						else
						{
							if(dst==NULL && !JReadFile(dstf, dst, dst_size)) return false;

							// append
							if(!patcher.AddOperatorAppend(dstf._Sections[section_i]._Size, offset)) return false;
							int bzerror;
							BZFILE* bzf = BZ2_bzWriteOpen(&bzerror, patcher._stub, 9, 0, 0);
							if(bzf==NULL) return false;
							BZ2_bzWrite(&bzerror, bzf, (char*)dst+dstf._Sections[section_i]._Offset, dstf._Sections[section_i]._Size);
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

}
