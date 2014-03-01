
include "../../LibCommon/CommonDDL.ddl"

struct CARD_AVATAR_OPERATOR : A_CONTENT_OBJECT
{
};
task[GEN_STRUCT_SERIALIZE(CARD_AVATAR_OPERATOR)];
task[GEN_STRUCT_REFLECT(CARD_AVATAR_OPERATOR)];

struct CARD_AVATAR_DESC : A_LIVE_OBJECT
{
	_U32		avatar_id;
	string<100>	avatar_name;
};
task[GEN_STRUCT_SERIALIZE(CARD_AVATAR_DESC)];
task[GEN_STRUCT_REFLECT(CARD_AVATAR_DESC)];

struct CARD_AVATAR : A_LIVE_OBJECT
{
	string<100>	avatar_name;
};
task[GEN_STRUCT_SERIALIZE(CARD_AVATAR)];
task[GEN_STRUCT_REFLECT(CARD_AVATAR)];

struct CARD_AVATAR_OWNOBJ : A_LIVE_OBJECT
{
	string<100>	obj_name;
	_S32		obj_value;
};
task[GEN_STRUCT_SERIALIZE(CARD_AVATAR_OWNOBJ)];
task[GEN_STRUCT_REFLECT(CARD_AVATAR_OWNOBJ)];

class CARDGAME_C2S
{
	Ping();

	SelectServer(_U32 server_id);
	GetAvatarList();
	CreateAvatar(string avatar_name, _U32 type);
	EnterGame(_U32 avatar_id);
	Change(A_UUID obj_uuid, _S32 value);
	LeaveGame();
};

class CARDGAME_S2C
{
	Pong();

	GetAvatarListCallback(_U32 errcode, CARD_AVATAR_DESC arr[100], _U32 count);
	CreateAvatarCallback(_U32 errcode);
	EnterGameCallback(_U32 errcode);
	SyncAvatar(CARD_AVATAR avatar);
	SyncAvatarOwnObj(CARD_AVATAR_OWNOBJ ownobj);
	SyncCompleted();
	LeaveGameCallback(_U32 errcode);

};

task[GEN_CLASS_STUB(CARDGAME_C2S)];
task[GEN_CLASS_PROXY(CARDGAME_C2S)];
task[GEN_CLASS_REFLECT(CARDGAME_C2S)];
task[GEN_CLASS_STUB(CARDGAME_S2C)];
task[GEN_CLASS_PROXY(CARDGAME_S2C)];
task[GEN_CLASS_REFLECT(CARDGAME_S2C)];

task[GEN_PHP_STUB(CARDGAME_C2S)];
task[GEN_PHP_PROXY(CARDGAME_S2C)];
