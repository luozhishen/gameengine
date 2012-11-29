typedef struct DDL_PARAM {
	char name[100];
	char value[100];
} DDL_PARAM;

typedef struct DDL_CST {
	int ext;
	char type[100];
	char name[100];
	char value[100];
} DDL_CST;

typedef struct DDL_ARG {
	char type[100];
	char name[100];
	char size[100];  // string size
	char count[100]; // array count

	DDL_PARAM* params;
	unsigned int params_count;
} DDL_ARG;

typedef struct DDL_FUN {
	char mode[100];
	char name[100];
	DDL_ARG* args;
	unsigned int args_count;
} DDL_FUN;

typedef struct DDL_CLS {
	int ext;
	char name[100];
	DDL_FUN* funs;
	unsigned int funs_count;
	DDL_PARAM* params;
	unsigned int params_count;
} DDL_CLS;

typedef struct DDL_STR {
	int ext;
	char parent[100];
	char name[100];
	DDL_ARG* args;
	unsigned int args_count;
} DDL_STR;

typedef struct DDL_INC {
	int ext;
	char filename[300];
	char path[300];
	char name[300];
} DDL_INC;

typedef struct DDL_TASK {
	int ext;
	char type[100];
	char name[100];
	DDL_PARAM* params;
	unsigned int params_count;
} DDL_TASK;

extern int ddlgen_parser(const char* filename, const char* outfile);
extern unsigned int ddlgen_get_include_count();
extern unsigned int ddlgen_get_const_count();
extern unsigned int ddlgen_get_struct_count();
extern unsigned int ddlgen_get_class_count();
extern unsigned int ddlgen_get_task_count();
extern const DDL_INC* ddlgen_get_include(unsigned int index);
extern const DDL_CST* ddlgen_get_const(unsigned int index);
extern const DDL_STR* ddlgen_get_struct(unsigned int index);
extern const DDL_CLS* ddlgen_get_class(unsigned int index);
extern const DDL_TASK* ddlgen_get_task(unsigned int index);
extern const DDL_STR* ddlgen_struct(const char* name);
extern const DDL_CLS* ddlgen_class(const char* name);
extern const char* ddlgen_get_cparam(const DDL_CLS* cls, const char* name, const char* def);
extern const char* ddlgen_get_tparam(const DDL_TASK* task, const char* name, const char* def);

extern void ddlgen_error_set(const char* info);
extern const char* ddlgen_error_info();
extern const char* ddlgen_error_filename();
extern int ddlgen_error_line();
extern int ddlgen_error_col();

extern void ddlgen_notify_begin(const char* filename);
extern void ddlgen_notify_end(const char* filename);
extern void ddlgen_notify_include(const char* filename);
extern void ddlgen_notify_const(const DDL_CST* cst);
extern void ddlgen_notify_struct(const DDL_STR* str);
extern void ddlgen_notify_struct_arg(const DDL_STR* str, const DDL_ARG* arg);
extern void ddlgen_notify_struct_end(const DDL_STR* str);
extern void ddlgen_notify_class(const DDL_CLS* cls);
extern void ddlgen_notify_class_function(const DDL_CLS* cls, const DDL_FUN* fun);
extern void ddlgen_notify_class_function_arg(const DDL_CLS* cls, const DDL_FUN* fun, const DDL_ARG* arg);
extern void ddlgen_notify_class_function_end(const DDL_CLS* cls, const DDL_FUN* fun);
extern void ddlgen_notify_class_end(const DDL_CLS* cls);
extern void ddlgen_notify_task(const DDL_TASK* task);
