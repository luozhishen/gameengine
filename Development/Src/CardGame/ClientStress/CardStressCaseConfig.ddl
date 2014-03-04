
struct STRESSCASE_ECHO_CONFIG
{
	_U32 reset_time;
	_U32 retry_time;
	_U32 disconnect_time;
};

struct STRESSCASE_ECHO_STATUS
{
	_U32 failed_times;
	_U32 sucess_time;
};

struct STRESSCASE_ENTERSERVER_CONFIG
{
	_U32 reset_time;
};

struct STRESSCASE_ENTERSERVER_STATUS
{
	_U32 failed_times;
};
