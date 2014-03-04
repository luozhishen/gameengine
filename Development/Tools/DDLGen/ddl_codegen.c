#include "ddl_parser.h"
#include "ddl_codegen.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#ifndef _WIN32
#define stricmp strcasecmp
#endif

static FILE* _H;
static FILE* _C;

int has_param(DDL_ARG* arg, const char* name)
{
	unsigned int i;
	for(i=0; i<arg->params_count; i++)
	{
		if(stricmp(arg->params[i].name, name)==0) return 1;
	}
	return 0;
}

void OutH(int tab, const char* fmt, ...)
{
    va_list args;
	int i;
	char buf[1000];

	for(i=0; i<tab; i++) OutH(0,  "\t");

    va_start(args, fmt);
    vsprintf(buf, fmt, args);

	fprintf(_H,  "%s", buf);
}

void OutC(int tab, const char* fmt, ...)
{
    va_list args;
	int i;
	char buf[1000];

	for(i=0; i<tab; i++) OutC(0,  "\t");

    va_start(args, fmt);
    vsprintf(buf, fmt, args);

	fprintf(_C,  "%s", buf);
}

struct { char* name; char* rtype; } basetypes[] = {
	{ "_U8",	"TYPE_U8"},
	{ "_U16",	"TYPE_U16"},
	{ "_U32",	"TYPE_U32"},
	{ "_U64",	"TYPE_U64"},
	{ "_S8",	"TYPE_S8"},
	{ "_S16",	"TYPE_S16"},
	{ "_S32",	"TYPE_S32"},
	{ "_S64",	"TYPE_S64"},
	{ "_F32",	"TYPE_F32"},
	{ "_F64",	"TYPE_F64"},
};

const char* get_ctype(const DDL_ARG* arg, int is_class)
{
	static char ret[400];

	if(strcmp(arg->type, "string")==0) {
		if(is_class) {
			if(arg->count[0]) {						// array<string<>,>; array<string<>>
				sprintf(ret, "DDL::String<%s>*", arg->size);
				return ret;
			} else {								// string<>; string
				sprintf(ret, "char*");
				return ret;
			}
		} else {
			if(arg->count[0] && arg->size[0]) {		// array<string<>,>
				sprintf(ret, "DDL::Array<DDL::String<%s>,%s>", arg->size, arg->count);
				return ret;
			}
			if(arg->size[0] && !arg->count[0]) {	// string<>
				sprintf(ret, "DDL::String<%s>", arg->size);
				return ret;
			}
			return NULL;
		}
	}

	if(strcmp(arg->type, "content_ref")==0) {
		if(!arg->count[0]) {
			sprintf(ret, "DDL::Pointer<%s>", arg->size);
		} else {
			sprintf(ret, "DDL::Array<DDL::Pointer<%s>, %s>", arg->size, arg->count);
		}
		return ret;
	}

	if(is_class) {
		if(arg->count[0]) {
			sprintf(ret, "%s*", arg->type);
			return ret;
		} else {
			return arg->type;
		}
	} else {
		if(arg->count[0]) {
			sprintf(ret, "DDL::Array<%s, %s>", arg->type, arg->count);
			return ret;
		} else {
			return arg->type;
		}
	}
}

int is_pointer(const DDL_ARG* arg)
{
	if(strcmp(arg->type, "string")==0) return 1;
	if(arg->count[0]) return 1;
	return 0;
}

int is_basetype(const DDL_ARG* arg)
{
	int i;
	for(i=0; i<sizeof(basetypes)/sizeof(basetypes[0]); i++)
	{
		if(strcmp(arg->type, basetypes[i].name)==0) return 1;
	}
	return 0;
}

const char* get_basetype_rtype(const DDL_ARG* arg)
{
	int i;
	for(i=0; i<sizeof(basetypes)/sizeof(basetypes[0]); i++)
	{
		if(strcmp(arg->type, basetypes[i].name)==0) return basetypes[i].rtype;
	}
	return NULL;
}

void get_arg_reflecttype(const DDL_ARG* arg, char* type)
{
	if(strcmp(arg->type, "content_ref")==0) strcpy(type, "TYPE_UUID_REF");
	else if(strcmp(arg->type, "string")==0) strcpy(type, "TYPE_STRING");
	else if(strcmp(arg->type, "A_UUID")==0) strcpy(type, "TYPE_UUID");
	else if(ddlgen_struct(arg->type)!=0) strcpy(type, "TYPE_STRUCT");
	else if(is_basetype(arg)) strcpy(type, get_basetype_rtype(arg));
	else strcpy(type, "");
	if(arg->count[0]!='\0') strcat(type, "|TYPE_ARRAY");
}

