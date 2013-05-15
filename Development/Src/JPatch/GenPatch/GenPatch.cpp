#include <AtlasBase.h>
#include <Bundle.h>
#include <PatchApply.h>
#include "PatchGen.h"

int main(int argc, char* argv[])
{
	if(argc!=6)
	{
		printf("invalid parameter\n");
		return 1;
	}
	JPatch::CBundle v1, v2;
	if(!v1.InitFromBLS(argv[1], argv[2]))
	{
		printf("error in InitFromBls\n");
		return 1;
	}
	if(!v2.InitFromBLS(argv[3], argv[4]))
	{
		printf("error in InitFromBls\n");
		return 1;
	}
	if(!GenPatch(argv[5], v1, v2, true))
	{
		printf("error in GenPatch\n");
		return 1;
	}
	printf("done.\n");
	return 0;
}
