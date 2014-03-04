#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ddl_parser.h"
#include "ddl_codegen.h"
#include "ddl_codegen_php.h"
#include "ddl_codegen_javascript.h"

static int phpmode = 0;
static int jsmode = 0;

#ifndef _WIN32
#define stricmp strcasecmp
#endif

int main(int argc, char* argv[])
{
	char filename[300];
	char* pos;

	if(argc<2) return 0;
	if(argc>=3) {
		int i;
		for(i=2; i<argc; i++) {
			const char* r = strrchr(argv[i], '.');
			if(!r) {
				printf("invalid parameter\n");
				return -1;
			}
			if(strcmp(r, ".js")==0) {
				if(!ddlgen_codejs_open(argv[i])) {
					printf("can't open %s", argv[2]);
					return -1;
				}
				jsmode = 1;
			} else if(strcmp(r, ".php")==0) {
				if(!ddlgen_codephp_open(argv[i])) {
					printf("can't open %s", argv[2]);
					return -1;
				}
				phpmode = 1;
			} else {
				printf("invalid parameter\n");
				return -1;
			}
		}
	}

	strcpy(filename, argv[1]);
	pos = strrchr(filename, '.');
	if(!pos || stricmp(pos, ".ddl")!=0) {
		printf("invalid input file");
		return -1;
	}
	*pos = '\0';

	if(!ddlgen_parser(argv[1], filename)) {
		const char* info;
		const char* filename;
		info = ddlgen_error_info();
		filename = ddlgen_error_filename();
		if(filename) {
			printf("[%s:%d,%d] %s", filename, ddlgen_error_line(), ddlgen_error_col(), info);
		} else {
			printf("%s", info);
		}
		return -1;
	}

	if(phpmode) {
		ddlgen_codephp_close();
	}
	if(jsmode) {
		ddlgen_codejs_close();
	}

	return 0;
}

void ddlgen_notify_begin(const char* filename)
{
	if(!ddlgen_code_open(filename)) {
		ddlgen_error_set("can't open output file");
	}
}

void ddlgen_notify_end(const char* filename)
{
	ddlgen_code_close();
}

void ddlgen_notify_include(const char* filename)
{
	//printf("....%s %s", __FUNCTION__, filename);
	if(!ddlgen_code_include(filename)) {
		ddlgen_error_set("error in ddlgen_code_include");
	}
}

void ddlgen_notify_const(const DDL_CST* cst)
{
	//printf("....%s %s %s = %s", __FUNCTION__, cst->type, cst->name, cst->value);
	if(!cst->ext && !ddlgen_code_const(cst)) {
		ddlgen_error_set("error in ddlgen_code_const");
	}
}

void ddlgen_notify_struct(const DDL_STR* str)
{
	//printf("....%s %s", __FUNCTION__, str->name);
}

void ddlgen_notify_struct_arg(const DDL_STR* str, const DDL_ARG* arg)
{
	if(!get_ctype(arg, 0)) ddlgen_error_set("invalid type");
	//printf("....<%s> <%s> <%s> <%s> <%s> %d", __FUNCTION__, arg->type, arg->name, arg->size, arg->count, arg->ispointer);
}

void ddlgen_notify_struct_end(const DDL_STR* str)
{
	//printf("....%s %s %d", __FUNCTION__, str->name, str->args_count);
	if(!str->ext && !ddlgen_code_struct(str)) {
		ddlgen_error_set("error in ddlgen_code_struct");
	}

	if(jsmode) {
		ddlgen_codejs_task_struct(str, NULL);
	}
	if(phpmode) {
		ddlgen_codephp_task_struct(str, NULL);
	}
}

void ddlgen_notify_class(const DDL_CLS* cls)
{
	//printf("....%s %s", __FUNCTION__, cls->name);
}

void ddlgen_notify_class_function(const DDL_CLS* cls, const DDL_FUN* fun)
{
	//printf("....%s %s::%s", __FUNCTION__, cls->name, fun->name);
}

void ddlgen_notify_class_function_arg(const DDL_CLS* cls, const DDL_FUN* fun, const DDL_ARG* arg)
{
	if(!get_ctype(arg, 1)) ddlgen_error_set("invalid type");
	//printf("....<%s> <%s> <%s> <%s> <%s> %d", __FUNCTION__, arg->type, arg->name, arg->size, arg->count, arg->ispointer);
}

void ddlgen_notify_class_function_end(const DDL_CLS* cls, const DDL_FUN* fun)
{
	//printf("....%s %s::%s %d", __FUNCTION__, cls->name, fun->name, fun->args_count);
}

