#include <ZionDefines.h>
#include <ZionSTL.h>
#include <stdlib.h>

namespace Zion
{

	void* ZionMalloc(unsigned int size)
	{
		return malloc((size_t)size);
	}
	void* ZionRealloc(void* p, unsigned int size)
	{
		return realloc(p, (size_t)size);
	}
	void ZionFree(void* p)
	{
		free(p);
	}

}
