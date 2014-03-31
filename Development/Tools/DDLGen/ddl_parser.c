#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ddl_parser.h"
#include "ddl_codegen.h"

#ifndef _WIN32
#define stricmp strcasecmp
#endif

static const char* get_token_char(const char* buf, char c);
static const char* get_token_string(const char* buf, char* value, int size);
static const char* get_token_id(const char* buf, char* value, int size);
static const char* get_token_keyword(const char* buf, const char* keyword, char* value);
static const char* get_token_number(const char* buf, char* value, int size);
static const char* escape_blank(const char* buf);
static const char* escape_char(const char* buf, char c);

const char* get_token_char(const char* buf, char c)
{
	buf = escape_blank(buf);
	if(*buf!=c) return NULL;
	return escape_blank(buf+1);
}

const char* get_token_string(const char* buf, char* value, int size)
{
	int end;

	buf = escape_blank(buf);
	if(*buf!='"') return NULL;
	for(end=1; ;end++) {
		if(buf[end]=='\0') return NULL;
		if(buf[end]=='"') break;
		if(buf[end]=='\\') {
			if(buf[end-1]=='\0') return NULL;
			end++; continue;
		}
	}
	if(end+2>size) return NULL;

	memcpy(value, buf+1, end+1-1);
	value[end+1-2] = '\0';

	return escape_blank(buf+end+1);
}

const char* get_token_id(const char* buf, char* value, int size)
{
	int end;

	buf = escape_blank(buf);

	for(end=0;; end++) {
		if(buf[end]>='0' && buf[end]<='9') {
			if(end==0) return NULL;
			continue;
		}
		if(buf[end]>='a' && buf[end]<='z') continue;
		if(buf[end]>='A' && buf[end]<='Z') continue;
		if(buf[end]=='_') continue;

		break;
	}
	if(end==0) return NULL;

	if(end+1>size) return NULL;
	memcpy(value, buf, end);
	value[end] = '\0';
	return escape_blank(buf+end);
}

const char* get_token_keyword(const char* buf, const char* keyword, char* value)
{
	char id[100];
	buf = escape_blank(buf);
	buf = get_token_id(buf, id, sizeof(id));
	if(buf==NULL) return NULL;
	if(strcmp(id, keyword)!=0) return NULL;
	if(value) strcpy(value, keyword);
	return escape_blank(buf);
}

const char* get_token_number(const char* buf, char* value, int size)
{
	int end;
	const char* tbuf;
	char kk[100];

	buf = escape_blank(buf);

	for(end=0; ;end++) {
		if(buf[end]>='0' && buf[end]<='9') continue;
		if(buf[end]=='+') continue;
		if(buf[end]=='-') continue;
		if(buf[end]=='*') continue;
		if(buf[end]=='/') continue;
		if(buf[end]=='.') continue;
		tbuf = get_token_id(buf+end, kk, sizeof(kk));
		if(tbuf) {
			end = (int)(tbuf - buf - 1);
			continue;
		}
		if(end==0) return NULL;
		break;
	}

	if(end+1>size) return NULL;
	memcpy(value, buf, end);
	value[end] = '\0';

	return escape_blank(buf+end);
}

const char* get_token_exp(const char* buf, char* value, int size)
{
	int end;

	buf = escape_blank(buf);

	for(end=0; ;end++) {
		if(buf[end]>='0' && buf[end]<='9') continue;
		if(buf[end]=='+') continue;
		if(buf[end]=='-') continue;
		if(buf[end]=='*') continue;
		if(buf[end]=='/') continue;
		if(buf[end]=='.') continue;
		if(buf[end]>='a' && buf[end]<='z') continue;
		if(buf[end]>='A' && buf[end]<='Z') continue;
		if(buf[end]=='_') continue;
		if(buf[end]=='(') continue;
		if(buf[end]==')') continue;

		if(end==0) return NULL;
		break;
	}

	if(end+1>size) return NULL;
	memcpy(value, buf, end);
	value[end] = '\0';

	return escape_blank(buf+end);
}

const char* escape_blank(const char* buf)
{
	for(;;buf++) {

		if(*buf=='/' && buf[1]=='/') {
			buf+=2;
			while(*buf!='\n' && *buf!='\0') buf++;
		}
		if(*buf=='\0') return buf;
		if(*buf<=' ') continue;

		return buf;
	}
}