void get_arg_structinfo(const DDL_ARG* arg, char* sinfo)
{
	if(ddlgen_struct(arg->type)) {
		sprintf(sinfo, "&_rfl_struct_%s_info", arg->type);
	} else {
		strcpy(sinfo, "NULL");
	}
}

int ddlgen_code_open(const char* filename)
{
	char sz[400];
	char* p1;
	char* p2;
	int i;

	char fhpp[400];
	char fcpp[400];
	sprintf(fhpp, "%s.h",   filename);
	sprintf(fcpp, "%s.cpp", filename);
	_H = fopen(fhpp, "wt");
	_C = fopen(fcpp, "wt");
	if(!_H || !_C) {
		if(_H) fclose(_H);
		if(_C) fclose(_C);
		printf("[%s]\n", filename);
		return 0;
	}

	OutH(0, "////////////////////////////////////////////////\n");
	OutH(0, "// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //\n");
	OutH(0, "////////////////////////////////////////////////\n");
	OutH(0, "\n");
	OutC(0, "////////////////////////////////////////////////\n");
	OutC(0, "// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //\n");
	OutC(0, "////////////////////////////////////////////////\n");
	OutC(0, "\n");
	OutC(0, "#include <ZionBase.h>\n"); 
	{
		char* p1 = (char*)strrchr(filename, '/');
		char* p2 = (char*)strrchr(filename, '\\');
		if(p2>p1) p1 = p2;
		if(!p1) p1 = (char*)filename;
		else p1 += 1;
		OutC(0, "#include \"%s.h\"\n", p1); 
	}
	OutC(0, "\n");

	strcpy(sz, filename);
	p1 = strrchr(sz, '\\');
	p2 = strrchr(sz, '/');
	if(p2>p1) p1 = p2;
	if(!p1) p1 = sz; else p1 += 1;
	for(i=(int)strlen(p1)-1; i>=0; i--) {
		if(p1[i]>='a' && p1[i]<='z') {
			p1[i] = p1[i] - 'a' + 'A';
		}
		if(p1[i]=='.') {
			p1[i] = '_';
		}
	}


	OutH(0, "#ifndef __%s_DDL_CODEGEN__\n", p1);
	OutH(0, "#define __%s_DDL_CODEGEN__\n", p1);
	OutH(0, "\n");
	OutH(0, "#pragma pack(push)\n");
	OutH(0, "#pragma pack(4)\n");
	OutH(0, "\n");

	return 1;
}

void ddlgen_code_close()
{

	OutH(0, "\n");
	OutH(0, "#pragma pack(pop)\n");
	OutH(0, "\n");
	OutH(0, "#endif\n");
	fclose(_H);
}

int ddlgen_code_include(const char* filename)
{
	char fn[400];
	char* p;
	strcpy(fn, filename);
	p = strrchr(fn, '.');
	if(p) {
		if(stricmp(p+1, "ddl")!=0) return 0;
		*p = '\0';
		OutH(0, "#include \"%s.h\"\n", fn);
	} else {
		OutH(0, "#include \"%s\"\n", filename);
	}
	OutH(0, "\n");
	return 1;
}

int ddlgen_code_const(const DDL_CST* cst)
{
	OutH(0, "const %s %s = %s;\n", cst->type, cst->name, cst->value);
	OutH(0, "\n");
	return 1;
}

int ddlgen_code_struct(const DDL_STR* str)
{
	unsigned int a;
	OutH(0, "struct %s%s%s\n", str->name, str->parent[0]?" : ":"", str->parent);
	OutH(0, "{\n");
	for(a=0; a<str->args_count; a++) {
		OutH(1, "%s %s;\n", get_ctype(&str->args[a], 0), str->args[a].name);
	}
	OutH(0, "};\n");
	OutH(0, "\n");

	ddlgen_code_task_struct_serialize(str, NULL);
	ddlgen_code_task_struct_reflect(str, NULL);
	return 1;
}

