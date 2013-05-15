#ifndef __KNIGHTGAME_LOGICMANAGER__
#define __KNIGHTGAME_LOGICMANAGER__

#define WITH_LUAEXTENTION 0

#include "KnightGameDDL.h"

#if WITH_LUAEXTENTION
#include "LuaPlus.h"
using namespace LuaPlus;
#pragma warning(disable: 4239)
#endif //WITH_LUAEXTENTION

namespace Atlas
{
#if WITH_LUAEXTENTION
	extern LuaStateOwner			g_LuaState;
	extern LuaObject				g_LuaGlobalObj;

	extern const char*				WRONG_DATAOBJ_PROP_NAME;

	void InitLogicManager();

	class LuaDataObjWrapper
	{
	public:
		LuaDataObjWrapper() : iRefCount(0), bSynced(false) {};
		~LuaDataObjWrapper() {};
		const char* GetExactlyType();
		LuaObject GetData();
		const char* GetPropertyValue(const char* PropertyName);
		bool GetBool(const char* PropertyName);
		float GetNumber(const char* PropertyName);
		const char* GetString(const char* PropertyName);

		void IncreaseRef() { iRefCount++; };
		void DecreaseRef() { iRefCount--; if( iRefCount <= 0 ) Destroy(); };

	private:
		LuaDataObjWrapper( const DDLReflect::STRUCT_INFO* InType, const A_CONTENT_OBJECT* InData );
		void Destroy();
		bool SyncObj();
		void SyncDataStructToLuaTable( const DDLReflect::STRUCT_INFO* InType, const A_CONTENT_OBJECT* InData, LuaObject LuaDataTable );
		void SyncDataFieldToLuaTable( const DDLReflect::FIELD_INFO* finfo, const void* InfoData, LuaObject LuaDataTable, int iMapIndex = -1 );
		friend int RegisterLuaDataObj( LuaState* state );


	private:
		LuaObject										DataObjTable;
		bool											bSynced;
		const DDLReflect::STRUCT_INFO*					LuaObjectType;
		const A_CONTENT_OBJECT*							LuaObjectData;
		Atlas::Map<Atlas::String, Atlas::String>		CachedDataMap;
		int												iRefCount;

		static LuaObject										g_ClassMetaTable;
		static Atlas::Map<Atlas::String, LuaDataObjWrapper*>	g_LuaDataObj_Map;
	};

#endif // WITH_LUAEXTENTION
}

#endif // __KNIGHTGAME_LOGICMANAGER__
