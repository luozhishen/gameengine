include "../../LibCommon/CommonDDL.ddl"

// Basic
const _U32 KNIGHT_DESCRIPTION_MAX = 256;
const _U32 KNIGHT_RESOURCE_URL_MAX = 128;

// Skills
const _U32 KNIGHT_SKILL_NAME_MAX = 16;
const _U32 KNIGHT_SKILL_BUFF_MAX = 8;

struct KNIGHT_SKILL_CONFIG : A_CONTENT_OBJECT
{
	string<KNIGHT_SKILL_NAME_MAX>			SkillName;
	string<KNIGHT_DESCRIPTION_MAX>			SkillDesc;

	string<KNIGHT_RESOURCE_URL_MAX>			ResourceURL;
	_U32									Element;
	_U32									FormulaType;
	_U32									Target;
	_U32									Range;
	_U32									AveValue;
	_F32									Probability;
	array<string<KNIGHT_RESOURCE_URL_MAX>, KNIGHT_SKILL_BUFF_MAX>	BuffPaths;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_SKILL_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_SKILL_CONFIG)];