int ddlgen_code_class(const DDL_CLS* cls)
{
	OutH(0, "class %s;\n", cls->name);
	OutH(0, "\n");
	ddlgen_code_task_class_reflect(cls, NULL);
	return 1;
}

int ddlgen_code_task_class_stub(const DDL_CLS* cls, const DDL_TASK* task)
{
	unsigned int f, a;

	OutH(0, "namespace DDLStub\n");
	OutH(0, "{\n");
	OutH(0, "\n");
	OutH(0, "	template<typename CLASS>\n");
	OutH(0, "	class %s : public DDLStub<CLASS>\n", cls->name);
	OutH(0, "	{\n");
	OutH(0, "	public:\n");
	OutH(0, "		%s(CLASS* Class) : DDLStub<CLASS>(Class)\n", cls->name);
	OutH(0, "		{\n");
	OutH(0, "		}\n");
	OutH(0, "\n");
	OutH(0, "		virtual const DDLReflect::CLASS_INFO* GetClassInfo()\n");
	OutH(0, "		{\n");
	OutH(0, "			return DDLReflect::GetClass<::%s>();\n", cls->name);
	OutH(0, "		}\n");
	OutH(0, "		\n");
	OutH(0, "		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)\n");
	OutH(0, "		{\n");
	for(f=0; f<cls->funs_count; f++) {
		OutH(1, "		if(fid==%d)\n", f);
		OutH(1, "		{\n");
		for(a=0; a<cls->funs[f].args_count; a++) {
			if(is_pointer(&cls->funs[f].args[a])) {
				OutH(3, "	_U32 __length;\n");
				break;
			}
		}
		for(a=0; a<cls->funs[f].args_count; a++) {
			OutH(2, "		%s _prefix_%s;\n", get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
		}
		if(cls->funs[f].args_count>0) OutH(0, "\n");
		for(a=0; a<cls->funs[f].args_count; a++) {
			OutH(2, "		// <%s> <%s> <%s> <%s>;\n", cls->funs[f].args[a].type, cls->funs[f].args[a].name, cls->funs[f].args[a].size, cls->funs[f].args[a].count);
			if(strcmp(cls->funs[f].args[a].type, "string")==0) {
				if(!cls->funs[f].args[a].size[0] && !cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.Read(__length)) return false;\n");
					OutH(3, "	_prefix_%s = (%s)alloca(sizeof(_prefix_%s[0])*(__length+1));\n", cls->funs[f].args[a].name, get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
					OutH(3, "	if(!_prefix_%s) return false;\n", cls->funs[f].args[a].name);
					OutH(3, "	if(!Buf.ReadBuffer(_prefix_%s, (unsigned int)sizeof(_prefix_%s[0])*__length)) return false;\n", cls->funs[f].args[a].name, cls->funs[f].args[a].name);
					OutH(3, "	_prefix_%s[__length] = '\\0';\n", cls->funs[f].args[a].name);
					continue;
				}
				if(cls->funs[f].args[a].size[0] && !cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.ReadString(_prefix_%s)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
				if(cls->funs[f].args[a].size[0] && cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.Read(__length)) return false;\n");
					OutH(3, "	_prefix_%s = (%s)alloca(sizeof(_prefix_%s[0])*__length);\n", cls->funs[f].args[a].name, get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
					OutH(3, "	if(!_prefix_%s) return false;\n", cls->funs[f].args[a].name);
					OutH(3, "	if(!Buf.ReadStringPointer(_prefix_%s, __length)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
			} else {
				if(cls->funs[f].args[a].count[0]=='\0') {
					OutH(4, "if(!Buf.Read(_prefix_%s)) return false;\n", cls->funs[f].args[a].name);
					continue;
				} else {
					OutH(4, "if(!Buf.Read(__length)) return false;\n");
					OutH(4, "_prefix_%s = (%s)alloca(sizeof(_prefix_%s[0])*__length);\n", cls->funs[f].args[a].name, get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
					OutH(4, "if(!_prefix_%s) return false;\n", cls->funs[f].args[a].name);
					OutH(4, "if(!Buf.ReadPointer(_prefix_%s, __length)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
			}
			OutH(2, "		bu ke neng!!!!!!\n");
		}
		if(cls->funs[f].args_count>0) OutH(0, "\n");
		OutH(1, "			// call implement\n");
		OutH(1, "			DDLStub<CLASS>::GetClass()->%s(", cls->funs[f].name);
		for(a=0; a<cls->funs[f].args_count; a++) {
			if(a>0) OutH(0,  ", ");
			OutH(0,  "_prefix_%s", cls->funs[f].args[a].name);
		}
		OutH(0, ");\n");
		OutH(1, "			return true;\n");
		OutH(1, "		}\n");
	}
	OutH(0, "			return false;\n");
	OutH(0, "		}\n");
	OutH(0, "	};\n");
	OutH(0, "\n");
	OutH(0, "}\n");
	OutH(0, "\n");
	return 1;
}

int ddlgen_code_task_class_stub_sigslot(const DDL_CLS* cls, const DDL_TASK* task)
{
	unsigned int f, a;

	OutH(0, "namespace DDLSigSlot\n");
	OutH(0, "{\n");
	OutH(0, "\n");
	OutH(0, "	class %s : public DDLStub::IStub\n", cls->name);
	OutH(0, "	{\n");
	OutH(0, "	public:\n");
	OutH(0, "		virtual const DDLReflect::CLASS_INFO* GetClassInfo()\n");
	OutH(0, "		{\n");
	OutH(0, "			return DDLReflect::GetClass<::%s>();\n", cls->name);
	OutH(0, "		}\n");
	OutH(0, "		\n");
	OutH(0, "		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)\n");
	OutH(0, "		{\n");
	for(f=0; f<cls->funs_count; f++) {
		OutH(1, "		if(fid==%d)\n", f);
		OutH(1, "		{\n");
		for(a=0; a<cls->funs[f].args_count; a++) {
			if(is_pointer(&cls->funs[f].args[a])) {
				OutH(3, "	_U32 __length;\n");
				break;
			}
		}
		for(a=0; a<cls->funs[f].args_count; a++) {
			OutH(2, "		%s _prefix_%s;\n", get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
		}
		if(cls->funs[f].args_count>0) OutH(0, "\n");
		for(a=0; a<cls->funs[f].args_count; a++) {
			OutH(2, "		// <%s> <%s> <%s> <%s>;\n", cls->funs[f].args[a].type, cls->funs[f].args[a].name, cls->funs[f].args[a].size, cls->funs[f].args[a].count);
			if(strcmp(cls->funs[f].args[a].type, "string")==0) {
				if(!cls->funs[f].args[a].size[0] && !cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.Read(__length)) return false;\n");
					OutH(3, "	_prefix_%s = (%s)alloca(sizeof(_prefix_%s[0])*(__length+1));\n", cls->funs[f].args[a].name, get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
					OutH(3, "	if(!_prefix_%s) return false;\n", cls->funs[f].args[a].name);
					OutH(3, "	if(!Buf.ReadBuffer(_prefix_%s, (unsigned int)sizeof(_prefix_%s[0])*__length)) return false;\n", cls->funs[f].args[a].name, cls->funs[f].args[a].name);
					OutH(3, "	_prefix_%s[__length] = '\\0';\n", cls->funs[f].args[a].name);
					continue;
				}
				if(cls->funs[f].args[a].size[0] && !cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.ReadString(_prefix_%s)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
				if(cls->funs[f].args[a].size[0] && cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.Read(__length)) return false;\n");
					OutH(3, "	_prefix_%s = (%s)alloca(sizeof(_prefix_%s[0])*__length);\n", cls->funs[f].args[a].name, get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
					OutH(3, "	if(!_prefix_%s) return false;\n", cls->funs[f].args[a].name);
					OutH(3, "	if(!Buf.ReadStringPointer(_prefix_%s, __length)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
			} else {
				if(cls->funs[f].args[a].count[0]=='\0') {
					OutH(4, "if(!Buf.Read(_prefix_%s)) return false;\n", cls->funs[f].args[a].name);
					continue;
				} else {
					OutH(4, "if(!Buf.Read(__length)) return false;\n");
					OutH(4, "_prefix_%s = (%s)alloca(sizeof(_prefix_%s[0])*__length);\n", cls->funs[f].args[a].name, get_ctype(&cls->funs[f].args[a], 1), cls->funs[f].args[a].name);
					OutH(4, "if(!_prefix_%s) return false;\n", cls->funs[f].args[a].name);
					OutH(4, "if(!Buf.ReadPointer(_prefix_%s, __length)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
			}
			OutH(2, "		bu ke neng!!!!!!\n");
		}
		if(cls->funs[f].args_count>0) OutH(0, "\n");
		OutH(1, "			// call implement\n");
		OutH(1, "			_%s(", cls->funs[f].name);
		for(a=0; a<cls->funs[f].args_count; a++) {
			if(a>0) OutH(0,  ", ");
			OutH(0,  "_prefix_%s", cls->funs[f].args[a].name);
		}
		OutH(0, ");\n");
		OutH(1, "			return true;\n");
		OutH(1, "		}\n");
	}
	OutH(0, "			return false;\n");
	OutH(0, "		}\n");
	OutH(0, "\n");
	for(f=0; f<cls->funs_count; f++) {
		OutH(2, "sigslot::signal%d<", cls->funs[f].args_count);
		for(a=0; a<cls->funs[f].args_count; a++) {
			if(a>0) OutH(0, ", ");
			OutH(0, "%s", get_ctype(&cls->funs[f].args[a], 1));
		}
		OutH(0, "> _%s;\n", cls->funs[f].name);
	}
	OutH(0, "	};\n");
	OutH(0, "\n");
	OutH(0, "}\n");
	OutH(0, "\n");
	return 1;
}

int ddlgen_code_task_class_prox(const DDL_CLS* cls, const DDL_TASK* task)
{
	unsigned int f, a;

	OutH(0, "namespace DDLProxy\n");
	OutH(0, "{\n");
	OutH(0, "\n");
	OutH(0, "	template<typename CLIENT, typename BUFFER>\n");
	OutH(0, "	class %s : public DDLProxy<CLIENT, BUFFER>\n", cls->name);
	OutH(0, "	{\n");
	OutH(0, "	public:\n");
	OutH(0, "		%s(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::%s>())\n", cls->name, cls->name);
	OutH(0, "		{\n");
	OutH(0, "		}\n");
	OutH(0, "\n");
	OutH(0, "		static %s<CLIENT, BUFFER> Get(CLIENT* Client)\n", cls->name);
	OutH(0, "		{\n");
	OutH(0, "			%s<CLIENT, BUFFER> Proxy(Client);\n", cls->name);
	OutH(0, "			return Proxy;\n");
	OutH(0, "		}\n");
	for(f=0; f<cls->funs_count; f++) {
		int need_length = 0;
		OutH(0, "\n");
		OutH(1, "	bool %s(", cls->funs[f].name);
		for(a=0; a<cls->funs[f].args_count; a++) {
			if(a>0) OutH(0,  ", ");
			OutH(0, "%s%s%s %s",
				is_basetype(&cls->funs[f].args[a]) && !is_pointer(&cls->funs[f].args[a])?"":"const ",
				get_ctype(&cls->funs[f].args[a], 1),
				(!is_basetype(&cls->funs[f].args[a]) && !is_pointer(&cls->funs[f].args[a]))?"&":"",
				cls->funs[f].args[a].name
				);
			if(is_pointer(&cls->funs[f].args[a])) {
				need_length = 1;
			}
		}
		OutH(0, ")\n");
		OutH(1, "	{\n");
		OutH(1, "		BUFFER Buf;\n");
		if(need_length) {
			OutH(2, "	_U32 __length;\n");
		}
		for(a=0; a<cls->funs[f].args_count; a++) {
			OutH(2, "	// <%s> <%s> <%s> <%s>\n", cls->funs[f].args[a].type, cls->funs[f].args[a].name, cls->funs[f].args[a].size, cls->funs[f].args[a].count);
			if(strcmp(cls->funs[f].args[a].type, "string")==0) {
				if(!cls->funs[f].args[a].size[0] && !cls->funs[f].args[a].count[0]) {
					OutH(3, "__length = DDL::StringLength(%s);\n", cls->funs[f].args[a].name);
					OutH(3, "if(!Buf.Write(__length)) return false;\n");
					OutH(3, "if(!Buf.WriteData(%s, (unsigned int)sizeof(%s[0])*__length)) return false;\n", cls->funs[f].args[a].name, cls->funs[f].args[a].name);
					continue;
				}
				if(cls->funs[f].args[a].size[0] && !cls->funs[f].args[a].count[0]) {
					OutH(3, "if(!Buf.WriteString(%s)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
				if(cls->funs[f].args[a].size[0] && cls->funs[f].args[a].count[0]) {
					OutH(3, "	if(!Buf.WriteStringPointer(%s, (_U16)(%s))) return false;\n", cls->funs[f].args[a].name, cls->funs[f].args[a].count);
					continue;
				}
			} else {
				if(cls->funs[f].args[a].count[0]=='\0') {
					OutH(3, "if(!Buf.Write(%s)) return false;\n", cls->funs[f].args[a].name);
					continue;
				} else {
					OutH(3, "__length = (_U16)(%s);\n", cls->funs[f].args[a].count);
					OutH(3, "if(!Buf.Write(__length)) return false;\n");
					OutH(3, "if(!Buf.WritePointer(%s, __length)) return false;\n", cls->funs[f].args[a].name);
					continue;
				}
			}
			OutH(2, "	bu ke neng!!!!!!\n");
		}
		OutH(0, "\n");
		OutH(1, "		// send\n");
		OutH(1, "		return this->GetClient()->Send(this->GetClassID(), %d, Buf);\n", f);
		OutH(1, "	}\n");
	}
	OutH(0, "	};\n");
	OutH(0, "\n");
	OutH(0, "}\n");
	OutH(0, "\n");
	return 1;
}

int ddlgen_code_task_class_reflect(const DDL_CLS* cls, const DDL_TASK* task)
{
	unsigned int a, f;
	int fidx[400];

	OutH(0, "namespace DDLReflect\n");
	OutH(0, "{\n");
	OutH(0, "	template<>\n");
	OutH(0, "	const CLASS_INFO* GetClass<%s>();\n", cls->name);
	OutH(0, "}\n");
	OutH(0, "\n");

	fidx[0] = 0;
	OutC(0, "namespace DDLReflect\n");
	OutC(0, "{\n");
	OutC(0, "	static FIELD_INFO _class_%s_fieldinfos[] = \n", cls->name);
	OutC(0, "	{\n");
	for(f=0; f<cls->funs_count; f++)
	{
	OutC(0, "		// %d %s\n", f, cls->funs[f].name);
	for(a=0; a<cls->funs[f].args_count; a++)
	{
		char type[400];
		char sinfo[400];
		get_arg_reflecttype(&cls->funs[f].args[a], type);
		get_arg_structinfo(&cls->funs[f].args[a], sinfo);
		OutC(1, "	{%s, \"%s\", 0, 0, %s, (_U16)%s,(_U16) %s, 0, 0, NULL},\n", type, cls->funs[f].args[a].name, sinfo,
			cls->funs[f].args[a].size[0]=='\0'?"-1":cls->funs[f].args[a].size,
			"-1"
		);
	}
	fidx[f+1] = fidx[f] + cls->funs[f].args_count;
	}
	if(fidx[cls->funs_count]==0)
	{
		OutC(1, "	{0, NULL, 0, NULL, 0, 0, 0, 0},\n");
	}
	OutC(0, "	};\n");
	OutC(0, "	static FUNCTION_INFO _class_%s_funcinfos[] = \n", cls->name);
	OutC(0, "	{\n");
	for(f=0; f<cls->funs_count; f++)
	{
		OutC(1, "	{\"%s\", %d, _class_%s_fieldinfos+%d},\n", cls->funs[f].name, cls->funs[f].args_count, cls->name, fidx[f]);
	}
	if(cls->funs_count==0)
	{
		OutC(1, "	{NULL, 0, NULL},\n");
	}
	OutC(0, "	};\n");
	OutC(0, "	static CLASS_INFO _class_%s_info = { %s, \"%s\", %d, _class_%s_funcinfos };\n", cls->name, ddlgen_get_cparam(cls, "iid", "0"), cls->name, cls->funs_count, cls->name);
	OutC(0, "	template<>\n");
	OutC(0, "	const CLASS_INFO* GetClass<%s>()\n", cls->name);
	OutC(0, "	{\n");
	OutC(0, "		return &_class_%s_info;\n", cls->name);
	OutC(0, "	}\n");
	OutC(0, "}\n");
	OutC(0, "\n");

	return 1;
}

int ddlgen_code_task_struct_serialize(const DDL_STR* str, const DDL_TASK* task)
{
	unsigned int a;

	OutH(0, "namespace DDL\n");
	OutH(0, "{\n");
	OutH(0, "	template<>\n");
	OutH(0, "	bool BufferReader::Read<%s>(%s& Value);\n", str->name, str->name);
	OutH(0, "	template<>\n");
	OutH(0, "	bool BufferWriter::Write<%s>(const %s& Value);\n", str->name, str->name);
	OutH(0, "}\n");
	OutH(0, "\n");

	OutC(0, "namespace DDL\n");
	OutC(0, "{\n");
	OutC(0, "	template<>\n");
	OutC(0, "	bool BufferReader::Read<%s>(%s& Value)\n", str->name, str->name);
	OutC(0, "	{\n");
	if(str->parent[0]) {
		OutC(1, "	if(!BufferReader::Read<%s>(Value)) return false;\n", str->parent);
	}
	for(a=0; a<str->args_count; a++) {
		if(strcmp(str->args[a].type, "content_ref")==0) {
			if(!str->args[a].count[0]) {
			OutC(2, "if(!Read<A_UUID>(Value.%s.uuid)) return false;\n", str->args[a].name);
			} else {
			OutC(2, "if(!ReadArray<A_UUID, %s>(*((DDL::Array<A_UUID, %s>*)&Value.%s))) return false;\n", str->args[a].count, str->args[a].count, str->args[a].name);
			}
		} else if(strcmp(str->args[a].type, "string")==0) {
			if(!str->args[a].count[0]) {
			OutC(2, "if(!ReadString<%s>(Value.%s)) return false;\n", str->args[a].size, str->args[a].name);
			} else {
			OutC(2, "if(!ReadStringArray<%s, %s>(Value.%s)) return false;\n", str->args[a].size, str->args[a].count, str->args[a].name);
			}
		} else {
			if(!str->args[a].count[0]) {
			OutC(2, "if(!Read<%s>(Value.%s)) return false;\n", str->args[a].type, str->args[a].name);
			} else {
			OutC(2, "if(!ReadArray<%s, %s>(Value.%s)) return false;\n", str->args[a].type, str->args[a].count, str->args[a].name);
			}
		}
	}
	OutC(0, "		return true;\n");
	OutC(0, "	}\n");
	OutC(0, "	template<>\n");
	OutC(0, "	bool BufferWriter::Write<%s>(const %s& Value)\n", str->name, str->name);
	OutC(0, "	{\n");
	if(str->parent[0]) {
		OutC(1, "	if(!BufferWriter::Write<%s>(Value)) return false;\n", str->parent);
	}
	for(a=0; a<str->args_count; a++) {
		if(strcmp(str->args[a].type, "content_ref")==0) {
			if(!str->args[a].count[0]) {
			OutC(2, "if(!Write<A_UUID>(Value.%s.uuid)) return false;\n", str->args[a].name);
			} else {
			OutC(2, "if(!WriteArray<A_UUID, %s>(*((const DDL::Array<A_UUID, %s>*)&Value.%s))) return false;\n", str->args[a].count, str->args[a].count, str->args[a].name);
			}
		} else if(strcmp(str->args[a].type, "string")==0) {
			if(!str->args[a].count[0]) {
			OutC(2, "if(!WriteString<%s>(Value.%s)) return false;\n", str->args[a].size, str->args[a].name);
			} else {
			OutC(2, "if(!WriteStringArray<%s, %s>(Value.%s)) return false;\n", str->args[a].size, str->args[a].count, str->args[a].name);
			}
		} else {
			if(!str->args[a].count[0]) {
			OutC(2, "if(!Write<%s>(Value.%s)) return false;\n", str->args[a].type, str->args[a].name);
			} else {
			OutC(2, "if(!WriteArray<%s, %s>(Value.%s)) return false;\n", str->args[a].type, str->args[a].count, str->args[a].name);
			}
		}
	}
	OutC(0, "		return true;\n");
	OutC(0, "	}\n");
	OutC(0, "}\n");
	OutC(0, "\n");

	return 1;
}

int ddlgen_code_task_struct_reflect(const DDL_STR* str, const DDL_TASK* task)
{
	unsigned int a;
	char ss[400] = "NULL";

	if(str->parent[0]) sprintf(ss, "&_rfl_struct_%s_info", str->parent);

	OutH(0, "namespace DDLReflect\n");
	OutH(0, "{\n");
	OutH(0, "	template<>\n");
	OutH(0, "	const STRUCT_INFO* GetStruct<%s>();\n", str->name);
	OutH(0, "	extern STRUCT_INFO _rfl_struct_%s_info;\n", str->name, str->name);
	OutH(0, "}\n");
	OutH(0, "\n");
	OutC(0, "namespace DDLReflect\n");
	OutC(0, "{\n");

	OutC(0, "\n");
	OutC(0, "	static bool _struct_%s_readproc(DDL::BufferReader& buf, void* data)\n", str->name);
	OutC(0, "	{\n");
	OutC(0, "		return buf.Read<%s>(*((%s*)data));\n", str->name, str->name);
	OutC(0, "	}\n");
	OutC(0, "\n");
	OutC(0, "	static bool _struct_%s_writeproc(DDL::BufferWriter& buf, const void* data)\n", str->name);
	OutC(0, "	{\n");
	OutC(0, "		return buf.Write<%s>(*((const %s*)data));\n", str->name, str->name);
	OutC(0, "	}\n");
	OutC(0, "\n");

	if(str->args_count>0) {
		OutC(1, "static FIELD_INFO _struct_%s_fieldinfo[] =\n", str->name);
		OutC(1, "{\n");
		for(a=0; a<str->args_count; a++) {
			char type[400];
			char sinfo[400];
			char prefix[400];
			char elen[400];
			char ref_type[400];
			char ref_flags[400] = "0";
			if(str->args[a].count[0]) {
				if(strcmp(str->args[a].type, "content_ref")==0) {
				sprintf(prefix, "((size_t)(&((DDL::Array<A_UUID, %s>*)NULL)->_Array))", str->args[a].count);
				} else if(strcmp(str->args[a].type, "string")==0) {
				sprintf(prefix, "((size_t)(&((DDL::Array<DDL::String<%s>, %s>*)NULL)->_Array))", str->args[a].size, str->args[a].count);
				} else {
				sprintf(prefix, "((size_t)(&((DDL::Array<%s, %s>*)NULL)->_Array))", str->args[a].type, str->args[a].count);
				}
			} else {
				strcpy(prefix, "0");
			}
			if(strcmp(str->args[a].type, "content_ref")==0) {
				sprintf(elen, "sizeof(A_UUID)");
			} else if(strcmp(str->args[a].type, "string")==0) {
				sprintf(elen, "sizeof(DDL::String<%s>)", str->args[a].size);
			} else {
				sprintf(elen, "sizeof(%s)", str->args[a].type);
			}
			get_arg_reflecttype(&str->args[a], type);
			get_arg_structinfo(&str->args[a], sinfo);
			if(strcmp(str->args[a].type, "content_ref")==0)
			{
				sprintf(ref_type, "\"%s\"", str->args[a].size);
			}
			else
			{
				strcpy(ref_type, "NULL");
			}
			if(has_param(&str->args[a], "readonly"))
			{
				strcat(ref_flags, "|FLAG_READONLY");
			}
			if(has_param(&str->args[a], "notnull"))
			{
				strcat(ref_flags, "|FLAG_NOTNULL");
			}
			OutC(2, "{%s, \"%s\", %s, (_U16)ZION_OFFSETOF(%s, %s), %s, (_U16)%s, (_U16)%s, (_U16)%s, (_U16)%s, %s},\n", type, str->args[a].name, ref_flags, str->name, str->args[a].name, sinfo,
				strcmp(str->args[a].type, "string")!=0?"-1":str->args[a].size,
				str->args[a].count[0]=='\0'?"-1":str->args[a].count,
				prefix, elen, ref_type
			);
		}
		OutC(1, "};\n");
		OutC(1, "STRUCT_INFO _rfl_struct_%s_info = { %s, \"%s\", sizeof(%s), %d, _struct_%s_fieldinfo, _struct_%s_readproc, _struct_%s_writeproc };\n", str->name, ss, str->name, str->name, str->args_count, str->name, str->name, str->name);
	} else {
		OutC(1, "STRUCT_INFO _rfl_struct_%s_info = { %s, \"%s\", sizeof(%s), 0, NULL, _struct_%s_readproc, _struct_%s_writeproc };\n", str->name, ss, str->name, str->name, str->name, str->name);
	}
	OutC(0, "	template<>\n");
	OutC(0, "	const STRUCT_INFO* GetStruct<%s>()\n", str->name);
	OutC(0, "	{\n");
	OutC(0, "		return &_rfl_struct_%s_info;\n", str->name);
	OutC(0, "	}\n");
	OutC(0, "}\n");
	OutC(0, "\n");

	return 1;
}
