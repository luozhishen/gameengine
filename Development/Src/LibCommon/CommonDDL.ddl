
struct A_CONTENT_OBJECT
{
	[readonly] A_UUID uuid;
	[notnull] string<100> name;
};
task[GEN_STRUCT_SERIALIZE(A_CONTENT_OBJECT)];
task[GEN_STRUCT_REFLECT(A_CONTENT_OBJECT)];

struct A_LIVE_OBJECT
{
	A_UUID uuid;
};
task[GEN_STRUCT_SERIALIZE(A_LIVE_OBJECT)];
task[GEN_STRUCT_REFLECT(A_LIVE_OBJECT)];

struct A_SYNC_OBJECT
{
};
task[GEN_STRUCT_SERIALIZE(A_SYNC_OBJECT)];
task[GEN_STRUCT_REFLECT(A_SYNC_OBJECT)];

class SyncObjectProtocol
{
	CreateDomain(_U16 domain);
	DeleteDomain(_U16 domain);
	CreateObject(_U16 domain, _U32 id, _U16 index, _U16 type, _U32 len, _U8 data[len]);
	UpdateObject(_U16 domain, _U32 id, _U16 index, _U16 type, _U32 len, _U8 data[len]);
	DeleteObject(_U32 domain, _U32 id, _U16 index);
};
task[GEN_CLASS_REFLECT(SyncObjectProtocol)];
task[GEN_CLASS_STUB(SyncObjectProtocol)];
task[GEN_CLASS_PROXY(SyncObjectProtocol)];


struct SG_ITEM
{
	content_ref<SG_ENEMY_CONFIG> fref;
	A_UUID		fuuid;
	_U8			u8;
	_U16		u16;
	_U32		u32;
	_U64		u64;
	_S8			s8;
	_S16		s16;
	_S32		s32;
	_S64		s64;
	_F32		f32;
	_F64		f64;
	string<100>	str;
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM)];
task[GEN_STRUCT_REFLECT(SG_ITEM)];
