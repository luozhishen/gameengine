
struct STRESSCASE_LOGIN_CONFIG
{
	_U32 reset_time;
	_U32 retry_time;
	_U32 disconnect_time;
};
task[GEN_STRUCT_REFLECT(STRESSCASE_LOGIN_CONFIG)];

struct STRESSCASE_LOGIN_STATUS
{
	_U32 failed_times;
	_U32 sucess_times;
};
task[GEN_STRUCT_REFLECT(STRESSCASE_LOGIN_STATUS)];


