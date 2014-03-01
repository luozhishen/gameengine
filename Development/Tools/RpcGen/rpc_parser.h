
typedef struct RPC_ARG {
	char type[100];
	char name[100];
	char size[100];
} RPC_ARG;

typedef struct RPC_CMD {
	unsigned int index;

	char mode[100];
	char name[100];
	RPC_ARG* args;
	unsigned int args_count;
} RPC_CMD;

typedef struct RPC_CLS {
	char name[100];
	char number[100];
	RPC_CMD* cmds;
	unsigned cmds_count;
} RPC_CLS;

extern int rpcgen_parser(const char* text);
extern unsigned int rpcgen_inc_count();
extern const char* rpcgen_inc_get(unsigned int index);
extern unsigned int rpcgen_getcount();
extern const RPC_CLS* rpcgen_get(unsigned int index);

