#ifndef	__ZION_BASE__
#define	__ZION_BASE__

#include <string>
#ifndef _WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

#include "ZionDefines.h"
#include "ZionSTL.h"
#include "ZionSocket.h"
#include "ZionUUID.h"
#include "DDL.h"
#include "DDLReflect.h"
#include "ZionUtils.h"
#include "ZionHash.h"
#include "sigslot.h"

namespace Zion
{

	void ZionInit();
	void ZionFini();

};

#endif
