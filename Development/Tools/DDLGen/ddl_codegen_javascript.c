#include "ddl_parser.h"
#include "ddl_codegen.h"
#include "ddl_codegen_php.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>

static FILE* _P = NULL;
static void OutJS(int tab, const char* fmt, ...);
static const char* get_phptype(const DDL_ARG* arg);
static int is_struct(const DDL_ARG* arg);
static int need_range(const DDL_ARG* arg);
static const char* range_min(const DDL_ARG* arg);
static const char* range_max(const DDL_ARG* arg);

int ddlgen_codejs_open(const char* filename)
{
	_P = fopen(filename, "wt");
	if(!_P) return 0;
	return 1;
}

void ddlgen_codejs_close()
{
	fclose(_P);
	_P = NULL;
}

int ddlgen_codejs_task_struct(const DDL_STR* str, const DDL_TASK* task)
{
	unsigned int a;
	OutJS(0, "\n");

	OutJS(0, "function %s() {\n", str->name);
	if(str->parent[0]!='\0') {
		OutJS(1, "%s.call(this);\n", str->parent);
	}
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		if(arg->count[0]!='\0') {
			OutJS(1, "this.%s = [];\n", arg->name);
		} else {
			if(		strcmp(arg->type, "_U8")==0
				||	strcmp(arg->type, "_U16")==0
				||	strcmp(arg->type, "_U32")==0
				||	strcmp(arg->type, "_U64")==0
				||	strcmp(arg->type, "_S8")==0
				||	strcmp(arg->type, "_S16")==0
				||	strcmp(arg->type, "_S32")==0
				||	strcmp(arg->type, "_S64")==0) {
				OutJS(1, "this.%s = 0;\n", arg->name);
			} else if(strcmp(arg->type, "_F32")==0
				||	strcmp(arg->type, "_F64")==0) {
				OutJS(1, "this.%s = 0.0;\n", arg->name);
			} else if(strcmp(arg->type, "string")==0) {
				OutJS(1, "this.%s = '';\n", arg->name);
			} else if(strcmp(arg->type, "A_UUID")==0) {
				OutJS(1, "this.%s = '{00000000-0000-0000-0000-000000000000}';\n", arg->name);
			} else if(is_struct(arg)) {
				OutJS(1, "this.%s = new %s();\n", arg->name, arg->type);
			}
		}
	}
	OutJS(0, "	this.__proto__ = %s;\n", str->name);
	OutJS(0, "}\n");
	OutJS(0, "%s.serialize = function() {\n", str->name);
	if(str->parent[0]!='\0') {
		OutJS(1, "%s.serialize.call(this);\n", str->parent);
	}
	OutJS(0, "	console.log('%s');\n", str->name);
	OutJS(0, "}\n");
	if(str->parent[0]!='\0') {
		OutJS(0, "%s.prototype = %s;\n", str->name, str->parent);
	}
	OutJS(0, "module.exports.%s = %s;\n", str->name, str->name);

	return 1;
}

int ddlgen_codejs_task_class_stub(const DDL_CLS* cls, const DDL_TASK* task)
{
	return 1;
}

int ddlgen_codejs_task_class_proxy(const DDL_CLS* cls, const DDL_TASK* task)
{
	return 1;
}

void OutJS(int tab, const char* fmt, ...)
{
    va_list args;
	int i;
	char buf[1000];

	for(i=0; i<tab; i++) OutJS(0,  "\t");

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
		||	strcmp(arg->type, "A_UUID")==0) {
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
		||	strcmp(arg->type, "A_UUID")==0
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

int need_range(const DDL_ARG* arg)
{
	return	strcmp(arg->type, "_U8")==0
		||	strcmp(arg->type, "_U16")==0
		||	strcmp(arg->type, "_U32")==0
		||	strcmp(arg->type, "_U64")==0
		||	strcmp(arg->type, "_S8")==0
		||	strcmp(arg->type, "_S16")==0;
}

const char* range_min(const DDL_ARG* arg)
{
	if(strcmp(arg->type, "_U8")==0)		return "0";
	if(strcmp(arg->type, "_U16")==0)		return "0";
	if(strcmp(arg->type, "_U32")==0)		return "0";
	if(strcmp(arg->type, "_U64")==0)		return "0";
	if(strcmp(arg->type, "_S8")==0)		return "-127";
	if(strcmp(arg->type, "_S16")==0)		return "-32767";
	return NULL;
}

const char* range_max(const DDL_ARG* arg)
{
	if(strcmp(arg->type, "_U8")==0)		return "255";
	if(strcmp(arg->type, "_U16")==0)		return "65535";
	if(strcmp(arg->type, "_U32")==0)		return "4294967295";
	if(strcmp(arg->type, "_U64")==0)		return "4294967295";
	if(strcmp(arg->type, "_S8")==0)		return "127";
	if(strcmp(arg->type, "_S16")==0)		return "32767";
	return NULL;
}
