#ifndef	__ATLAS_BASE__
#define	__ATLAS_BASE__

#include <string>
#ifndef _WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

#include "ZionDefines.h"
#include "ZionSTL.h"
#include "ZionSocket.h"
#include "AsyncSockIO.h"
#include "DDL.h"
#include "DDLReflect.h"
#include "ZionUtils.h"
#include "AsyncRPC.h"
#include "ZionUUID.h"
#include "ZionHash.h"
#include "sigslot.h"

namespace Zion
{

	void AtlasInit();
	void AtlasFini();

};

#endif
