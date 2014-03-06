#ifdef	_WIN32

#include "ZionDefines.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static bool zion_fatal_msgbox = false;

void zion_set_fatal(bool msgbox)
{
	zion_fatal_msgbox = true;
}

void zion_fatal(const char* fmt, ...)
{
	char msg[1024];
	va_list args;
	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	va_end(args);

	if(zion_fatal_msgbox)
	{
		MessageBoxA(NULL, msg, "fatal", MB_OK);
	}
	else
	{
		printf("fatal : %s\n", msg);
	}
	abort();
}

#endif
