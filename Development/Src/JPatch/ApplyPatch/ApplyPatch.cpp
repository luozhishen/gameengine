#include <ZionBase.h>
#include <Bundle.h>
#include <PatchApply.h>

int main(int argc, char* argv[])
{
	if(argc!=3 && argc!=4)
	{
		printf("invalid parameter\n");
		return 1;
	}
	if(!JPatch::ApplyPatch(argv[1], argv[2], argc==4?argv[2]:NULL))
	{
		printf("error in apply patch\n");
		return 1;
	}
	printf("done\n");
	return 0;
}
