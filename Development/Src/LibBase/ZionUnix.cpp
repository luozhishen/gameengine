#ifndef	_WIN32

void zion_set_fatal(bool msgbox)
{
}

void zion_fatal(const char* fmt, ...)
{
}

static _U32 atom_unix_add(_U32 volatile *mem, _U32 val)
{
    asm volatile ("lock; xaddl %0,%1"
                  : "=r" (val), "=m" (*mem)
                  : "0" (val), "m" (*mem)
                  : "memory", "cc");
    return val;
}

_U32 atom_unix_inc(_U32 volatile* mem)
{
    return atom_unix_add(mem, 1) + 1;
}

_U32 atom_unix_dec(_U32 volatile* mem)
{
	_U32 prev;

    asm volatile ("lock; decl %0; setnz %1"
				: "=m" (*mem), "=qm" (prev)
				: "m" (*mem)
				: "memory");

	return prev;
}

_U32 atom_unix_swap(_U32 volatile* mem, _U32 val)
{
	_U32 prev = val;

	asm volatile ("xchgl %0, %1"
				: "=r" (prev), "+m" (*mem)
				: "0" (prev));
    return prev;
}

_U32 atom_unix_cas(_U32 volatile* mem, _U32 with, _U32 cmp)
{
	_U32 prev;

	asm volatile ("lock; cmpxchgl %1, %2"
				: "=a" (prev)
				: "r" (with), "m" (*(mem)), "0"(cmp)
				: "memory", "cc");
	return prev;
}

void atom_slist_init(ATOM_SLIST_HEADER* header)
{
	header->First = NULL;
	header->Count = 0;
}

ATOM_SLIST_ENTRY* atom_slist_pop(ATOM_SLIST_HEADER* head)
{
	ATOM_SLIST_ENTRY* val;

	#ifndef __x86_64__
	asm volatile( "push     %%ebx\n\t"
							"mov                  %1, %%edi\n\t"
							"mov                  (%%edi), %%eax\n\t"
							"mov                  4(%%edi), %%edx\n\t"
							"__L__2:\n\t"
							"or                   %%eax, %%eax\n\t"
							"je __L__1\n\t"
							"mov                  (%%eax), %%ebx\n\t"
							"mov                  %%edx, %%ecx\n\t"
							"dec                  %%ecx\n\t"
							"lock\n\t"
							"cmpxchg8b    (%%edi)\n\t"
							"jne                  __L__2\n\t"
							"__L__1:\n\t"
							"pop                  %%ebx\n\t"
							"mov                  %%eax, %0\n\t"
						: "=g" (val)
						: "g" (head)
						: "eax", "ecx", "edx", "edi", "memory");
	#else
	asm volatile( "push     %%rbx\n\t"
							"mov                  %1, %%rdi\n\t"
							"mov                  (%%rdi), %%rax\n\t"
							"mov                  8(%%rdi), %%rdx\n\t"
							"__L__2:\n\t"
							"or                   %%rax, %%rax\n\t"
							"je __L__1\n\t"
							"mov                  (%%rax), %%rbx\n\t"
							"mov                  %%rdx, %%rcx\n\t"
							"dec                  %%rcx\n\t"
							"lock\n\t"
							"cmpxchg16b   (%%rdi)\n\t"
							"jne                  __L__2\n\t"
							"__L__1:\n\t"
							"pop                  %%rbx\n\t"
							"mov                  %%rax, %0\n\t"
						: "=g" (val)
						: "g" (head)
						: "rax", "rcx", "rdx", "rdi", "memory");
	#endif

	return val;
}

ATOM_SLIST_ENTRY* atom_slist_push(ATOM_SLIST_HEADER* head, ATOM_SLIST_ENTRY* ListEntry)
{
	ATOM_SLIST_ENTRY* p;

	assert(((unsigned long)head&(sizeof(long)-1))==0);
	assert(((unsigned long)ListEntry&(sizeof(long)-1))==0);

	#ifndef __x86_64__
	asm volatile( "push		%%ebx\n\t"
							"mov                  %2, %%ebx\n\t"
							"mov                  %1, %%edi\n\t"
							"mov                  (%%edi), %%eax\n\t"
							"mov                  4(%%edi), %%edx\n\t"
							"__L__3:\n\t"
							"mov                  %%eax, (%%ebx)\n\t"
							"mov                  %%edx, %%ecx\n\t"
							"inc                  %%ecx\n\t"
							"lock\n\t"
							"cmpxchg8b    (%%edi)\n\t"
							"jne                  __L__3\n\t"
							"pop                  %%ebx\n\t"
							"mov                  %%eax, %0\n\t"
						: "=g" (p)
						: "g" (head), "g" (ListEntry)
						: "eax", "ecx", "edx", "edi", "memory");
	#else
	asm volatile( "push		%%rbx\n\t"
							"mov                  %2, %%rbx\n\t"
							"mov                  %1, %%rdi\n\t"
							"mov                  (%%rdi), %%rax\n\t"
							"mov                  8(%%rdi), %%rdx\n\t"
							"__L__3:\n\t"
							"mov                  %%rax, (%%rbx)\n\t"
							"mov                  %%rdx, %%rcx\n\t"
							"inc                  %%rcx\n\t"
							"lock\n\t"
							"cmpxchg16b   (%%rdi)\n\t"
							"jne                  __L__3\n\t"
							"pop                  %%rbx\n\t"
							"mov                  %%rax, %0\n\t"
						: "=g" (p)
						: "g" (head), "g" (ListEntry)
						: "rax", "rcx", "rdx", "rdi", "memory");
	#endif
	return(p);
}

ATOM_SLIST_ENTRY* atom_slist_flush(ATOM_SLIST_HEADER* head)
{
	ATOM_SLIST_ENTRY* p;

	assert(((unsigned long)head&(sizeof(long)-1))==0);

#ifndef __x86_64__
asm volatile( "push			%%ebx\n\t"
							"mov                  %1, %%edi\n\t"
							"mov                  (%%edi), %%eax\n\t"
							"mov                  4(%%edi), %%edx\n\t"
							"xor                  %%ebx, %%ebx\n\t"
							"xor                  %%edx, %%edx\n\t"
							"__L__4:\n\t"
							"or                   %%eax, %%eax\n\t"
							"je                   __L__5\n\t"
							"lock\n\t"
							"cmpxchg8b    (%%edi)\n\t"
							"jne                  __L__4\n\t"
							"__L__5:\n\t"
							"pop                  %%ebx\n\t"
							"mov                  %%eax, %0"
						: "=g" (p)
						: "g" (head)
						: "eax", "ecx", "edx", "edi", "memory");
#else
asm volatile( "push			%%rbx\n\t"
							"mov                  %1, %%rdi\n\t"
							"mov                  (%%rdi), %%rax\n\t"
							"mov                  8(%%rdi), %%rdx\n\t"
							"xor                  %%rbx, %%rbx\n\t"
							"xor                  %%rdx, %%rdx\n\t"
							"__L__4:\n\t"
							"or                   %%rax, %%rax\n\t"
							"je                   __L__5\n\t"
							"lock\n\t"
							"cmpxchg16b   (%%rdi)\n\t"
							"jne                  __L__4\n\t"
							"__L__5:\n\t"
							"pop                  %%rbx\n\t"
							"mov                  %%rax, %0"
						: "=g" (p)
						: "g" (head)
						: "rax", "rcx", "rdx", "rdi", "memory");
#endif
        return(p);
}

#endif
