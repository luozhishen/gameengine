#ifndef __JPATCH_APPLY_INCLUDE__
#define __JPATCH_APPLY_INCLUDE__

namespace JPatch
{

	enum PATCH_OPERATOR
	{
		OPERATOR_NA,
		OPERATOR_APPEND	= 1,	// size, patch
		OPERATOR_COPY	= 2,	// size, offset
		OPERATOR_PATCH	= 3,	// offset, size, patch
		OPERATOR_END	= 4,	// end
	};

	bool ApplyPatch(const char* patch_file, const char* m_path, const char* p_path);

}

#endif
