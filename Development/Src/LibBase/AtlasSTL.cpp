#include "AtlasSTL.h"

namespace Atlas
{

	void* AtlasMalloc(unsigned int size)
	{
		return malloc((size_t)size);
	}
	void* AtlasRealloc(void* p, unsigned int size)
	{
		return realloc(p, (size_t)size);
	}
	void AtlasFree(void* p)
	{
		free(p);
	}

}
