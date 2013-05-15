#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "KnightLogicManager.h"
#pragma warning(disable: 4239)

namespace Atlas
{

#if WITH_LUAEXTENTION
	int RegisterLuaDataObj( LuaState* state )// const char* DataObjType, const char* DataObjKey, bool bSyncObj )
	{
		const char* DataObjType;
		const char* DataObjKey;
		bool bSyncObj = false;
		LuaStack LuaArgs(state);
		if( (LuaArgs.Count() == 2 || LuaArgs.Count() == 3) && LuaArgs[1].IsString() && LuaArgs[2].IsString() )
		{
			DataObjType = LuaArgs[1].GetString();
			DataObjKey = LuaArgs[2].GetString();
			if( LuaArgs.Count() == 3 && LuaArgs[3].IsBoolean() )
			{
				bSyncObj = LuaArgs[3].GetBoolean();
			}
		}
		else
		{
			return 0;
		}

		LuaDataObjWrapper* RegisterObjWrapper = NULL;
		const DDLReflect::STRUCT_INFO* StructInfo = Atlas::ContentObject::GetType(DataObjType);
		if( !StructInfo ) return 0;

		const DDLReflect::STRUCT_INFO* pTopStructInfo = StructInfo;
		while( pTopStructInfo->parent && strcmp( pTopStructInfo->parent->name, "A_CONTENT_OBJECT" ) )
		{
			pTopStructInfo = pTopStructInfo->parent;
		}

		Atlas::String DataObjUniqueKey =  pTopStructInfo->name;
		DataObjUniqueKey += DataObjKey;
		Atlas::Map<Atlas::String, LuaDataObjWrapper*>::iterator i = LuaDataObjWrapper::g_LuaDataObj_Map.find(DataObjUniqueKey.c_str());
		if( i != LuaDataObjWrapper::g_LuaDataObj_Map.end() )
			RegisterObjWrapper = i->second;

		if( !RegisterObjWrapper )
		{
			const A_CONTENT_OBJECT* ContentObjectConfig = Atlas::ContentObject::QueryByKey( StructInfo, DataObjKey );
			if( !ContentObjectConfig ) return 0;

			const DDLReflect::STRUCT_INFO* ExactStructType = Atlas::ContentObject::GetObjectType( ContentObjectConfig->uuid );
			assert( DDLReflect::IsParent( ExactStructType, StructInfo ) || StructInfo == ExactStructType );

			RegisterObjWrapper = new LuaDataObjWrapper( ExactStructType, ContentObjectConfig );
			LuaDataObjWrapper::g_LuaDataObj_Map[DataObjUniqueKey] = RegisterObjWrapper;
		}

		LuaObject DataObj = g_LuaState->BoxPointer(RegisterObjWrapper);
		DataObj.SetMetaTable( LuaDataObjWrapper::g_ClassMetaTable );
		RegisterObjWrapper->IncreaseRef();

		if( bSyncObj && RegisterObjWrapper )
		{
			RegisterObjWrapper->SyncObj();
		}

		return 1;
	}

	static int DestroyLuaDataObj( LuaState* state )
	{
		LuaDataObjWrapper* pWrapper = (LuaDataObjWrapper*)(state->UnBoxPointer(1));
		if( pWrapper )
		{
			pWrapper->DecreaseRef();
		}

		return 0;
	}

	// ----------------------- LuaDataObjWrapper --------------------------
	LuaObject										LuaDataObjWrapper::g_ClassMetaTable;
	Atlas::Map<Atlas::String, LuaDataObjWrapper*>	LuaDataObjWrapper::g_LuaDataObj_Map;

	LuaDataObjWrapper::LuaDataObjWrapper( const DDLReflect::STRUCT_INFO* InType, const A_CONTENT_OBJECT* InData ):
		iRefCount(0),
		bSynced(false),
		LuaObjectType( InType ),
		LuaObjectData( InData )
	{
		static bool bLuaWrapperInitialized = false;
		if( !bLuaWrapperInitialized )
		{
			g_ClassMetaTable = g_LuaGlobalObj.CreateTable("LuaDataObjWrapper");
			g_ClassMetaTable.SetObject("__index", g_ClassMetaTable);
			// Register LuaDataObjWrapper interfaces here.
			g_ClassMetaTable.RegisterObjectDirect("GetData", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::GetData );
			g_ClassMetaTable.RegisterObjectDirect("GetExactlyType", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::GetExactlyType );
			g_ClassMetaTable.RegisterObjectDirect( "GetPropValue", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::GetPropertyValue );
			g_ClassMetaTable.RegisterObjectDirect( "GetBool", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::GetBool );
			g_ClassMetaTable.RegisterObjectDirect( "GetNumber", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::GetNumber );
			g_ClassMetaTable.RegisterObjectDirect( "GetString", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::GetString );
			//g_ClassMetaTable.RegisterObjectDirect( "__gc", (LuaDataObjWrapper*)0, &LuaDataObjWrapper::Destroy );
			g_ClassMetaTable.Register( "__gc", DestroyLuaDataObj );

			bLuaWrapperInitialized = true;
		}
	}

