#ifndef __ATLAS_COMMON__
#define __ATLAS_COMMON__

#include "ContentObject.h"
#include "LiveObject.h"
#include "SyncCommon.h"
#include "SyncDomain.h"
#include "SyncObject.h"
#include "SyncProxy.h"
#include "SyncMonitor.h"

#include "CommonDDL.h"

namespace Atlas
{

	const char* AtlasGameName();
	const char* AtlasGameDir();

	_U16 RegisterClientStub(const DDLReflect::CLASS_INFO* pClassInfo);
	_U16 RegisterServerStub(const DDLReflect::CLASS_INFO* pClassInfo, _U32 nodeid);

	_U16 GetClientStubID(const DDLReflect::CLASS_INFO* pClassInfo);
	_U16 GetServerStubID(const DDLReflect::CLASS_INFO* pClassInfo);

	bool GetClientStub(_U16 id, const DDLReflect::CLASS_INFO*& pClassInfo);
	bool GetClientStub(const char* name, const DDLReflect::CLASS_INFO*& pClassInfo);
	bool GetServerStub(_U16 id, const DDLReflect::CLASS_INFO*& pClassInfo, _U32* nodeid=NULL);
	bool GetServerStub(const char* name, const DDLReflect::CLASS_INFO*& pClassInfo, _U32* nodeid=NULL);

	bool GetServerFunctionStub(_U16 code, const DDLReflect::CLASS_INFO*& cls, _U16& fid);
	bool GetServerFunctionStub(const char* name, const DDLReflect::CLASS_INFO*& cls, _U16& fid);
	bool GetClientFunctionStub(_U16 code, const DDLReflect::CLASS_INFO*& cls, _U16& fid);
	bool GetClientFunctionStub(const char* name, const DDLReflect::CLASS_INFO*& cls, _U16& fid);

	_U32 GetStubCount();
	const DDLReflect::CLASS_INFO* GetClientStub(_U32 index);
	const DDLReflect::CLASS_INFO* GetServerStub(_U32 index);

	template<typename T>
	_U16 RegisterClientStub()
	{
		const DDLReflect::CLASS_INFO* pClassInfo = DDLReflect::GetClass<T>();
		ATLAS_ASSERT(pClassInfo);
		if(!pClassInfo) return (_U16)-1;
		return RegisterClientStub(pClassInfo);
	}
	template<typename T>
	_U16 RegisterServerStub(_U32 nodeid)
	{
		const DDLReflect::CLASS_INFO* pClassInfo = DDLReflect::GetClass<T>();
		ATLAS_ASSERT(pClassInfo);
		if(!pClassInfo) return -1;
		return RegisterServerStub(pClassInfo, nodeid);
	}

	template<typename T>
	_U16 GetClientStubID()
	{
		static _U16 id = (_U16)-1;
		if(id!=(_U16)-1) return id;
		return GetClientStubID(DDLReflect::GetClass<T>());
	}
	template<typename T>
	_U16 GetServerStubID()
	{
		static _U16 id = (_U16)-1;
		if(id!=(_U16)-1) return id;
		return GetServerStubID(DDLReflect::GetClass<T>());
	}

	void InitDDLStub();
	void InitContentObjects();
	void InitLiveObjects();
	void InitSyncObjects();

	namespace Engine
	{

		void InitDDLStub();
		void InitContentObjects();
		void InitLiveObjects();
		void InitSyncObjects();

	}

}

#endif
