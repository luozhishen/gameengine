
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN 
	#define _WIN32_WINNT 0x0600
	#include "win/async.c"
	#include "win/core.c"
	#include "win/dl.c"
	#include "win/error.c"
	#include "win/fs-event.c"
	#include "win/fs.c"
	#include "win/getaddrinfo.c"
	#include "win/handle.c"
	#include "win/loop-watcher.c"
	#include "win/pipe.c"
	#include "win/poll.c"
	#include "win/process-stdio.c"
	#include "win/process.c"
	#include "win/req.c"
	#include "win/req-inl.h"
	#include "win/signal.c"
	#include "win/stream.c"
	#include "win/tcp.c"
	#include "win/thread.c"
	#include "win/threadpool.c"
	#include "win/timer.c"
	#include "win/tty.c"
	#include "win/udp.c"
	#include "win/util.c"
	#include "win/winapi.c"
	#include "win/winapi.h"
	#include "win/winsock.c"
	char uv_zero_[1] = "";
	const uv_buf_t uv_null_buf_ = { 0, NULL };
#else
	#include "unix/async.c"
	#include "unix/core.c"
	#include "unix/dl.c"
	#include "unix/fs.c"
	#include "unix/getaddrinfo.c"
	#include "unix/internal.h"
	#include "unix/loop-watcher.c"
	#include "unix/loop.c"
	#include "unix/pipe.c"
	#include "unix/poll.c"
	#include "unix/process.c"
	#include "unix/signal.c"
	#include "unix/stream.c"
	#include "unix/tcp.c"
	#include "unix/thread.c"
	#include "unix/threadpool.c"
	#include "unix/timer.c"
	#include "unix/tty.c"
	#include "unix/udp.c"

	#ifdef _AIX
		#define _ALL_SOURCE
		#define _XOPEN_SOURCE 500
		#include "unix/aix.c"
	#endif

	#ifdef _DARWIN
		#define _DARWIN_USE_64_BIT_INODE 1
		#include "unix/darwin.c"
		#include "unix/darwin-proctitle.c"
		#include "unix/fsevents.c"
		#include "unix/kqueue.c"
		#include "unix/proctitle.c"
	#endif

	#ifdef _FREEBSD
		#include "unix/freebsd.c"
		#include "unix/kqueue.c"
	#endif

	#ifdef _LINUX
		#include "unix/linux-core.c"
		#include "unix/linux-inotify.c"
		#include "unix/linux-syscalls.c"
		#include "unix/linux-syscalls.h"
		#include "unix/proctitle.c"
	#endif

	#ifdef _NETBSD
		#include "unix/kqueue.c"
		#include "unix/netbsd.c"
	#endif

	#ifdef _OPENBSD
		#include "unix/kqueue.c"
		#include "unix/openbsd.c"
	#endif

	#ifdef _SUNOS
		#define __EXTENSIONS__
		#define _XOPEN_SOURCE 500
		#include "unix/sunos.c"
	#endif
#endif
