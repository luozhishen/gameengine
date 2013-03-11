#ifndef __ATLAS_LIVE_DB_API__
#define __ATLAS_LIVE_DB_API__

#include "CommonDDL.h"

namespace Atlas
{

	class CLiveDBTableSingle;
	class CLiveDBTableMulti;

	class CLiveDBTable
	{
		friend class CLiveDBTableSingle;
		friend class CLiveDBTableMulti;
	public:
		enum TABLE_TYPE
		{
			TABLE_TYPE_SINGLE,
			TABLE_TYPE_ARRAY,
		};

		TABLE_TYPE GetTableType() { return m_Type; }

	protected:
		CLiveDBTable(TABLE_TYPE Type, const char* Name);
		virtual ~CLiveDBTable();

	private:
		TABLE_TYPE m_Type;
		const char* m_Name;
	};

	class CLiveDBTableSingle : public CLiveDBTable
	{
	public:
		CLiveDBTableSingle(const char* Name);

		virtual bool Set(DDLReflect::STRUCT_INFO* pInfo, _U32 id, const void* data) = 0;
		virtual bool Get(DDLReflect::STRUCT_INFO* pInfo, _U32 id, const void* data) = 0;
		virtual bool Del(DDLReflect::STRUCT_INFO* pInfo, _U32 id) = 0;

		virtual bool GetList(DDLReflect::STRUCT_INFO* pInfo, _U32 id, Atlas::Vector<A_LIVE_OBJECT>& list) = 0;

	};

	class CLiveDBTableMulti : public CLiveDBTable
	{
	public:
		CLiveDBTableMulti(const char* Name);

		virtual bool Set(DDLReflect::STRUCT_INFO* pInfo, _U32 id, const A_UUID& uuid, const void* data) = 0;
		virtual bool Get(DDLReflect::STRUCT_INFO* pInfo, _U32 id, const A_UUID& uuid, const void* data) = 0;
		virtual bool Del(DDLReflect::STRUCT_INFO* pInfo, _U32 id, const A_UUID& uuid) = 0;
		virtual bool Del(DDLReflect::STRUCT_INFO* pInfo, _U32 id) = 0;

		virtual bool GetList(DDLReflect::STRUCT_INFO* pInfo, _U32 id, Atlas::Vector<A_LIVE_OBJECT>& list) = 0;
	};

	class CLiveDBProvider
	{
	public:
		CLiveDBProvider();
		virtual ~CLiveDBProvider();

		virtual CLiveDBTable* GetTable(const char* name) = 0;

		CLiveDBTableSingle* GetTableSingle(const char* name);
		CLiveDBTableMulti* GetTableMulti(const char* name);
	};

	class CLiveDBProviderFactory
	{
	public:
		CLiveDBProviderFactory(const char* Name);
		~CLiveDBProviderFactory();

		const char* GetName() { return m_Name; }

		virtual CLiveDBProvider* Create(const char* connstr) = 0;

	private:
		const char* m_Name;
	};

	CLiveDBProvider* OpenContentDBProvider(const char* type, const char* connstr);
	void CloseContentDBProvider(CLiveDBProvider* pProvider);
	void RegsiterContentDBOProvider(CLiveDBProviderFactory* pFactory);

}

#endif
