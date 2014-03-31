
#include "../LibBase/ZionDefines.h"
#include "../LibBase/ZionUnix.cpp"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	A_SLIST_HEADER* header = (A_SLIST_HEADER*)malloc(sizeof(A_SLIST_HEADER));
	A_SLIST_ENTRY* entry = (A_SLIST_ENTRY*)malloc(sizeof(A_SLIST_ENTRY)*1000000);
	A_SLIST_INIT(header);
	printf("%p %p\n", header, &entry[1]);
	for(_U32 i=0; i<10; i++)
	{
		A_SLIST_PUSH(header, entry+1);
		ZION_ASSERT(entry+1==A_SLIST_POP(header));
	}
	return 0;
}
