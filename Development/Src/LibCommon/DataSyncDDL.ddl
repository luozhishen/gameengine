
include "CommonDDL.ddl"

const _U32 SYNCFLAG_MODEMASK = 1;
const _U32 SYNCFLAG_BINARY = 0;
const _U32 SYNCFLAG_JSON = 1;
const _U32 SYNCFLAG_CLIENT_ACTIVE = 2;

class DATASYNC_C2S
{
	DS_CreateObject(string type, string data);
	DS_RemoveObject(A_UUID _uuid);
	DS_UpdateObject(A_UUID _uuid, string data);
};

class DATASYNC_S2C
{
	DS_SyncOpen(_U32 flag);
	DS_SyncReady();
	DS_SyncClose();

	DS_CreateObjectDone(A_UUID _uuid);
	DS_CreateObject(A_UUID _uuid, string type, string data);
	DS_CreateObject(A_UUID _uuid, string type, _U8 data[len], _U32 len);
	DS_RemoveObject(A_UUID _uuid);
	DS_UpdateObject(A_UUID _uuid, string data);
	DS_UpdateObject(A_UUID _uuid, _U8 data[len], _U32 len);
};

task[GEN_CLASS_STUB(DATASYNC_C2S)];
task[GEN_CLASS_PROXY(DATASYNC_C2S)];
task[GEN_CLASS_REFLECT(DATASYNC_C2S)];
task[GEN_CLASS_STUB(DATASYNC_S2C)];
task[GEN_CLASS_PROXY(DATASYNC_S2C)];
task[GEN_CLASS_REFLECT(DATASYNC_S2C)];
task[GEN_PHP_STUB(DATASYNC_C2S)];
task[GEN_PHP_PROXY(DATASYNC_S2C)];
