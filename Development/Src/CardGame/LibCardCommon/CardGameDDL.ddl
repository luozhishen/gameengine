
include "../../LibCommon/CommonDDL.ddl"






struct CARD_CONFIG : A_CONTENT_OBJECT
{
	_U32 iii;
};
task[GEN_STRUCT_SERIALIZE(CARD_CONFIG)];
task[GEN_STRUCT_REFLECT(CARD_CONFIG)];

struct CARD_INVITATION : A_CONTENT_OBJECT
{
	_U32 iii;
};
task[GEN_STRUCT_SERIALIZE(CARD_INVITATION)];
task[GEN_STRUCT_REFLECT(CARD_INVITATION)];

class CARDGAME_C2S
{
	Ping();
};

class CARDGAME_S2C
{
	Pong();
};

task[GEN_CLASS_STUB(CARDGAME_C2S)];
task[GEN_CLASS_PROXY(CARDGAME_C2S)];
task[GEN_CLASS_REFLECT(CARDGAME_C2S)];
task[GEN_CLASS_STUB(CARDGAME_S2C)];
task[GEN_CLASS_PROXY(CARDGAME_S2C)];
task[GEN_CLASS_REFLECT(CARDGAME_S2C)];

task[GEN_PHP_STUB(CARDGAME_C2S)];
task[GEN_PHP_PROXY(CARDGAME_S2C)];
	