#include <string.h>
#include "mosdk_error.h"

int MOErrorCode(const char* str)
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
	if(strcmp(str, "INVALID_DATA")==0)		return MOERROR_INVALID_DATA;
	if(strcmp(str, "NETWORK")==0)			return MOERROR_NETWORK;
	if(strcmp(str, "PENDING")==0)			return MOERROR_PENDING;
	return MOERROR_UNKNOWN;
}

const char * MOErrorString(int code)
{
	switch(code)
	{
	case MOERROR_NOERROR:			return "NOERROR";
	case MOERROR_INVALID_PARAMETER:	return "INVALID_PARAMETER";
	case MOERROR_AUTH_FAILED:		return "AUTH_FAILED";
	case MOERROR_SYSTEM_ERROR:		return "SYSTEM_ERROR";
	case MOERROR_LOCKED:			return "LOCKED";
	case MOERROR_INVALID_SESSION:	return "INVALID_SESSION";
	case MOERROR_ALREADY_EXISTED:	return "ALREADY_EXISTED";
	case MOERROR_NOT_EXISTED:		return "NOT_EXISTED";
	case MOERROR_UNDEFINE_METHOD:	return "UNDEFINE_METHOD";
	case MOERROR_TRY_AGAIN:			return "TRY_AGAIN";
	case MOERROR_INVALID_DATA:		return "INVALID_DATA";
	case MOERROR_NETWORK:			return "NETWORK";
	case MOERROR_PENDING:			return "PENDING";
	default:						return "UNKNOWN";
	}
}