		//std::wstring ANSIToUnicode(const Atlas::String& str)
		//{
		//	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		//	wchar_t* pUnicode;
		//	pUnicode = new wchar_t[unicodeLen+1];
		//	memset(pUnicode, 0, (unicodeLen+1)*sizeof(wchar_t));
		//	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
		//	std::wstring rt;
		//	rt = (wchar_t*)pUnicode;
		//	delete pUnicode;
		//	return rt;
		//}

	void LuaDataObjWrapper::Destroy()
	{
		Atlas::Map<Atlas::String, LuaDataObjWrapper*>::iterator i;
		for( i = g_LuaDataObj_Map.begin(); i != g_LuaDataObj_Map.end(); i++ )
		{
			if( i->second == this )
			{
				g_LuaDataObj_Map.erase(i);
				break;
			}
		}
		
		DataObjTable.Remove();
		delete this;
	}

	bool LuaDataObjWrapper::SyncObj()
	{
		if( !bSynced )
		{
			Atlas::String TableName = Atlas::StringFormat("DataObj_Table_%s_%s", LuaObjectType->name, *LuaObjectData->name );
			DataObjTable = g_LuaGlobalObj.CreateTable(TableName.c_str());

			// Sync data.
			DDLReflect::STRUCT_INFO* CachedObjType = ( DDLReflect::STRUCT_INFO*)LuaObjectType;
			while ( CachedObjType != NULL )
			{
				SyncDataStructToLuaTable( CachedObjType, LuaObjectData, DataObjTable );
				CachedObjType = CachedObjType->parent;
			}
			bSynced = true;
		}
		return bSynced;
	}