const char* escape_char(const char* buf, char c)
{
	for(;;buf++) {
		buf = escape_blank(buf);
		if(*buf!=c) break;
	}
	return buf;
}

typedef struct PARSER_STACK {
	char path[300];
	char name[300];
	const char* buf;
} PARSER_STACK;
static PARSER_STACK _stacks[20];
static unsigned int _stacks_count;

static DDL_INC _incs[10000];
static DDL_CST _csts[10000];
static DDL_ARG _args[10000];
static DDL_FUN _funs[10000];
static DDL_PARAM _params[10000];
static DDL_CLS _clss[10000];
static DDL_STR _strs[10000];
static DDL_TASK _tasks[10000];
static unsigned int _incs_c=0, _csts_c=0, _args_c=0, _strs_c=0, _funs_c=0, _params_c=0, _clss_c=0, _tasks_c=0;
static const char* _error_info;
static int _error_line, _error_col;
static const char* _error_info_set;

static char* split_filename(const char* filename, char* path, char* name);
static const char* set_error(const char* info, const char* pos);

static int parse(const char* path, const char* name, const char* buf);
static const char* parse_inc(const char* buf);
static const char* parse_cst(const char* buf);
static const char* parse_arg(const char* buf, int is_class);
static const char* parse_str(const char* buf);
static const char* parse_fun(const char* buf);
static const char* parse_param(const char* buf);
static const char* parse_cls(const char* buf);
static const char* parse_task(const char* buf);

int ddlgen_parser(const char* filename, const char* outfile)
{
	char* buf;
	char path[300], name[300];
	int ret;

	_incs_c = 0;
	_csts_c = 0;
	_args_c = 0;
	_strs_c = 0;
	_funs_c = 0;
	_clss_c = 0;
	_tasks_c = 0;
	_stacks_count = 0;
	_error_info = NULL;
	_error_line = 0;
	_error_col = 0;
	_error_info_set = NULL;

	buf = split_filename(filename, path, name);
	if(buf==NULL) {
		set_error("can't open input file", NULL);
		return 0;
	}

	_incs[_incs_c].ext = 0;
	strcpy(_incs[_incs_c].filename, "ROOT");
	strcpy(_incs[_incs_c].path, path);
	strcpy(_incs[_incs_c].name, name);
	_incs_c++;

	ddlgen_notify_begin(outfile);
	if(_error_info_set) {
		set_error(_error_info_set, NULL);
		return 0;
	}
	ret = parse(path, name, buf);
	if(!ret) {
		if(_error_info_set) {
			set_error(_error_info_set, NULL);
			return 0;
		}
	}

	ddlgen_notify_end(outfile);
	free(buf);
	return ret;
}

unsigned int ddlgen_get_include_count() { return _incs_c; }
unsigned int ddlgen_get_const_count() { return _csts_c; }
unsigned int ddlgen_get_struct_count() { return _strs_c; }
unsigned int ddlgen_get_class_count() { return _clss_c; }
unsigned int ddlgen_get_task_count() { return _tasks_c; }
const DDL_INC* ddlgen_get_include(unsigned int index) { return &_incs[index]; }
const DDL_CST* ddlgen_get_const(unsigned int index) { return &_csts[index]; }
const DDL_STR* ddlgen_get_struct(unsigned int index) { return &_strs[index]; }
const DDL_CLS* ddlgen_get_class(unsigned int index) { return &_clss[index]; }
const DDL_TASK* ddlgen_get_task(unsigned int index) { return &_tasks[index]; }

const DDL_STR* ddlgen_struct(const char* name)
{
	unsigned int i;
	for(i=0; i<_strs_c; i++) {
		if(strcmp(_strs[i].name, name)==0) return &_strs[i];
	}
	return NULL;
}

const DDL_CLS* ddlgen_class(const char* name)
{
	unsigned int i;
	for(i=0; i<_clss_c; i++) {
		if(strcmp(_clss[i].name, name)==0) return &_clss[i];
	}
	return NULL;
}

const char* ddlgen_get_cparam(const DDL_CLS* cls, const char* name, const char* def)
{
	unsigned int i;
	for(i=0; i<cls->params_count; i++) {
		if(stricmp(cls->params[i].name, name)==0) {
			return cls->params[i].value;
		}
	}
	return def;
}

