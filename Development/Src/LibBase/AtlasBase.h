#ifndef	__ATLAS_BASE__
#define	__ATLAS_BASE__

#include <string>
#ifndef _WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

#include "AtlasDefines.h"
#include "AtlasSTL.h"
#include "AtlasSocket.h"
#include "AsyncSockIO.h"
#include "DDL.h"
#include "DDLReflect.h"
#include "AtlasUtils.h"
#include "AsyncRPC.h"
#include "AtlasUUID.h"
#include "AtlasHash.h"
#include "sigslot.h"

namespace Atlas
{

	void AtlasInit();
	void AtlasFini();

};

#endif
