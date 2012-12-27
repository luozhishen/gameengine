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
	OutP(0, "<?php\n");
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
	OutP(0, "class %s%s%s\n", str->name, str->parent[0]?" extends ":"", str->parent);
	OutP(0, "{\n");
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		if(arg->count[0]!='\0') {
			OutP(1, "public $%s = array(); // array %s\n", arg->name, arg->type);
		} else {
			if(is_struct(arg)) {
				OutP(1, "public $%s; // %s;\n", arg->name, arg->type);
			} else {
				OutP(1, "public $%s; // %s\n", arg->name, get_phptype(arg));
			}
		}
	}

	OutP(1, "public function __construct()\n");
	OutP(1, "{\n");
	if(str->parent[0]!='\0') {
		OutP(2, "parent::__construct();\n");
	}
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		if(arg->count[0]!='\0') {
			OutP(2, "$this->%s = array();\n", arg->name);
		} else {
			if(is_struct(arg)) {
				OutP(2, "$this->%s = new %s;\n", arg->name, arg->type);
			} else {
				if(strcmp(get_phptype(arg), "string")==0) {
					OutP(2, "$this->%s = '';\n", arg->name);
				}
				if(strcmp(get_phptype(arg), "int")==0) {
					OutP(2, "$this->%s = 0;\n", arg->name);
				}
				if(strcmp(get_phptype(arg), "float")==0) {
					OutP(2, "$this->%s = 0.0;\n", arg->name);
				}
			}
		}
	}
	OutP(1, "}\n");

	OutP(1, "public function ToStringInternal()\n");
	OutP(1, "{\n");
	if(str->parent[0]!='\0') {
		OutP(2, "$__result = parent::ToStringInternal();\n");
		if(str->args_count>0) {
		OutP(2, "if($__result!='') $__result = $__result.',';\n");
		}
	} else {
		OutP(2, "$__result = '';\n");
	}
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		OutP(2, "// %s\n", arg->name);
		if(arg->count[0]!='\0') {
			OutP(2, "if(!is_array($this->%s)) return '';\n", arg->name);
			OutP(2, "$__result = $__result.%s%s\":[';\n", a>0?"',\"":"'\"", arg->name);
			OutP(2, "for($__i=0; $__i<count($this->%s); $__i++)\n", arg->name);
			OutP(2, "{\n");
			OutP(2, "	if($__i>0) $__result = $__result.',';\n");
			if(is_struct(arg)) {
				OutP(3, "$__result = $__result.$this->%s[$__i]->ToString();\n", arg->name);
			} else {
				OutP(3, "if(!is_%s($this->%s[$__i])) return false;\n", get_phptype(arg), arg->name);
				if(strcmp(get_phptype(arg), "string")==0) {
					OutP(3, "$__result = $__result.'\"'.$this->%s[$__i].'\"';\n", arg->name);
				} else {
					OutP(3, "$__result = $__result.$this->%s[$__i];\n", arg->name);
				}
			}
			OutP(2, "}\n");
			OutP(2, "$__result = $__result.']';\n");
		} else {
			if(is_struct(arg)) {
				OutP(2, "if(!is_object($this->%s) || get_class($this->%s)!='%s') return '';\n", arg->name, arg->name, arg->type);
				OutP(2, "$__result = $__result.%s\"%s\":'.$this->%s->ToString();\n", a>0?"',":"'", arg->name, arg->name);
			} else {
				OutP(2, "if(!is_%s($this->%s)) return '';\n", get_phptype(arg), arg->name);
				if(strcmp(get_phptype(arg), "string")==0) {
					OutP(2, "$__result = $__result.%s%s\":\"'.$this->%s.'\"';\n", a>0?"',\"":"'\"", arg->name, arg->name);
				} else {
					OutP(2, "$__result = $__result.%s%s\":'.$this->%s;\n", a>0?"',\"":"'\"", arg->name, arg->name);
				}
			}
		}
	}
	OutP(1, "	return $__result;\n");
	OutP(1, "}\n");

	OutP(1, "public function ToString()\n");
	OutP(1, "{\n");
	OutP(1, "	return '{'.$this->ToStringInternal().'}';\n");
	OutP(1, "}\n");

	OutP(1, "public function ToArray($_array=array())\n");
	OutP(1, "{\n");
	if(str->parent[0]!='\0')
	{
		OutP(2, "$_array = parent::ToArray($_array);\n");
	}
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		OutP(2, "// %s\n", arg->name);
		if(arg->count[0]!='\0') {
			OutP(2, "$_earray = array();\n", arg->name);
			OutP(2, "for($__i=0; $__i<count($this->%s); $__i++)\n", arg->name);
			OutP(2, "{\n");
			if(is_struct(arg)) {
				OutP(3, "$_earray[$__i] = $this->%s[$__i]->ToArray(null);\n", arg->name);
			} else {
				OutP(3, "$_earray[$__i] = $this->%s[$__i];\n", arg->name);
			}
			OutP(2, "}\n");
			OutP(2, "$_array['%s'] = $_earray;\n", arg->name);
		} else {
			if(is_struct(arg)) {
				OutP(2, "$_array['%s'] = $this->%s->ToArray(null);\n", arg->name, arg->name);
			} else {
				OutP(2, "$_array['%s'] = $this->%s;\n", arg->name, arg->name);
			}
		}
	}
	OutP(1, "	return $_array;\n");
	OutP(1, "}\n");

	OutP(1, "public function FromArray($_array)\n");
	OutP(1, "{\n");
	OutP(1, "	if(!is_array($_array)) return false;\n");
	if(str->parent[0]!='\0')
	{
		OutP(2, "if(!parent::FromArray($_array)) return false;\n");
	}
	for(a=0; a<str->args_count; a++) {
		DDL_ARG* arg = &str->args[a];
		OutP(2, "// %s\n", arg->name);
		if(arg->count[0]!='\0') {
			OutP(2, "if(!is_array($_array['%s'])) return false;\n", arg->name);

			OutP(2, "$_earray = $_array['%s'];\n", arg->name);
			OutP(2, "$this->%s = array();\n", arg->name);
			OutP(2, "for($__i=0; $__i<count($_earray); $__i++)\n");
			OutP(2, "{\n");
			if(is_struct(arg)) {
				OutP(3, "if(!$this->%s[$__i]->FromArray($_earray[$__i])) return false;\n", arg->name);
			} else {
				OutP(3, "if(!is_%s($_earray[$__i])) return false;\n", get_phptype(arg));
				OutP(3, "$this->%s[$__i] = $_earray[$__i];\n", arg->name);
			}
			OutP(2, "}\n");
		} else {
			if(is_struct(arg)) {
				OutP(2, "if(!$this->%s->FromArray($_array['%s'])) return false;\n", arg->name, arg->name);
			} else {
				OutP(2, "if(!is_%s($_array['%s'])) return false;\n", get_phptype(arg), arg->name);
				OutP(2, "$this->%s = $_array['%s'];\n", arg->name, arg->name);
			}
		}
	}
	OutP(1, "	return true;\n");
	OutP(1, "}\n");

	OutP(0, "};\n");
	return 1;
}

