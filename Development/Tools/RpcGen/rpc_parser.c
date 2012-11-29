#include <string.h>
#include <stdio.h>
#include "rpc_parser.h"

static const char* get_token_char(const char* buf, char c);
static const char* get_token_string(const char* buf, char* value, int size);
static const char* get_token_id(const char* buf, char* value, int size);
static const char* get_token_keyword(const char* buf, const char* keyword, char* value);
static const char* get_token_number(const char* buf, char* value, int size);
static const char* escape_blank(const char* buf);

static const char* parse_int(const char* buf);
static const char* parse_inc(const char* buf);
static const char* parse_arg(const char* buf);
static const char* parse_cmd(const char* buf);

unsigned int ZHOUDAN_MODE = 1;

const char* get_token_char(const char* buf, char c)
{
	buf = escape_blank(buf);
	if(*buf!=c) return NULL;
	return buf+1;
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

	return buf+end+1;
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
	return buf+end;
}

const char* get_token_keyword(const char* buf, const char* keyword, char* value)
{
	char id[100];
	buf = escape_blank(buf);
	buf = get_token_id(buf, id, sizeof(id));
	if(buf==NULL) return NULL;
	if(strcmp(id, keyword)!=0) return NULL;
	if(value) strcpy(value, keyword);
	return buf;
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

	return buf+end;
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

	return buf+end;
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

static RPC_ARG _args[10000];
static RPC_CMD _cmds[1000];
static RPC_CLS _clss[1000];
static char* _incs[1000] = { NULL };
static unsigned int _args_c=0, _cmds_c=0, _clss_c=0, _incs_c=0;
static unsigned int _rpc_function_idx = 0;
static unsigned int _rpc_callback_idx = 0;

const char* parse_int(const char* buf)
{
	char name[100], number[100];
	buf = get_token_keyword(buf, "interface", NULL);
	if(buf==NULL) return NULL;
	buf = get_token_char(buf, '(');
	if(buf==NULL) return NULL;
	buf = get_token_id(buf, name, sizeof(name));
	if(buf==NULL) return NULL;
	buf = get_token_char(buf, ',');
	if(buf==NULL) return NULL;
	buf = get_token_number(buf, number, sizeof(number));
	if(buf==NULL) return NULL;
	buf = get_token_char(buf, ')');
	if(buf==NULL) return NULL;

	buf = get_token_char(buf, '{');
	if(buf==NULL) return NULL;

	strcpy(_clss[_clss_c].name, name);
	strcpy(_clss[_clss_c].number, number);
	_clss[_clss_c].cmds = _cmds + _cmds_c;
	_clss[_clss_c].cmds_count = 0;

	for(;;) {
		const char* tbuf;

		tbuf = parse_cmd(buf);
		if(tbuf) { buf = tbuf; continue; }

		break;
	}

	buf = get_token_char(buf, '}');
	if(buf==NULL) return NULL;
	_clss_c++;
	return buf;
}

const char* parse_inc(const char* buf)
{
	char file[100];
	buf = get_token_char(buf, '#');
	if(buf==NULL) return NULL;
	buf = get_token_keyword(buf, "include", NULL);
	if(buf==NULL) return NULL;
	buf = get_token_string(buf, file, sizeof(file));
	if(buf==NULL) return NULL;
	_incs[_incs_c++] = _strdup(file);
	return buf;
}

const char* parse_arg(const char* buf)
{
	const char* tbuf;
	char type[100], name[100], size[100] = "";

	buf = get_token_id(buf, type, sizeof(type));
	if(buf==NULL) return NULL;
	buf = get_token_id(buf, name, sizeof(name));
	if(buf==NULL) return NULL;

	tbuf = get_token_char(buf, '[');
	if(tbuf!=NULL) {
		tbuf = get_token_exp(tbuf, size, sizeof(size));
		if(tbuf==NULL) return NULL;
		tbuf = get_token_char(tbuf, ']');
		if(tbuf==NULL) return NULL;
		buf = tbuf;
	}

	if(strcmp(type, "_STR")==0) {
		if(size[0]!='\0') {
			printf("type _STR can't user array length\n");
			return NULL;
		}
		strcpy(type, "char");
		sprintf(size, "strlen(%s)+1", name);
	}

	strcpy(_args[_args_c].type, type);
	strcpy(_args[_args_c].name, name);
	strcpy(_args[_args_c].size, size);
	_args_c++;
	_cmds[_cmds_c].args_count++;

	return buf;
}

const char* parse_cmd(const char* buf)
{
	char mode[100], name[100];
	int first = 1;
	const char* tbuf;

	if(get_token_char(buf, '[')) {
		buf = get_token_char(buf, '[');
		if(buf==NULL) return NULL;
		buf = get_token_id(buf, mode, sizeof(mode));
		if(buf==NULL) return NULL;
		buf = get_token_char(buf, ']');
		if(buf==NULL) return NULL;
	} else {
		strcpy(mode, "message");
	}

	buf = get_token_id(buf, name, sizeof(name));
	if(buf==NULL) return NULL;
	buf = get_token_char(buf, '(');
	if(buf==NULL) return NULL;

	memset(&_cmds[_cmds_c], 0, sizeof(_cmds[_cmds_c]));
	_cmds[_cmds_c].args = &_args[_args_c];

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

		buf = parse_arg(buf);
		if(buf==NULL) return NULL;
	}

	buf = get_token_char(buf, ')');
	if(buf==NULL) return NULL;
	buf = get_token_char(buf, ';');
	if(buf==NULL) return NULL;

	if(strcmp(mode, "callback")!=0) {
		_cmds[_cmds_c].index = _rpc_function_idx++;
	} else if(strcmp(mode, "message")!=0) {
		_cmds[_cmds_c].index = _rpc_callback_idx++;
	} else {
		return NULL;
	}

	strcpy(_cmds[_cmds_c].mode, mode);
	strcpy(_cmds[_cmds_c].name, name);
	_cmds_c++;
	_clss[_clss_c].cmds_count++;
	return buf;
}

int rpcgen_parser(const char* text)
{
	const char* buf = text;
	const char* tbuf;

	for(;;) {
		buf = escape_blank(buf);
		if(*buf=='\0') break;

		tbuf = parse_inc(buf);
		if(tbuf) { buf = tbuf; continue; }

		tbuf = parse_int(buf);
		if(tbuf) { buf = tbuf; continue; }
		//tbuf = parse_cmd(buf);
		//if(tbuf) { buf = tbuf; continue; }

		return 0;
	}
	return 1;
}

unsigned int rpcgen_inc_count()
{
	return _incs_c;
}

const char* rpcgen_inc_get(unsigned int index)
{
	if(index>=_incs_c) return NULL;
	return _incs[index];
}


unsigned int rpcgen_getcount()
{
	return _clss_c;
}

const RPC_CLS* rpcgen_get(unsigned int index)
{
	if(index>=_clss_c) return NULL;
	return &_clss[index];
}