const char* ddlgen_get_tparam(const DDL_TASK* task, const char* name, const char* def)
{
	unsigned int i;
	for(i=0; i<task->params_count; i++) {
		if(stricmp(task->params[i].name, name)==0) {
			return task->params[i].value;
		}
	}
	return def;
}

void ddlgen_error_set(const char* info) { _error_info_set = info; }
const char* ddlgen_error_info() { return _error_info; }
const char* ddlgen_error_filename() { return _stacks_count>0?_stacks[_stacks_count-1].name:NULL; }
int ddlgen_error_line() { return _error_line; }
int ddlgen_error_col() { return _error_col; }

char* split_filename(const char* filename, char* path, char* name)
{
	const char* split;
	const char* cptr;
	char* buf;
	char curdir[300];

	FILE* fp;
	int size;

	split = strrchr(filename, '\\');
	cptr = strrchr(filename, '/');
	if(cptr>split) split = cptr;

	if(!getcwd(curdir, sizeof(curdir))) {
		assert(0);
	}
	if(split) {
		strcpy(path, filename);
		path[split-filename] = '\0';
		if(chdir(path)!=0) {
			assert(0);
		}
		if(!getcwd(path, sizeof(curdir))) {
			assert(0);
		}
		strcpy(name, split+1);
	} else {
		strcpy(path, curdir);
		strcpy(name, filename);
	}
	if(chdir(curdir)!=0) {
		assert(0);
	}

	buf = NULL;
	fp = fopen(filename, "rt");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		size = (int)ftell(fp) * 2;
		fseek(fp, 0, SEEK_SET);
		buf = (char*)malloc(size+10);
		buf[0] = '\0';
		while(!feof(fp)) {
			if(fgets(buf+strlen(buf), size-(int)strlen(buf), fp)==NULL) {
				break;
			}
		}
		fclose(fp);
	}

	return buf;
}

const char* set_error(const char* info, const char* pos)
{
	if(!_error_info) {
		_error_info = info;
		if(pos && _stacks_count>0 && pos!=NULL) {
			const char* v;
			_error_line = 1;
			_error_col = 0;
			v = _stacks[_stacks_count-1].buf;
			for(; v<=pos; v++) {
				if(*v=='\n') {
					_error_line++;
					_error_col = 0;
				} else {
					_error_col++;
				}
			}
		}
	}
	return NULL;
}

int parse(const char* path, const char* name, const char* buf)
{
	const char* tbuf;
	char curpath[300];

	if(!getcwd(curpath, sizeof(curpath))) {
		assert(0);
	}

	strcpy(_stacks[_stacks_count].path, path);
	strcpy(_stacks[_stacks_count].name, name);
	_stacks[_stacks_count].buf = buf;
	_stacks_count++;
	if(chdir(path)!=0) {
		assert(0);
	}
	for(;;) {
		buf = escape_blank(buf);
		if(*buf=='\0') { break; }

		tbuf = parse_inc(buf);
		if(tbuf) { buf = tbuf; continue; }

		tbuf = parse_cst(buf);
		if(tbuf) { buf = tbuf; continue; }

		tbuf = parse_str(buf);
		if(tbuf) { buf = tbuf; continue; }

		tbuf = parse_cls(buf);
		if(tbuf) { buf = tbuf; continue; }

		tbuf = parse_task(buf);
		if(tbuf) { buf = tbuf; continue; }

		// set error unknown
		set_error("unknown word", buf);
		if(chdir(curpath)!=0) {
			assert(0);
		}
		return 0;
	}
	_stacks_count--;

	if(chdir(curpath)!=0) {
		assert(0);
	}
	return 1;
}

