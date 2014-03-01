#ifndef	__ZION_BASE__
#define	__ZION_BASE__

#ifndef _WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

#include "ZionDefines.h"
#include "ZionSTL.h"
#include "ZionSocket.h"
#include "ZionUUID.h"
#include "ZionUtils.h"
#include "DDL.h"
#include "DDLReflect.h"
#include "DDLDataObject.h"
#include "DDLProxy.h"
#include "DDLStub.h"
#include "ZionHash.h"
#include "sigslot.h"

namespace Zion
{

	void ZionInit();
	void ZionFini();

};

#endif