int ddlgen_codephp_task_class_stub(const DDL_CLS* cls, const DDL_TASK* task)
{
	int f, a;
	OutP(0, "\n");
	OutP(0, "function %s_Dispatcher($fname, $_array, $_this)\n", cls->name);
	OutP(0, "{\n");
	for(f=0; f<(int)cls->funs_count; f++)
	{
		DDL_FUN* fun = &cls->funs[f];
		OutP(1, "if($fname=='%s')\n", fun->name);
		OutP(1, "{\n");
		for(a=0; a<(int)fun->args_count; a++)
		{
			DDL_ARG* arg = &fun->args[a];

			if(arg->count[0]!='\0') {
				OutP(2, "if(!is_array($_array['%s'])) return false;\n", arg->name);
				OutP(2, "$__%s = array();\n", arg->name);
				OutP(2, "$_earray = $_array['%s'];\n", arg->name);
				OutP(2, "for($__i=0; $__i<count($_earray); $__i++)\n");
				OutP(2, "{\n");
				if(is_struct(arg)) {
					OutP(3, "if(!is_array($_earray[$__i])) return false;\n");
					OutP(3, "if(!$__%s[$__i]->FromArray($_earray[$__i])) return false;\n", arg->name);
				} else {
					OutP(3, "if(!is_%s($_earray[$__i])) return false;\n", get_phptype(arg));
					OutP(3, "$__%s[$__i] = $_earray[$__i];\n", arg->name);
				}
				OutP(2, "}\n");
			} else {
				if(is_struct(arg)) {
					OutP(2, "if(!is_array($_array['%s'])) return false;\n", arg->name);
					OutP(2, "$__%s = new %s;\n", arg->name, arg->type);
					OutP(2, "if(!$__%s->FromArray($_array[%s]) return false;\n", arg->name, arg->name);
				} else {
					OutP(2, "if(!is_%s($_array['%s'])) return false;\n", get_phptype(arg), arg->name);
					OutP(2, "$__%s = $_array['%s'];\n", arg->name, arg->name);
				}
			}
		}

		OutP(1, "	$_this->%s(", fun->name);
		for(a=0; a<(int)fun->args_count; a++)
		{
			if(a>0) OutP(0, ", ");
			OutP(0, "$__%s", fun->args[a].name);
		}
		OutP(0, ");\n");
		OutP(1, "	return true;\n");
		OutP(1, "}\n");
	}
	OutP(0, "	return false;\n");
	OutP(0, "}\n");
	return 1;
}

