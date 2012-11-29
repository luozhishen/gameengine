#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rpc_parser.h"
#include "rpc_codegen.h"

static int load_textfile(const char* filename, char* buf, int buflen)
{
	FILE* fp;
	int len = 0;

	fp = fopen(filename, "rt");
	if(fp==NULL) return -1;

	for(;;) {
		if(fgets(buf+len, buflen-len, fp)==NULL) {
			break;
		}
		len += (int)strlen(buf+len);
	}

	fclose(fp);
	return len+1;
}

static int check_input(const char* name)
{
	char xname[1000];
	strcpy(xname, name);
	if(strlen(xname)<4) return 0;
	if(memcmp(xname+strlen(xname)-4, ".rpc", 4)!=0) return 0;
	xname[strlen(xname)-4] = '\0';
	if(strrchr(xname, '//')) memmove(xname, strrchr(xname, '//')+1, strlen(strrchr(xname, '//'))+1);
	if(strrchr(xname, '\\')) memmove(xname, strrchr(xname, '\\')+1, strlen(strrchr(xname, '\\'))+1);
	if(strchr(xname, '.')) return 0;
	return 1;
}

static char strbuf[100*1024];

int update_project(const char* file)
{
	return 0;
}

int main(int argc, char* argv[])
{
	char xname[1980], fname[1980];
	FILE* fp;

	if(argc==3 && strcmp(argv[1], "update")==0)
	{
		return update_project(argv[2]);
	}

	assert(argc==2);
	if(argc!=2) return 0;

	if(load_textfile(argv[1], strbuf, sizeof(strbuf))<0) {
		assert(0);
		return -1;
	}

	if(!rpcgen_parser(strbuf)) {
		printf("failed to parse %s\n", argv[1]);
		return -1;
	}

	if(!code_check()) {
		printf("failed to parse %s\n", argv[1]);
		return -1;
	}

	strcpy(xname, argv[1]);
	xname[strlen(xname)-4] = '\0';
	if(strrchr(xname, '//')) memmove(xname, strrchr(xname, '//')+1, strlen(strrchr(xname, '//'))+1);
	if(strrchr(xname, '\\')) memmove(xname, strrchr(xname, '\\')+1, strlen(strrchr(xname, '\\'))+1);

	strcpy(fname, argv[1]);
	sprintf(fname+strlen(fname)-4, ".c.h");
	fp = fopen(fname, "wt");
	if(!fp) {
		printf("can't open '%s' file\n", fname);
		return -1;
	}
	if(!code_gen_clt_inc(xname, fp)) {
		printf("failed to gen inc file\n");
		return -1;
	}
	fclose(fp);

	strcpy(fname, argv[1]);
	sprintf(fname+strlen(fname)-4, ".c.cpp");
	fp = fopen(fname, "wt");
	if(!fp) {
		printf("can't open inl file\n");
		return -1;
	}
	if(!code_gen_clt_src(xname, fp)) {
		printf("failed to gen inl file\n");
		return -1;
	}
	fclose(fp);

	strcpy(fname, argv[1]);
	sprintf(fname+strlen(fname)-4, ".s.h");
	fp = fopen(fname, "wt");
	if(!fp) {
		printf("can't open '%s' file\n", fname);
		return -1;
	}
	if(!code_gen_svr_inc(xname, fp)) {
		printf("failed to gen inc file\n");
		return -1;
	}
	fclose(fp);

	strcpy(fname, argv[1]);
	sprintf(fname+strlen(fname)-4, ".s.cpp");
	fp = fopen(fname, "wt");
	if(!fp) {
		printf("can't open inl file\n");
		return -1;
	}
	if(!code_gen_svr_src(xname, fp)) {
		printf("failed to gen inl file\n");
		return -1;
	}
	fclose(fp);

	return 0;
}