void ddlgen_notify_class_end(const DDL_CLS* cls)
{
	//printf("....%s %s %d", __FUNCTION__, cls->name, cls->funs_count);
	if(!cls->ext && !ddlgen_code_class(cls)) {
		ddlgen_error_set("error in ddlgen_code_class");
	}
}

static int code_task(const DDL_TASK* task)
{
	if(strcmp(task->type, "GEN_CLASS_STUB")==0) {
		const DDL_CLS* cls;
		cls = ddlgen_class(task->name);
		if(!cls) {
			printf("not found");
			return 0;
		}
		if(!ddlgen_code_task_class_stub(cls, task)) {
			return 0;
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_CLASS_SIGSLOT")==0) {
		const DDL_CLS* cls;
		cls = ddlgen_class(task->name);
		if(!cls) {
			printf("not found");
			return 0;
		}
		if(!ddlgen_code_task_class_stub_sigslot(cls, task)) {
			return 0;
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_CLASS_PROXY")==0) {
		const DDL_CLS* cls;
		cls = ddlgen_class(task->name);
		if(!cls) {
			printf("not found");
			return 0;
		}
		if(!ddlgen_code_task_class_prox(cls, task)) {
			return 0;
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_JS_STUB")==0) {
		if(jsmode) {
			const DDL_CLS* cls;
			cls = ddlgen_class(task->name);
			if(!cls) {
				printf("not found");
				return 0;
			}
			if(!ddlgen_codejs_task_class_stub(cls, task)) {
				return 0;
			}
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_JS_PROXY")==0) {
		if(jsmode) {
			const DDL_CLS* cls;
			cls = ddlgen_class(task->name);
			if(!cls) {
				printf("not found");
				return 0;
			}
			if(!ddlgen_codejs_task_class_proxy(cls, task)) {
				return 0;
			}
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_PHP_STUB")==0) {
		if(phpmode) {
			const DDL_CLS* cls;
			cls = ddlgen_class(task->name);
			if(!cls) {
				printf("not found");
				return 0;
			}
			if(!ddlgen_codephp_task_class_stub(cls, task)) {
				return 0;
			}
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_PHP_PROXY")==0) {
		if(phpmode) {
			const DDL_CLS* cls;
			cls = ddlgen_class(task->name);
			if(!cls) {
				printf("not found");
				return 0;
			}
			if(!ddlgen_codephp_task_class_proxy(cls, task)) {
				return 0;
			}
		}
		return 1;
	}

	/*
	if(strcmp(task->type, "GEN_CLASS_REFLECT")==0) {
		const DDL_CLS* cls;
		cls = ddlgen_class(task->name);
		if(!cls) {
			printf("not found");
			return 0;
		}
		if(!ddlgen_code_task_class_reflect(cls, task)) {
			return 0;
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_STRUCT_SERIALIZE")==0) {
		const DDL_STR* str;
		str = ddlgen_struct(task->name);
		if(!str) {
			printf("not found");
			return 0;
		}
		if(!ddlgen_code_task_struct_serialize(str, task)) {
			return 0;
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_STRUCT_REFLECT")==0) {
		const DDL_STR* str;
		str = ddlgen_struct(task->name);
		if(!str) {
			printf("not found");
			return 0;
		}
		if(!ddlgen_code_task_struct_reflect(str, task)) {
			return 0;
		}
		return 1;
	}
	*/

	if(strcmp(task->type, "GEN_JS")==0) {
		if(phpmode) {
			const DDL_STR* str;
			str = ddlgen_struct(task->name);
			if(!str) {
				printf("not found");
				return 0;
			}
			if(!ddlgen_codejs_task_struct(str, task)) {
				return 0;
			}
		}
		return 1;
	}

	if(strcmp(task->type, "GEN_PHP")==0) {
		if(phpmode) {
			const DDL_STR* str;
			str = ddlgen_struct(task->name);
			if(!str) {
				printf("not found");
				return 0;
			}
			if(!ddlgen_codephp_task_struct(str, task)) {
				return 0;
			}
		}
		return 1;
	}

	printf("unknown type\n");
	return 0;
}

void ddlgen_notify_task(const DDL_TASK* task)
{
	//printf("....%s <%s> <%s> <%s>", __FUNCTION__, task->type, task->name, task->args);
	if(!task->ext && !code_task(task)) {
		ddlgen_error_set("error in ddlgen_code_task");
	}
}