const char* parse_inc(const char* buf)
{
	char file[100];
	const char* tbuf;
	char path[300], name[300];
	char* txt;
	int i;

	buf = get_token_keyword(buf, "include", NULL);
	if(buf==NULL) return NULL;
	tbuf = get_token_string(buf, file, sizeof(file));
	if(tbuf==NULL) {
		return set_error("include need filename", buf);
	}

	txt = split_filename(file, path, name);
	if(txt==NULL) {
		return set_error("can't open input file", buf);
	}
	for(i=0; i<(int)_incs_c; i++) {
		if(strcmp(_incs[i].path, path)==0 && strcmp(_incs[i].name, name)==0) {
			break;
		}
	}
	_incs[_incs_c].ext = (_stacks_count==1);
	strcpy(_incs[_incs_c].filename, file);
	strcpy(_incs[_incs_c].path, path);
	strcpy(_incs[_incs_c].name, name);
	_incs_c++;
	if(i==(int)_incs_c-1) {
		if(!parse(path, name, txt)) {
			return set_error("error in parse", buf);
		}
	}
	free(txt);

	if(_stacks_count==1) {
		ddlgen_notify_include(file);
		if(_error_info_set) {
			return set_error(_error_info_set, buf);
		}
	}
	buf = tbuf;
	return buf;
}

