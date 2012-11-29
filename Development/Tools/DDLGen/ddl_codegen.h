
int ddlgen_code_open(const char* filename);
void ddlgen_code_close();

const char* get_ctype(const DDL_ARG* arg, int is_class);
int is_pointer(const DDL_ARG* arg);
int is_basetype(const DDL_ARG* arg);
int is_array(const DDL_ARG* arg);

int ddlgen_code_include(const char* filename);
int ddlgen_code_const(const DDL_CST* cst);
int ddlgen_code_struct(const DDL_STR* str);
int ddlgen_code_class(const DDL_CLS* cls);
int ddlgen_code_task_class_server(const DDL_CLS* cls, const DDL_TASK* task);
int ddlgen_code_task_class_client(const DDL_CLS* cls, const DDL_TASK* task);
int ddlgen_code_task_class_reflect(const DDL_CLS* cls, const DDL_TASK* task);
int ddlgen_code_task_struct_serialize(const DDL_STR* str, const DDL_TASK* task);
int ddlgen_code_task_struct_reflect(const DDL_STR* str, const DDL_TASK* task);

void ddlgen_param_clear();
void ddlgen_param_add(const char* name, const char* value);
