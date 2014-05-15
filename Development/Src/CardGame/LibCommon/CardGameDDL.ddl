
include "../../LibBase/ConfusedNumberDDL.ddl"
include "../../LibCommon/CommonDDL.ddl"
include "../../LibCommon/DataSyncDDL.ddl"

struct CG_AVATAR_DESC : A_LIVE_OBJECT
{
	_U32			avatar_id;
	string<100>		avatar_name;
};

struct CG_AVATAR : A_LIVE_OBJECT
{
	string<100>		avatar_name;
	A_CONFUSED_F32	money;
};

struct CG_CARD : A_LIVE_OBJECT
{
	A_UUID			content_uuid;
	_U32			win_count;
	_U32			lost_count;
};

struct CG_SHOPITEM : A_CONTENT_OBJECT
{
	string<100>		shopitem_id;
	_U32			price;
	A_UUID			content_uuid;
};

struct CG_CARD_CONFIG : A_CONTENT_OBJECT
{
	string<100>		disp_name;
	_U32			point;
};

class CGSERVER_BASE
{
	Ping();

	SelectServer(_U32 server_id);
	GetAvatarList();
	CreateAvatar(string avatar_name);
	EnterGame(_U32 avatar_id);
	LeaveGame();
};

class CGCALLBACK_BASE
{
	Pong();

	GetAvatarListCallback(_U32 errcode, CG_AVATAR_DESC arr[count], _U32 count);
	CreateAvatarCallback(_U32 errcode);
	EnterGameCallback(_U32 errcode);
	LeaveGameCallback(_U32 errcode);
};

class CGSERVER_GAME
{
	Buy(string shopitem_id);
	Discard(A_UUID uuid);

	Beg(_U32 money);
	Gamble(A_UUID card, _S32 mode);
};

class CGCALLBACK_GAME
{
	Ready();

	BegResult(_U32 money);
	GambleResult(_U32 point);
};

task[GEN_CLASS_PROXY(CGSERVER_BASE)];
task[GEN_CLASS_PROXY(CGSERVER_GAME)];
task[GEN_CLASS_SIGSLOT(CGCALLBACK_BASE)];
task[GEN_CLASS_SIGSLOT(CGCALLBACK_GAME)];

task[GEN_PHP_PROXY(DATASYNC_JSON_S2C)];
task[GEN_PHP_STUB(CGSERVER_BASE)];
task[GEN_PHP_STUB(CGSERVER_GAME)];
task[GEN_PHP_PROXY(CGCALLBACK_BASE)];
task[GEN_PHP_PROXY(CGCALLBACK_GAME)];