	void LuaDataObjWrapper::SyncDataFieldToLuaTable( const DDLReflect::FIELD_INFO* finfo, const void* InfoData, LuaObject LuaDataTable, int iMapIndex )
	{
		if( finfo->type & DDLReflect::TYPE_ARRAY )
		{
			DDLReflect::FIELD_INFO FieldInfoCopy = *finfo;
			FieldInfoCopy.type &= DDLReflect::TYPE_MASK;

			_U32 uArrayLength = *(_U32*)InfoData;
			LuaObject LuaDataArray = LuaDataTable.CreateTable( finfo->name );
			for( _U32 ArrayIndex = 0; ArrayIndex < uArrayLength; ++ArrayIndex )
			{
				SyncDataFieldToLuaTable( &FieldInfoCopy, (char*)InfoData + FieldInfoCopy.prefix + FieldInfoCopy.elen*ArrayIndex, LuaDataArray, ArrayIndex + 1 );
			}
			return;
		}

		A_UUID uuid;
		switch( finfo->type )
		{
		case DDLReflect::TYPE_U8:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_U8*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_U8*)(InfoData) );
			break;
		case DDLReflect::TYPE_U16:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_U16*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_U16*)(InfoData) );
			break;
		case DDLReflect::TYPE_U32:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_U32*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_U32*)(InfoData) );
			break;
		case DDLReflect::TYPE_U64:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_U64*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_U64*)(InfoData) );
			break;
		case DDLReflect::TYPE_S8:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_S8*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_S8*)(InfoData) );
			break;									
		case DDLReflect::TYPE_S16:					
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_S16*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_S16*)(InfoData) );
			break;									
		case DDLReflect::TYPE_S32:					
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_S32*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_S32*)(InfoData) );
			break;									
		case DDLReflect::TYPE_S64:					
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_S64*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_S64*)(InfoData) );
			break;
		case DDLReflect::TYPE_F32:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_F32*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_F32*)(InfoData) );
			break;
		case DDLReflect::TYPE_F64:
			iMapIndex > 0 ?	LuaDataTable.SetNumber(iMapIndex, *(_F64*)(InfoData) ) :
								LuaDataTable.SetNumber(finfo->name, *(_F64*)(InfoData) );
			break;
		case DDLReflect::TYPE_STRING:
			//iMapIndex > 0 ?	LuaDataTable.SetWString(iMapIndex, (const lua_WChar *)(ANSIToUnicode((const char*)(InfoData)).c_str()) ) :
			//					LuaDataTable.SetWString(finfo->name, (const lua_WChar *)(ANSIToUnicode((const char*)(InfoData)).c_str()) );
			iMapIndex > 0 ?	LuaDataTable.SetString(iMapIndex, (const char*)(InfoData) ) :
								LuaDataTable.SetString(finfo->name, (const char*)(InfoData) );
			break;
		case DDLReflect::TYPE_UUID:
		case DDLReflect::TYPE_UUID_REF:
			uuid = *(A_UUID*)(InfoData);
			char UuidStr[100];
			AUuidToString(uuid, UuidStr);
			iMapIndex > 0 ?	LuaDataTable.SetString(iMapIndex, UuidStr ) :
								LuaDataTable.SetString(finfo->name, UuidStr );
			break;
		case DDLReflect::TYPE_STRUCT:
			if( iMapIndex > 0 )
			{
				LuaObject LuaDataStruct = LuaDataTable.CreateTable( iMapIndex );
				SyncDataStructToLuaTable( finfo->sinfo, (const A_CONTENT_OBJECT*)InfoData, LuaDataStruct );
			}
			else
			{
				LuaObject LuaDataStruct = LuaDataTable.CreateTable( finfo->name );
				SyncDataStructToLuaTable( finfo->sinfo, (const A_CONTENT_OBJECT*)InfoData, LuaDataStruct );
			}
			break;
		}
	}

	void LuaDataObjWrapper::SyncDataStructToLuaTable( const DDLReflect::STRUCT_INFO* InType, const A_CONTENT_OBJECT* InData, LuaObject LuaDataTable )
	{
		for(_U16 f=0; f<InType->fcount; f++)
		{
			SyncDataFieldToLuaTable( &InType->finfos[f], (void*)((char*)InData + InType->finfos[f].offset), LuaDataTable );
		}
	}


	LuaObject LuaDataObjWrapper::GetData()
	{
		SyncObj();
		return DataObjTable;
	}

	const char* LuaDataObjWrapper::GetExactlyType()
	{
		return LuaObjectType->name;
	}

	const char* LuaDataObjWrapper::GetPropertyValue(const char* PropertyName)
	{
		Atlas::Map<Atlas::String, Atlas::String>::iterator i = CachedDataMap.find(PropertyName);
		if(i==CachedDataMap.end())
		{
			Atlas::String PropValueStr;
			DDLReflect::FIELD_INFO finfo;
			CachedDataMap[PropertyName] = DDLReflect::StructParamToString(LuaObjectType, PropertyName, LuaObjectData, PropValueStr, &finfo)
				? PropValueStr : WRONG_DATAOBJ_PROP_NAME;
		}

		if( CachedDataMap[PropertyName] != WRONG_DATAOBJ_PROP_NAME )
		{
			return CachedDataMap[PropertyName].c_str();
		}

		return NULL;
	}

	bool LuaDataObjWrapper::GetBool(const char* PropertyName)
	{
		const char* PropValue = GetPropertyValue( PropertyName );
		return PropValue ? (atoi( PropValue ) != 0) : false;
	}

	float LuaDataObjWrapper::GetNumber(const char* PropertyName)
	{
		const char* PropValue = GetPropertyValue( PropertyName );
		return PropValue ? atof( PropValue ) : 0.0f;
	}

	const char* LuaDataObjWrapper::GetString(const char* PropertyName)
	{
		return GetPropertyValue( PropertyName );
	}

	// ----------------------- Logic Manager Init ------------------------------

	LuaStateOwner			g_LuaState;
	LuaObject				g_LuaGlobalObj = g_LuaState->GetGlobals();

	const char*		WRONG_DATAOBJ_PROP_NAME = "DATAPROP_NONE";


	void InitLogicManager()
	{
		static bool g_LogiManager_Inited = false;
		if( !g_LogiManager_Inited )
		{
			g_LuaState->OpenLibs();
			g_LuaGlobalObj.Register("RegisterLuaObj", RegisterLuaDataObj );
			// Register other global lua interface here.

			g_LogiManager_Inited = true;
		}
	}

#endif // WITH_LUAEXTENTION

};
