#ifndef __DDL_CODEGEN_JAVASCRIPT__
#define __DDL_CODEGEN_JAVASCRIPT__

int ddlgen_codejs_open(const char* filename);
void ddlgen_codejs_close();

int ddlgen_codejs_task_struct(const DDL_STR* str, const DDL_TASK* task);
int ddlgen_codejs_task_class_stub(const DDL_CLS* str, const DDL_TASK* task);
int ddlgen_codejs_task_class_proxy(const DDL_CLS* str, const DDL_TASK* task);

#endif
