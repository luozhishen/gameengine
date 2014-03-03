
include "CommonDDL.ddl"

const _U32 SYNCFLAG_MODEMASK = 1;
const _U32 SYNCFLAG_BINARY = 0;
const _U32 SYNCFLAG_JSON = 1;
const _U32 SYNCFLAG_CLIENT_ACTIVE = 2;
const _U32 SYNCFLAG_SERVER = 4;

class DATASYNC_JSON_C2S
{
	DS_CreateObject(string type, string data);
	DS_UpdateObject(A_UUID _uuid, string data);
	DS_RemoveObjects(A_UUID _uuid[count], _U32 count);
};

class DATASYNC_BINARY_C2S
{
	DS_CreateObject(_U16 type, _U8 buf[len], _U32 len);
	DS_UpdateObject(A_UUID _uuid, _U8 buf[len], _U32 len);
	DS_RemoveObjects(A_UUID _uuid[count], _U32 count);
};

class DATASYNC_JSON_S2C
{
	DS_SyncOpen(_U32 flag);
	DS_SyncReady();
	DS_SyncClose();

	DS_CreateObjectDone(A_UUID _uuid);
	DS_CreateObject(string type, string json);
	DS_UpdateObject(A_UUID _uuid, string json);
	DS_RemoveObjects(A_UUID _uuid[count], _U32 count);
};

class DATASYNC_BINARY_S2C
{
	DS_SyncOpen(_U32 flag);
	DS_SyncReady();
	DS_SyncClose();

	DS_CreateObjectDone(A_UUID _uuid);
	DS_CreateObject(_U16 type, _U8 buf[len], _U32 len);
	DS_UpdateObject(A_UUID _uuid, _U8 buf[len], _U32 len);
	DS_RemoveObjects(A_UUID _uuid[count], _U32 count);
};

task[GEN_CLASS_STUB(DATASYNC_JSON_C2S)];
task[GEN_CLASS_PROXY(DATASYNC_JSON_C2S)];
task[GEN_CLASS_REFLECT(DATASYNC_JSON_C2S)];
task[GEN_CLASS_STUB(DATASYNC_JSON_S2C)];
task[GEN_CLASS_PROXY(DATASYNC_JSON_S2C)];
task[GEN_CLASS_REFLECT(DATASYNC_JSON_S2C)];
task[GEN_PHP_STUB(DATASYNC_JSON_C2S)];
task[GEN_PHP_PROXY(DATASYNC_JSON_S2C)];

task[GEN_CLASS_STUB(DATASYNC_BINARY_C2S)];
task[GEN_CLASS_PROXY(DATASYNC_BINARY_C2S)];
task[GEN_CLASS_REFLECT(DATASYNC_BINARY_C2S)];
task[GEN_CLASS_STUB(DATASYNC_BINARY_S2C)];
task[GEN_CLASS_PROXY(DATASYNC_BINARY_S2C)];
task[GEN_CLASS_REFLECT(DATASYNC_BINARY_S2C)];
task[GEN_PHP_STUB(DATASYNC_BINARY_C2S)];
task[GEN_PHP_PROXY(DATASYNC_BINARY_S2C)];
