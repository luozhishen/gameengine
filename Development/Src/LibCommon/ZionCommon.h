#ifndef __ZION_COMMON__
#define __ZION_COMMON__

#include "CommonDDL.h"
#include "ContentObject.h"
#include "LiveObject.h"
#include "LiveData.h"

namespace Zion
{

	const char* GetAppName();
	const char* GetHomeDirectory();
	void SetHomeDirectory(const char* path);

	_U16 RegisterClientStub(const DDLReflect::CLASS_INFO* pClassInfo);
	_U16 RegisterServerStub(const DDLReflect::CLASS_INFO* pClassInfo, _U32 nodeid=-1);

	_U16 GetClientStubID(const DDLReflect::CLASS_INFO* pClassInfo);
	_U16 GetServerStubID(const DDLReflect::CLASS_INFO* pClassInfo);

	bool GetClientStub(_U16 id, const DDLReflect::CLASS_INFO*& pClassInfo);
	bool GetClientStub(const char* name, const DDLReflect::CLASS_INFO*& pClassInfo);
	bool GetServerStub(_U16 id, const DDLReflect::CLASS_INFO*& pClassInfo, _U32* nodeid=NULL);
	bool GetServerStub(const char* name, const DDLReflect::CLASS_INFO*& pClassInfo, _U32* nodeid=NULL);

	bool GetServerFunctionStub(_U16 iid, _U16 fid, const DDLReflect::CLASS_INFO*& cls);
	bool GetServerFunctionStub(const char* name, const DDLReflect::CLASS_INFO*& cls, _U16& fid);
	bool GetClientFunctionStub(_U16 iid, _U16 fid, const DDLReflect::CLASS_INFO*& cls);
	bool GetClientFunctionStub(const char* name, const DDLReflect::CLASS_INFO*& cls, _U16& fid);

	_U32 GetStubCount();
	const DDLReflect::CLASS_INFO* GetClientStub(_U32 index);
	const DDLReflect::CLASS_INFO* GetServerStub(_U32 index);

	template<typename T>
	_U16 RegisterClientStub()
	{
		const DDLReflect::CLASS_INFO* pClassInfo = DDLReflect::GetClass<T>();
		ZION_ASSERT(pClassInfo);
		if(!pClassInfo) return (_U16)-1;
		return RegisterClientStub(pClassInfo);
	}
	template<typename T>
	_U16 RegisterServerStub(_U32 nodeid)
	{
		const DDLReflect::CLASS_INFO* pClassInfo = DDLReflect::GetClass<T>();
		ZION_ASSERT(pClassInfo);
		if(!pClassInfo) return (_U16)-1;
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

	namespace Engine
	{

		void InitDDLStub();
		void InitContentObjects();
		void InitLiveObjects();

	}

}

#endif
