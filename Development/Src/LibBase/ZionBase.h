#ifndef	__ZION_BASE__
#define	__ZION_BASE__

#include "ZionDefines.h"
#include "ZionSTL.h"
#include "ZionSocket.h"
#include "ZionUUID.h"
#include "ZionUtils.h"
#include "ZionHash.h"
#include "DDL.h"
#include "DDLReflect.h"
#include "DDLProxy.h"
#include "DDLStub.h"
#include "DataObject.h"
#include "sigslot.h"
#include "ConfusedNumber.h"

namespace Zion
{

	void ZionInit();
	void ZionFini();

};

#endif
