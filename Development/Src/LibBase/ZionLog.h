#ifndef __ATLAS_LOG__
#define __ATLAS_LOG__

#define ALOGLVL_DEBUG		0
#define ALOGLVL_INFO		1
#define ALOGLVL_WARN		2
#define ALOGLVL_ERROR		3
#define ALOGLVL_FATAL		4

#define ALOG_DEBUG(l, fmt, ...)				ALOG_HELPER(l, log4boost::priority::PL_INFO, x)
#define ALOG_INFO(l, fmt, ...)				ALOG_HELPER(l, log4boost::priority::PL_INFO, x)
#define ALOG_WARN(l, fmt, ...)				ALOG_HELPER(l, log4boost::priority::PL_WARN, x)
#define ALOG_ERROR(l, fmt, ...)				ALOG_HELPER(l, log4boost::priority::PL_ERROR, x)
#define ALOG_FATAL(l, fmt, ...)				ALOG_HELPER(l, log4boost::priority::PL_FATAL, x)

namespace Zion
{
	class ALogger
	{
	public:
		ALogger();
		~ALogger();

		void Log(_U32 lvl, const char* fmt, ...);
	};
}

#endif
