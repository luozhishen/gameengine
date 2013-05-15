#ifndef __JPATCH_GEN_INCLUDE__
#define __JPATCH_GEN_INCLUDE__

namespace JPatch
{

	bool GenPatch(const char* patch_file, CBundle& Origin, CBundle& Lastest, bool showdiff);

}

#endif
