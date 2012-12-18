#include "ddl_parser.h"
#include "ddl_codegen.h"
#include "ddl_codegen_php.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>

static FILE* _P = NULL;
static void OutP(int tab, const char* fmt, ...);
static const char* get_phptype(const DDL_ARG* arg);
static int is_struct(const DDL_ARG* arg);

int ddlgen_codephp_open(const char* filename)
{
	_P = fopen(filename, "wt");
	if(!_P) return 0;
	return 1;
}

void ddlgen_codephp_close()
{
	fclose(_P);
	_P = NULL;
}

int ddlgen_codephp_task_struct(const DDL_STR* str, const DDL_TASK* task)
{
	unsigned int a;
	OutP(0, "\n");
	OutP(0, "struct %s%s%s\n", str->name, str->parent[0]?" : ":"", str->parent);
	OutP(0, "{\n");
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		if(arg->count[0]!='\0') {
			OutP(1, "public %s = array();\n", arg->name);
		} else {
			if(is_struct(arg)) {
				OutP(1, "public %s %s;\n", arg->type, arg->name);
			} else {
				OutP(1, "public %s %s;\n", get_phptype(arg), arg->name);
			}
		}
	}

	OutP(1, "public ToArray(_array)\n");
	OutP(1, "{\n");
	OutP(1, "	if(_array==nil) _array = array();\n");
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		if(arg->count[0]!='\0') {
			OutP(2, "__array = array();\n", arg->name);
			OutP(2, "for(__i=0; i<count(%s); i++)\n", arg->name);
			OutP(2, "{\n");
			if(is_struct(arg)) {
				OutP(3, "__array[__i] = %s[__i].ToArray(nil);\n");
			} else {
				OutP(3, "__array[__i] = %s[__i];\n");
			}
			OutP(2, "}\n");
			OutP(2, "_array['%s'] = __array;\n", arg->name);
		} else {
			if(is_struct(arg)) {
				OutP(2, "_array['%s'] = %s.ToArray(nil);\n", arg->name, arg->name);
			} else {
				OutP(2, "_array['%s'] = %s;\n", arg->name, arg->name);
			}
		}
	}
	OutP(1, "}\n");

	OutP(0, "};\n");
	return 1;
}

void OutP(int tab, const char* fmt, ...)
{
    va_list args;
	int i;
	char buf[1000];

	for(i=0; i<tab; i++) OutP(0,  "\t");

    va_start(args, fmt);
    vsprintf(buf, fmt, args);

	fprintf(_P,  "%s", buf);
}

const char* get_phptype(const DDL_ARG* arg)
{
	if(strcmp(arg->type, "string")==0) {
		return "string";
	}
	if(		strcmp(arg->type, "content_ref")==0
		||	strcmp(arg->type, "uuid")==0) {
		return "string";
	}
	if(		strcmp(arg->type, "_U8")==0
		||	strcmp(arg->type, "_U16")==0
		||	strcmp(arg->type, "_U32")==0
		||	strcmp(arg->type, "_U64")==0) {
		return "int";
	}
	if(		strcmp(arg->type, "_S8")==0
		||	strcmp(arg->type, "_S16")==0
		||	strcmp(arg->type, "_S32")==0
		||	strcmp(arg->type, "_S64")==0) {
		return "int";
	}
	if(		strcmp(arg->type, "_F32")==0
		||	strcmp(arg->type, "_F64")==0) {
		return "float";
	}
	return NULL;
}

int is_struct(const DDL_ARG* arg)
{
	if(strcmp(arg->type, "string")==0
		||	strcmp(arg->type, "content_ref")==0
		||	strcmp(arg->type, "uuid")==0
		||	strcmp(arg->type, "_U8")==0
		||	strcmp(arg->type, "_U16")==0
		||	strcmp(arg->type, "_U32")==0
		||	strcmp(arg->type, "_U64")==0
		||	strcmp(arg->type, "_S8")==0
		||	strcmp(arg->type, "_S16")==0
		||	strcmp(arg->type, "_S32")==0
		||	strcmp(arg->type, "_S64")==0
		||	strcmp(arg->type, "_F32")==0
		||	strcmp(arg->type, "_F64")==0) {
			return 0;
	}
	return 1;
}
