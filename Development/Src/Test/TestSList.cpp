
#include "../LibBase/ZionDefines.h"
#include "../LibBase/ZionUnix.cpp"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	A_SLIST_HEADER* header = (A_SLIST_HEADER*)malloc(sizeof(A_SLIST_HEADER));

	A_SLIST_ENTRY x;
	A_SLIST_ENTRY* ret;
	A_SLIST_PUSH(header, &x);
	printf("%p %d\n", header->First, (int)header->Count);
	ret = A_SLIST_POP(header);
	printf("%p %p\n", &x, ret);
	ZION_ASSERT(&x==ret);

	A_SLIST_ENTRY* entry = (A_SLIST_ENTRY*)malloc(sizeof(A_SLIST_ENTRY)*1000000);
	printf("%p\n", entry);
	A_SLIST_INIT(header);
	A_SLIST_PUSH(header, entry);
	ZION_ASSERT(entry==A_SLIST_POP(header));
	return 0;
}

