#include <string.h>
#include "mosdk_error.h"

int MOError(const char* str)
{
	if(strcmp(str, "0")==0)					return MOERROR_NOERROR;
	if(strcmp(str, "INVALID_PARAMETER")==0)	return MOERROR_INVALID_PARAMETER;
	if(strcmp(str, "AUTH_FAILED")==0)		return MOERROR_AUTH_FAILED;
	if(strcmp(str, "SYSTEM_ERROR")==0)		return MOERROR_SYSTEM_ERROR;
	if(strcmp(str, "LOCKED")==0)			return MOERROR_LOCKED;
	if(strcmp(str, "INVALID_SESSION")==0)	return MOERROR_INVALID_SESSION;
	if(strcmp(str, "ALREADY_EXISTED")==0)	return MOERROR_ALREADY_EXISTED;
	if(strcmp(str, "NOT_EXISTED")==0)		return MOERROR_NOT_EXISTED;
	if(strcmp(str, "UNDEFINE_METHOD")==0)	return MOERROR_UNDEFINE_METHOD;
	if(strcmp(str, "TRY_AGAIN")==0)			return MOERROR_TRY_AGAIN;
	return MOERROR_UNKNOWN;
}
