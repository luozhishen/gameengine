#ifndef __BS_CORE_INTERNEL__
#define __BS_CORD_INTERNEL__

int bspatch(u_char* old, off_t oldsize, FILE* f, u_char** new_out, off_t* newsize_out);
int bsdiff(u_char* old, off_t oldsize, u_char* newp, off_t newsize, FILE* pf);

#endif
