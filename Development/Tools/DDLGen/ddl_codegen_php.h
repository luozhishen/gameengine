#ifndef __DDL_CODEGEN_PHP__
#define __DDL_CODEGEN_PHP__

int ddlgen_codephp_open(const char* filename);
void ddlgen_codephp_close();

int ddlgen_codephp_task_struct(const DDL_STR* str, const DDL_TASK* task);

#endif