int ddlgen_codephp_task_class_proxy(const DDL_CLS* cls, const DDL_TASK* task)
{
	int f, a;
	OutP(0, "\n");
	OutP(0, "class %s\n", cls->name);
	OutP(0, "{\n");
	for(f=0; f<(int)cls->funs_count; f++)
	{
		DDL_FUN* fun = &cls->funs[f];
		OutP(1, "public function %s(", fun->name);
		for(a=0; a<(int)fun->args_count; a++)
		{
			DDL_ARG* arg = &fun->args[a];
			if(a>0) OutP(0, ", ");
			OutP(0, "$%s", arg->name);
		}
		OutP(0, ")\n");
		OutP(1, "{\n");
		if(fun->args_count==0) {
			OutP(1, "	return '{\"method\":\"%s.%s\",\"message\":{}}';\n", cls->name, fun->name);
			OutP(1, "}\n");
			continue;
		}

		for(a=0; a<(int)fun->args_count; a++)
		{
			DDL_ARG* arg = &fun->args[a];
			if(arg->count[0]!='\0') {
				OutP(2, "if(!is_array($%s)) return '';\n", arg->name);
				OutP(2, "$__result = %s'%s\"%s\":[';\n", a>0?"$__result.":"", a>0?",":"", arg->name);
				OutP(2, "for($__i=0; $__i<count($%s); $__i++)\n", arg->name);
				OutP(2, "{\n");
				OutP(2, "	if($__i>0) $__result = $__result.',';\n");
				if(is_struct(arg)) {
					OutP(3, "$__result = $__result.$%s[$__i]->ToString();\n", arg->name);
				} else {
					OutP(3, "if(!is_%s($%s[$__i])) return false;\n", get_phptype(arg), arg->name);
					if(strcmp(get_phptype(arg), "string")==0) {
						OutP(3, "$__result = $__result.'\"'.$%s.'\"';\n", arg->name);
					} else {
						OutP(3, "$__result = $__result.$%s;\n", arg->name);
					}
				}
				OutP(2, "}\n");
				OutP(2, "$__result = $__result.']';\n");
			} else {
				if(is_struct(arg)) {
					OutP(2, "if(!is_object($%s) || get_class($%s)!='%s') return '';\n", arg->name, arg->name, arg->type);
					OutP(2, "$__result = %s%s'\"%s\":'.$%s->ToString();\n", a>0?"$__result.":"", a>0?",":"", arg->name, arg->name);
				} else {
					OutP(2, "if(!is_%s($%s)) return '';\n", get_phptype(arg), arg->name);
					if(strcmp(get_phptype(arg), "string")==0) {
						OutP(2, "$__result = %s%s'\"%s\":\"'.$%s.'\"';\n", a>0?"$__result.":"", a>0?",":"", arg->name, arg->name);
					} else {
						OutP(2, "$__result = %s'%s\"%s\":'.$%s;\n", a>0?"$__result.":"", a>0?",":"", arg->name, arg->name);
					}
				}
			}
		}
		OutP(1, "	return '{\"method\":\"%s.%s\",\"message\":{'.$__result.'}}';\n", cls->name, fun->name);
		OutP(1, "}\n");
	}
	OutP(0, "}\n");
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
