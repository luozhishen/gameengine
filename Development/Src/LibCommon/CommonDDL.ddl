
struct A_CONTENT_OBJECT
{
	[readonly] A_UUID _uuid;
	[notnull] string<100> _name;
};
task[GEN_STRUCT_SERIALIZE(A_CONTENT_OBJECT)];
task[GEN_STRUCT_REFLECT(A_CONTENT_OBJECT)];

struct A_LIVE_OBJECT
{
	A_UUID _uuid;
};
task[GEN_STRUCT_SERIALIZE(A_LIVE_OBJECT)];
task[GEN_STRUCT_REFLECT(A_LIVE_OBJECT)];

class DATASYNC_C2S
{
	CreateObject(string type, string data);
	RemoveObject(A_UUID _uuid);
	UpdateObject(A_UUID _uuid, string data);
};

class DATASYNC_S2C
{
	SyncStart(_U16 mode);
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
