#include <ZionBase.h>
#include <ZionCommon.h>

#include "DataSyncDDL.h"

namespace Zion
{

	static char _global_game_dir[400] = "";
	const char* ZionGameDir()
	{
		if(_global_game_dir[0]) return _global_game_dir;
		if(!GetExeDir()) return "";
		char path[400];
		char* p;
		strcpy(path, GetExeDir());
		p = strrchr(path, ZION_PATH_SLASH);
		if(!p) return NULL;
		sprintf(p, "%c..%c..%c%s%c", ZION_PATH_SLASH, ZION_PATH_SLASH, ZION_PATH_SLASH, ZionGameName(), ZION_PATH_SLASH);
		strcpy(_global_game_dir, path);
		return _global_game_dir;
	}

	void ZionSetGameDir(const char* path)
	{
		strcpy(_global_game_dir, path);
	}

	static struct {
		const DDLReflect::CLASS_INFO* classinfo;
		bool is_client_stub;
		bool is_server_stub;
		_U32 nodeid;
	}  _global_stubs[256];
	static _U16 _global_stub_count = 0;

	_U16 RegisterClientStub(const DDLReflect::CLASS_INFO* pClassInfo)
	{
		for(_U16 i=0; i<_global_stub_count; i++)
		{
			if(_global_stubs[i].classinfo==pClassInfo)
			{
				_global_stubs[i].is_client_stub = true;
				return i;
			}
		}
		_global_stubs[_global_stub_count].classinfo = pClassInfo;
		_global_stubs[_global_stub_count].is_client_stub = true;
		_global_stubs[_global_stub_count].is_server_stub = false;
		_global_stubs[_global_stub_count].nodeid = (_U32)-1;
		*((_U16*)&pClassInfo->iid) = _global_stub_count;
		return _global_stub_count++;
	}

	_U16 RegisterServerStub(const DDLReflect::CLASS_INFO* pClassInfo, _U32 nodeid)
	{
		for(_U16 i=0; i<_global_stub_count; i++)
		{
			if(_global_stubs[i].classinfo==pClassInfo)
			{
				_global_stubs[i].is_server_stub = true;
				_global_stubs[i].nodeid = nodeid;
				return i;
			}
		}
		_global_stubs[_global_stub_count].classinfo = pClassInfo;
		_global_stubs[_global_stub_count].is_client_stub = false;
		_global_stubs[_global_stub_count].is_server_stub = true;
		_global_stubs[_global_stub_count].nodeid = nodeid;
		*((_U16*)&pClassInfo->iid) = _global_stub_count;
		return _global_stub_count++;
	}

	_U16 GetClientStubID(const DDLReflect::CLASS_INFO* pClassInfo)
	{
		for(_U16 i=0; i<_global_stub_count; i++)
		{
			if(_global_stubs[i].classinfo==pClassInfo) return _global_stubs[i].is_client_stub?i:-1;
		}
		return (_U16)-1;
	}

	_U16 GetServerStubID(const DDLReflect::CLASS_INFO* pClassInfo)
	{
		for(_U16 i=0; i<_global_stub_count; i++)
		{
			if(_global_stubs[i].classinfo==pClassInfo) return _global_stubs[i].is_server_stub?i:-1;
		}
		return (_U16)-1;
	}

	bool GetClientStub(_U16 id, const DDLReflect::CLASS_INFO*& pClassInfo)
	{
		if(id>=_global_stub_count) return false;
		if(!_global_stubs[id].is_client_stub) return false;
		pClassInfo = _global_stubs[id].classinfo;
		return true;
	}

	bool GetClientStub(const char* name, const DDLReflect::CLASS_INFO*& pClassInfo)
	{
		for(_U16 i=0; i<_global_stub_count; i++)
		{
			if(!_global_stubs[i].is_client_stub) continue;
			if(strcmp(_global_stubs[i].classinfo->name, name)!=0) continue;
			pClassInfo = _global_stubs[i].classinfo;
			return true;
		}
		return false;
	}

	bool GetServerStub(_U16 id, const DDLReflect::CLASS_INFO*& pClassInfo, _U32* nodeid)
	{
		if(id>=_global_stub_count) return false;
		if(!_global_stubs[id].is_server_stub) return false;
		pClassInfo = _global_stubs[id].classinfo;
		if(nodeid) *nodeid = _global_stubs[id].nodeid;
		return true;
	}

	bool GetServerStub(const char* name, const DDLReflect::CLASS_INFO*& pClassInfo, _U32* nodeid)
	{
		for(_U16 i=0; i<_global_stub_count; i++)
		{
			if(!_global_stubs[i].is_server_stub) continue;
			if(strcmp(_global_stubs[i].classinfo->name, name)!=0) continue;
			pClassInfo = _global_stubs[i].classinfo;
			if(nodeid) *nodeid = _global_stubs[i].nodeid;
			return true;
		}
		return false;
	}

	bool GetServerFunctionStub(_U16 iid, _U16 fid, const DDLReflect::CLASS_INFO*& cls)
	{
		if(!Zion::GetServerStub(iid, cls)) return false;
		if(fid>=cls->fcount) return false;
		return true;
	}

	bool GetServerFunctionStub(const char* name, const DDLReflect::CLASS_INFO*& cls, _U16& fid)
	{
		char sname[200];
		strcpy(sname, name);
		char* pos = strchr(sname, '.');
		if(!pos) return NULL;
		*(pos++) = '\0';

		const DDLReflect::CLASS_INFO* pClassInfo;
		if(!Zion::GetServerStub(sname, pClassInfo)) return false;

		for(_U16 f=0; f<pClassInfo->fcount; f++)
		{
			if(strcmp(pos, pClassInfo->finfos[f].name)==0)
			{
				cls = pClassInfo;
				fid = f;
				return true;
			}
		}

		return false;
	}

	bool GetClientFunctionStub(_U16 iid, _U16 fid, const DDLReflect::CLASS_INFO*& cls)
	{
		if(!Zion::GetClientStub(iid, cls)) return false;
		if(fid>=cls->fcount) return false;
		return true;
	}

	bool GetClientFunctionStub(const char* name, const DDLReflect::CLASS_INFO*& cls, _U16& fid)
	{
		char sname[200];
		strcpy(sname, name);
		char* pos = strchr(sname, '.');
		if(!pos) return NULL;
		*(pos++) = '\0';

		const DDLReflect::CLASS_INFO* pClassInfo;
		if(!Zion::GetClientStub(sname, pClassInfo)) return false;

		for(_U16 f=0; f<pClassInfo->fcount; f++)
		{
			if(strcmp(pos, pClassInfo->finfos[f].name)==0)
			{
				cls = pClassInfo;
				fid = f;
				return true;
			}
		}

		return false;
	}

	_U32 GetStubCount()
	{
		return _global_stub_count;
	}

	const DDLReflect::CLASS_INFO* GetClientStub(_U32 index)
	{
		if(index>=_global_stub_count) return NULL;
		if(!_global_stubs[index].is_client_stub) return NULL;
		return _global_stubs[index].classinfo;
	}

	const DDLReflect::CLASS_INFO* GetServerStub(_U32 index)
	{
		if(index>=_global_stub_count) return NULL;
		if(!_global_stubs[index].is_server_stub) return NULL;
		return _global_stubs[index].classinfo;
	}

	namespace Engine
	{

		void InitDDLStub()
		{
			RegisterClientStub<DATASYNC_S2C>();
			RegisterServerStub<DATASYNC_C2S>(0);
		}

		void InitContentObjects()
		{
		}

		void InitLiveObjects()
		{
		}

	}

}
