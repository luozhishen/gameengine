
struct STRESSCASE_LOGIN_CONFIG
{
	_U32 reset_time;
	_U32 retry_time;
	_U32 disconnect_time;
};
task[GEN_STRUCT_REFLECT(STRESSCASE_LOGIN_CONFIG)];