const char* parse_cst(const char* buf)
{
	char name[100], type[100], value[100];
	const char* tbuf;
	const char* start = buf;
	buf = get_token_keyword(buf, "const", NULL);
	if(buf==NULL) return NULL;
	tbuf = get_token_id(buf, type, sizeof(type));
	if(tbuf==NULL) {
		return set_error("unknown type", buf);
	}
	buf = tbuf;
	tbuf = get_token_id(buf, name, sizeof(name));
	if(tbuf==NULL) {
		return set_error("unknown name", buf);
	}
	buf = tbuf;
	tbuf = get_token_char(buf, '=');
	if(tbuf==NULL) {
		return set_error("can't find =", buf);
	}
	buf = tbuf;
	tbuf = get_token_exp(buf, value, sizeof(value));
	if(tbuf==NULL) {
		return set_error("unknown expression", buf);
	}
	buf = tbuf;
	tbuf = get_token_char(buf, ';');
	if(tbuf==NULL) {
		return set_error("can't find ;", buf);
	}
	buf = escape_char(tbuf, ';');
	strcpy(_csts[_csts_c].type, type);
	strcpy(_csts[_csts_c].name, name);
	strcpy(_csts[_csts_c].value, value);
	_csts[_csts_c].ext = (_stacks_count!=1);
	_csts_c++;
	ddlgen_notify_const(&_csts[_csts_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}
	return buf;
}

const char* parse_arg_string(const char* buf, DDL_ARG* arg)
{
	const char* tbuf;

	buf = get_token_keyword(buf, "string", arg->type);
	if(buf==NULL) return NULL;

	tbuf = get_token_char(buf, '<');
	if(tbuf) {
		buf = tbuf;
		tbuf = get_token_exp(buf, arg->size, sizeof(arg->size));
		if(tbuf==NULL) {
			return set_error("unknown string length", buf);
		}
		buf = tbuf;
		tbuf = get_token_char(buf, '>');
		if(tbuf==NULL) {
			return set_error("can't find >", buf);
		}
		buf = tbuf;
	}

	return buf;
}

const char* parse_arg_content_ref(const char* buf, DDL_ARG* arg)
{
	const char* tbuf;

	tbuf = get_token_id(buf, arg->type, sizeof(arg->type));
	if(!tbuf) {
		return NULL;
	}
	
	if(strcmp(arg->type, "content_ref")!=0)
	{
		return NULL;
	}

	buf = tbuf;
	tbuf = get_token_char(buf, '<');
	if(!tbuf) {
		return set_error("unknown", buf);
	}
	buf = tbuf;
	tbuf = get_token_id(buf, arg->size, sizeof(arg->size));
	if(!tbuf) {
		return set_error("unknown", buf);
	}
	buf = tbuf;
	tbuf = get_token_char(buf, '>');
	if(!tbuf) {
		return set_error("unknown", buf);
	}

	return tbuf;
}

const char* parse_arg_array(const char* buf, DDL_ARG* arg)
{
	const char* tbuf;

	tbuf = get_token_keyword(buf, "array", NULL);
	if(!tbuf) return NULL;
	buf = tbuf;

	tbuf = get_token_char(buf, '<');
	if(tbuf==NULL) {
		return set_error("can't find <", buf);
	}
	buf = tbuf;

	tbuf = parse_arg_string(buf, arg);
	if(tbuf==NULL) {
		tbuf = parse_arg_content_ref(buf, &_args[_args_c]);
		if(!tbuf)
		{
			tbuf = get_token_id(buf, arg->type, sizeof(arg->type));
			if(tbuf==NULL) {
				return set_error("unknown type", buf);
			}
		}
	}
	buf = tbuf;

	tbuf = get_token_char(buf, ',');
	if(tbuf==NULL) {
		return set_error("cannot found ,", buf);
	}
	buf = tbuf;

	tbuf = get_token_exp(buf, arg->count, sizeof(arg->count));
	if(tbuf==NULL) {
		return set_error("unknown array count", buf);
	}
	buf = tbuf;

	tbuf = get_token_char(buf, '>');
	if(tbuf==NULL) {
		return set_error("can't find >", buf);
	}
	buf = tbuf;

	return buf;
}

const char* parse_arg(const char* buf, int is_class)
{
	const char* tbuf;

	memset(&_args[_args_c], 0, sizeof(_args[_args_c]));

	_args[_args_c].params = _params + _args_c;
	if(!is_class) {
		tbuf = get_token_char(buf, '[');
		if(tbuf) {
			buf = tbuf;
			for(;;) {
				tbuf = get_token_char(buf, ']');
				if(tbuf) break;

				_params[_params_c].name[0] = '\0';
				_params[_params_c].value[0] = '\0';
				tbuf = get_token_id(buf, _params[_params_c].name, sizeof(_params[_params_c].name));
				if(!tbuf) {
					return set_error("unknown arg param", buf);
				}
				_args[_args_c].params_count++;
				_params_c++;

				buf = tbuf;
				tbuf = get_token_char(buf, ']');
				if(tbuf) break;

				tbuf = get_token_char(buf, ',');
				if(!tbuf) {
					return set_error("need ,", buf);
				}
				buf = tbuf;
			}
			buf = tbuf;
		}
	}

	tbuf = parse_arg_string(buf, &_args[_args_c]);
	if(!tbuf) {
		if(!is_class) {
			tbuf = parse_arg_array(buf, &_args[_args_c]);
		}
		if(!tbuf) {
			if(!is_class) {
				tbuf = parse_arg_content_ref(buf, &_args[_args_c]);
			}
			if(!tbuf) {
				tbuf = get_token_id(buf, _args[_args_c].type, sizeof(_args[_args_c].type));
				if(!tbuf) {
					return set_error("unknown type", buf);
				}
			}
		}
	}
	if(strcmp(_args[_args_c].type, "string")==0) { }
	else if(strcmp(_args[_args_c].type, "A_UUID")==0) { }
	else if(strcmp(_args[_args_c].type, "content_ref")==0) { }
	else if(is_basetype(&_args[_args_c])) { }
	else if(ddlgen_struct(_args[_args_c].type)) { }
	else {
		return set_error("unknown type", buf);
	}
	buf = tbuf;

	tbuf = get_token_id(buf, _args[_args_c].name, sizeof(_args[_args_c].name));
	if(!tbuf) {
		return set_error("unknown arg name", buf);
	}
	buf = tbuf;

	if(is_class) {
		tbuf = get_token_char(buf, '[');
		if(tbuf) {
			buf = tbuf;
			tbuf = get_token_exp(buf, _args[_args_c].count, sizeof(_args[_args_c].count));
			if(!tbuf) {
				return set_error("unknown pointer size", buf);
			}
			buf = tbuf;
			tbuf = get_token_char(buf, ']');
			if(!tbuf) {
				return set_error("cannot found ]", buf);
			}
			buf = tbuf;
		}
	}

	if(is_class) {
		_funs[_funs_c].args_count++;
	} else {
		_strs[_strs_c].args_count++;
	}

	_args_c++;

	return buf;
}

const char* parse_str(const char* buf)
{
	char parent[100];
	char name[100];
	const char* tbuf;
	const char* start = buf;

	buf = get_token_keyword(buf, "struct", NULL);
	if(buf==NULL) return NULL;
	tbuf = get_token_id(buf, name, sizeof(name));
	if(tbuf==NULL) {
		return set_error("unknown expression", buf);
	}
	buf = tbuf;

	tbuf = get_token_char(buf, ':');
	if(tbuf) {
		buf = tbuf;
		tbuf = get_token_id(buf, parent, sizeof(parent));
		if(tbuf==NULL) {
			return set_error("unknown parent struct name", buf);
		}
		if(strcmp(name, parent)==0) {
			return set_error("invalid parent struct", buf);
		}
		if(!ddlgen_struct(parent)) {
			return set_error("parent struct not found", buf);
		}
		buf = tbuf;
	} else {
		parent[0] = '\0';
	}

	tbuf = get_token_char(buf, '{');
	if(tbuf==NULL) {
		return set_error("can't find {", buf);
	}
	buf = tbuf;

	memset(&_strs[_strs_c], 0, sizeof(_strs[_strs_c]));
	strcpy(_strs[_strs_c].parent, parent);
	strcpy(_strs[_strs_c].name, name);
	_strs[_strs_c].args = &_args[_args_c];
	_strs[_strs_c].ext = (_stacks_count!=1);
	_strs_c++;
	ddlgen_notify_struct(&_strs[_strs_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}

	for(;;) {
		if(*buf=='}') break;

		tbuf = parse_arg(buf, 0);
		if(tbuf==NULL) {
			return NULL;
		}
		_strs[_strs_c-1].args_count++;
		ddlgen_notify_struct_arg(&_strs[_strs_c-1], &_args[_args_c-1]);
		if(_error_info_set) {
			return set_error(_error_info_set, buf);
		}
		buf = tbuf;
		buf = get_token_char(buf, ';');
		if(buf==NULL) {
			return set_error("can't find ; ", tbuf);
		}
	};

	tbuf = get_token_char(buf, '}');
	if(buf==NULL) {
		return set_error("can't find }", buf);
	}
	buf = escape_char(tbuf, ';');

	ddlgen_notify_struct_end(&_strs[_strs_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}
	return buf;
}

const char* parse_fun(const char* buf)
{
	char mode[100], name[100];
	int first = 1;
	const char* tbuf;
	const char* start = buf;

	if(get_token_char(buf, '[')) {
		buf = get_token_char(buf, '[');
		if(buf==NULL) return NULL;
		buf = get_token_id(buf, mode, sizeof(mode));
		if(buf==NULL) return NULL;
		buf = get_token_char(buf, ']');
		if(buf==NULL) return NULL;
	} else {
		strcpy(mode, "");
	}

	buf = get_token_id(buf, name, sizeof(name));
	if(buf==NULL) return NULL;
	buf = get_token_char(buf, '(');
	if(buf==NULL) return NULL;

	memset(&_funs[_funs_c], 0, sizeof(_funs[_funs_c]));
	_funs[_funs_c].args = &_args[_args_c];
	strcpy(_funs[_funs_c].mode, mode);
	strcpy(_funs[_funs_c].name, name);
	_funs_c++;
	ddlgen_notify_class_function(&_clss[_clss_c-1], &_funs[_funs_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}

	for(;;) {
		if(!first) {
			tbuf = get_token_char(buf, ',');
			if(tbuf==NULL) break;
			buf = tbuf;
		} else {
			first = 0;
			tbuf = get_token_char(buf, ')');
			if(tbuf!=NULL) break;
		}

		tbuf = parse_arg(buf, 1);
		if(tbuf==NULL) {
			set_error("unknown word", buf);
			return NULL;
		}
		_funs[_funs_c-1].args_count++;
		ddlgen_notify_class_function_arg(&_clss[_clss_c-1], &_funs[_funs_c-1], &_args[_args_c-1]);
		if(_error_info_set) {
			return set_error(_error_info_set, buf);
		}
		buf = tbuf;
	}

	tbuf = get_token_char(buf, ')');
	if(tbuf==NULL) {
		return set_error("can't find )", buf);
	}
	buf = tbuf;
	tbuf = get_token_char(buf, ';');
	if(tbuf==NULL) {
		return set_error("can't find ;", buf);
	}
	buf = escape_char(tbuf, ';');

	ddlgen_notify_class_function_end(&_clss[_clss_c-1], &_funs[_funs_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}
	return buf;
}

const char* parse_param(const char* buf)
{
	const char* tbuf;
	char name[100], value[100];

	tbuf = get_token_id(buf, name, sizeof(name));
	if(!tbuf) return NULL;
	buf = tbuf;

	tbuf = get_token_char(buf, '=');
	if(!tbuf) return set_error("= not found", buf);
	buf = tbuf;

	tbuf = get_token_exp(buf, value, sizeof(value));
	if(!tbuf) {
		tbuf = get_token_string(buf, value, sizeof(value));
		if(!tbuf) return set_error("unknown word", buf);
	}
	buf = tbuf;

	strcpy(_params[_params_c].name, name);
	strcpy(_params[_params_c].value, value);
	_params_c++;

	return buf;
}

const char* parse_cls(const char* buf)
{
	char name[100];
	const char* tbuf;
	const char* start = buf;

	buf = get_token_keyword(buf, "class", NULL);
	if(buf==NULL) return NULL;
	tbuf = get_token_id(buf, name, sizeof(name));
	if(buf==NULL) {
		return set_error("unknown name", buf);
	}
	buf = tbuf;
	tbuf = get_token_char(buf, '{');
	if(tbuf==NULL) {
		return set_error("can't find {", buf);
	}
	buf = tbuf;

	memset(&_clss[_clss_c], 0, sizeof(_clss[_clss_c]));
	strcpy(_clss[_clss_c].name, name);
	_clss[_clss_c].funs = &_funs[_funs_c];
	_clss[_clss_c].params = &_params[_params_c];
	_clss[_clss_c].ext = (_stacks_count!=1);
	_clss_c++;
	ddlgen_notify_class(&_clss[_clss_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}

	for(;;) {
		const char* tbuf;

		tbuf = get_token_char(buf, '[');
		if(tbuf) {
			int first = 1;
			buf = tbuf;

			for(;;) {
				if(!first) {
					tbuf = get_token_char(buf, ',');
					if(!tbuf) break;
					buf = tbuf;
				}

				tbuf = parse_param(buf);
				if(!tbuf) {
					if(first) return set_error("unknown word", buf);
					break;
				}
				buf = tbuf;
				_clss[_clss_c-1].params_count++;

				first = 0;
			}

			tbuf = get_token_char(buf, ']');
			if(!tbuf) return set_error("] not found", buf);
			buf = tbuf;
			continue;
		}

		tbuf = parse_fun(buf);
		if(tbuf) {
			_clss[_clss_c-1].funs_count++;
			buf = tbuf;
			continue;
		}

		tbuf = parse_param(buf);
		if(tbuf) {
			_clss[_clss_c-1].params_count++;
			buf = tbuf;
			continue;
		}

		break;
	}

	tbuf = get_token_char(buf, '}');
	if(tbuf==NULL) {
		return set_error("can't find }", buf);
	}
	buf = escape_char(tbuf, ';');

	ddlgen_notify_class_end(&_clss[_clss_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}
	return buf;
}

const char* parse_task(const char* buf)
{
	char type[100], name[100];
	const char* tbuf;
	const char* start = buf;

	buf = get_token_keyword(buf, "task", NULL);
	if(buf==NULL) return NULL;
	tbuf = get_token_char(buf, '[');
	if(tbuf==NULL) {
		return set_error("can't find [", buf);
	}
	buf = tbuf;
	tbuf = get_token_id(buf, type, sizeof(type));
	if(buf==NULL) {
		return set_error("unknown type", buf);
	}
	buf = tbuf;
	name[0] = '\0';
	tbuf = get_token_char(buf, '(');
	if(tbuf!=NULL) {
		buf = tbuf;
		tbuf = get_token_id(buf, name, sizeof(name));
		if(!tbuf) return set_error("unknown word", buf);
		buf = tbuf;
		tbuf = get_token_char(buf, ')');
		if(!tbuf) return set_error(") not found", buf);
		buf = tbuf;
	}

	strcpy(_tasks[_tasks_c].type, type);
	strcpy(_tasks[_tasks_c].name, name);
	_tasks[_tasks_c].ext = (_stacks_count!=1);
	_tasks_c++;

	for(;;) {
		tbuf = get_token_char(buf, ',');
		if(!tbuf) break;
		buf = tbuf;

		tbuf = parse_param(buf);
		if(!tbuf) return set_error("unknown word", buf);
		buf = tbuf;
		_tasks[_tasks_c-1].params_count++;
	}

	tbuf = get_token_char(buf, ']');
	if(tbuf==NULL) {
		return set_error("can't find ]", buf);
	}
	buf = escape_char(tbuf, ';');

	ddlgen_notify_task(&_tasks[_tasks_c-1]);
	if(_error_info_set) {
		return set_error(_error_info_set, start);
	}

	return buf;
}
