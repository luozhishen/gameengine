
include "CommonDDL.ddl"

class DATASYNC_C2S
{
	CreateObject(string type, string data);
	RemoveObject(A_UUID _uuid);
	UpdateObject(A_UUID _uuid, string data);
};

class DATASYNC_S2C
{
	SyncStart();
	SyncReady();
	SyncDisable();

	CreateObjectDone(A_UUID _uuid);
	CreateObject(A_UUID _uuid, string type, string data);
	RemoveObject(A_UUID _uuid);
	UpdateObject(A_UUID _uuid, string data);
};

task[GEN_CLASS_STUB(DATASYNC_C2S)];
task[GEN_CLASS_PROXY(DATASYNC_C2S)];
task[GEN_CLASS_REFLECT(DATASYNC_C2S)];
task[GEN_CLASS_STUB(DATASYNC_S2C)];
task[GEN_CLASS_PROXY(DATASYNC_S2C)];
task[GEN_CLASS_REFLECT(DATASYNC_S2C)];
task[GEN_PHP_STUB(DATASYNC_C2S)];
task[GEN_PHP_PROXY(DATASYNC_S2C)];
