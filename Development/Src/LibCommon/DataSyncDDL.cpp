////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#include <ZionBase.h>
#include "DataSyncDDL.h"

namespace DDLReflect
{
	static FIELD_INFO _class_DATASYNC_JSON_C2S_fieldinfos[] = 
	{
		// 0 DS_CreateObject
		{TYPE_STRING, "type", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_STRING, "data", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 DS_UpdateObject
		{TYPE_UUID, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_STRING, "data", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 DS_RemoveObjects
		{TYPE_UUID|TYPE_ARRAY, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "count", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_DATASYNC_JSON_C2S_funcinfos[] = 
	{
		{"DS_CreateObject", 2, _class_DATASYNC_JSON_C2S_fieldinfos+0},
		{"DS_UpdateObject", 2, _class_DATASYNC_JSON_C2S_fieldinfos+2},
		{"DS_RemoveObjects", 2, _class_DATASYNC_JSON_C2S_fieldinfos+4},
	};
	static CLASS_INFO _class_DATASYNC_JSON_C2S_info = { 0, "DATASYNC_JSON_C2S", 3, _class_DATASYNC_JSON_C2S_funcinfos };
	template<>
	const CLASS_INFO* GetClass<DATASYNC_JSON_C2S>()
	{
		return &_class_DATASYNC_JSON_C2S_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_DATASYNC_JSON_S2C_fieldinfos[] = 
	{
		// 0 DS_SyncOpen
		{TYPE_U32, "flag", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 DS_SyncReady
		// 2 DS_SyncClose
		// 3 DS_CreateObjectDone
		{TYPE_UUID, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 4 DS_CreateObject
		{TYPE_STRING, "type", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_STRING, "json", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 5 DS_UpdateObject
		{TYPE_UUID, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_STRING, "json", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 6 DS_RemoveObjects
		{TYPE_UUID|TYPE_ARRAY, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "count", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_DATASYNC_JSON_S2C_funcinfos[] = 
	{
		{"DS_SyncOpen", 1, _class_DATASYNC_JSON_S2C_fieldinfos+0},
		{"DS_SyncReady", 0, _class_DATASYNC_JSON_S2C_fieldinfos+1},
		{"DS_SyncClose", 0, _class_DATASYNC_JSON_S2C_fieldinfos+1},
		{"DS_CreateObjectDone", 1, _class_DATASYNC_JSON_S2C_fieldinfos+1},
		{"DS_CreateObject", 2, _class_DATASYNC_JSON_S2C_fieldinfos+2},
		{"DS_UpdateObject", 2, _class_DATASYNC_JSON_S2C_fieldinfos+4},
		{"DS_RemoveObjects", 2, _class_DATASYNC_JSON_S2C_fieldinfos+6},
	};
	static CLASS_INFO _class_DATASYNC_JSON_S2C_info = { 0, "DATASYNC_JSON_S2C", 7, _class_DATASYNC_JSON_S2C_funcinfos };
	template<>
	const CLASS_INFO* GetClass<DATASYNC_JSON_S2C>()
	{
		return &_class_DATASYNC_JSON_S2C_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_DATASYNC_BINARY_C2S_fieldinfos[] = 
	{
		// 0 DS_CreateObject
		{TYPE_U16, "type", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U8|TYPE_ARRAY, "buf", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "len", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 DS_UpdateObject
		{TYPE_UUID, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U8|TYPE_ARRAY, "buf", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "len", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 DS_RemoveObjects
		{TYPE_UUID|TYPE_ARRAY, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "count", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_DATASYNC_BINARY_C2S_funcinfos[] = 
	{
		{"DS_CreateObject", 3, _class_DATASYNC_BINARY_C2S_fieldinfos+0},
		{"DS_UpdateObject", 3, _class_DATASYNC_BINARY_C2S_fieldinfos+3},
		{"DS_RemoveObjects", 2, _class_DATASYNC_BINARY_C2S_fieldinfos+6},
	};
	static CLASS_INFO _class_DATASYNC_BINARY_C2S_info = { 0, "DATASYNC_BINARY_C2S", 3, _class_DATASYNC_BINARY_C2S_funcinfos };
	template<>
	const CLASS_INFO* GetClass<DATASYNC_BINARY_C2S>()
	{
		return &_class_DATASYNC_BINARY_C2S_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_DATASYNC_BINARY_S2C_fieldinfos[] = 
	{
		// 0 DS_SyncOpen
		{TYPE_U32, "flag", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 DS_SyncReady
		// 2 DS_SyncClose
		// 3 DS_CreateObjectDone
		{TYPE_UUID, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 4 DS_CreateObject
		{TYPE_U16, "type", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U8|TYPE_ARRAY, "buf", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "len", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 5 DS_UpdateObject
		{TYPE_UUID, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U8|TYPE_ARRAY, "buf", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "len", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 6 DS_RemoveObjects
		{TYPE_UUID|TYPE_ARRAY, "_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "count", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_DATASYNC_BINARY_S2C_funcinfos[] = 
	{
		{"DS_SyncOpen", 1, _class_DATASYNC_BINARY_S2C_fieldinfos+0},
		{"DS_SyncReady", 0, _class_DATASYNC_BINARY_S2C_fieldinfos+1},
		{"DS_SyncClose", 0, _class_DATASYNC_BINARY_S2C_fieldinfos+1},
		{"DS_CreateObjectDone", 1, _class_DATASYNC_BINARY_S2C_fieldinfos+1},
		{"DS_CreateObject", 3, _class_DATASYNC_BINARY_S2C_fieldinfos+2},
		{"DS_UpdateObject", 3, _class_DATASYNC_BINARY_S2C_fieldinfos+5},
		{"DS_RemoveObjects", 2, _class_DATASYNC_BINARY_S2C_fieldinfos+8},
	};
	static CLASS_INFO _class_DATASYNC_BINARY_S2C_info = { 0, "DATASYNC_BINARY_S2C", 7, _class_DATASYNC_BINARY_S2C_funcinfos };
	template<>
	const CLASS_INFO* GetClass<DATASYNC_BINARY_S2C>()
	{
		return &_class_DATASYNC_BINARY_S2C_info;
	}
}

