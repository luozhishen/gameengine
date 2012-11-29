#ifndef	__ATLAS_BASE__
#define	__ATLAS_BASE__

#include <string>

#include "AtlasDefines.h"
#include "AsyncSockIO.h"
#include "MemPool.h"
#include "DDL.h"
#include "DDLReflect.h"
#include "AtlasUtils.h"
#include "AsyncRPC.h"
#include "AtlasUUID.h"
#include "sigslot.h"

namespace Atlas
{

	void AtlasInit();
	void AtlasFini();

};

#endif
