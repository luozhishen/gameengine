#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheServer.h"
#include "DataCacheJsonRpc.h"
#include "DataCacheRpcImpl.h"
#include "DataCacheDBApi.h"
#include <uv.h>

#include <stdio.h>

namespace Zion
{
	namespace DataCache
	{

		String	CONFIG_DATABASE("sqlite:zion_db.sqlite");
		_U32	CONFIG_DATABASE_COUNT_MAX = 100;
		_U32	CONFIG_DATABASE_COUNT_INIT = 10;
		String	CONFIG_RPCEP("0.0.0.0:1980");
		bool CONFIG_SINGLETHREAD = true;
		bool CONFIG_ENABLE_DB_REPLAYLOG = false;
		bool CONFIG_ENABLE_RPC_REPLAYLOG = false;
		static FILE* _db_logfile = NULL;
		static FILE* _rpc_logfile = NULL;
		static FILE* _rpc_replay = NULL;
		static void ReplayRpc();

		static bool ParseArgs(int argc, char* argv[])
		{
			for(int i=1; i<argc; i++)
			{
				String name, value;
				char* pos = strchr(argv[i], '=');
				if(!pos) pos = argv[i] + strlen(argv[i]);
				char* pos1 = strrchr(argv[i], ':');
				if(pos1 && pos1<pos) pos = pos1;
				name.append(argv[i], pos-argv[i]);
				if(*pos!='\0') value = pos + 1;

				if(name=="-db")
				{
					CONFIG_DATABASE = value;
					continue;
				}
				if(name=="-db_conn_max" && !value.empty())
				{
					CONFIG_DATABASE_COUNT_MAX = (_U32)atoi(value.c_str());
					continue;
				}
				if(name=="-db_conn_init" && !value.empty())
				{
					CONFIG_DATABASE_COUNT_INIT = (_U32)atoi(value.c_str());
					continue;
				}
				if(name=="-rpcep")
				{
					CONFIG_RPCEP = value;
					continue;
				}
				if(name=="-singlethread")
				{
					CONFIG_SINGLETHREAD = true;
					if(value=="false") CONFIG_SINGLETHREAD = false;
					continue;
				}
				if(name=="-db_replaylog")
				{
					_db_logfile = fopen(value.c_str(), "wt");
					if(!_db_logfile)
					{
						ZION_FATAL("open %s failed\n", value.c_str());
					}
					CONFIG_ENABLE_DB_REPLAYLOG = true;
					continue;
				}
				if(name=="-rpc_replaylog")
				{
					_rpc_logfile = fopen(value.c_str(), "wt");
					if(!_rpc_logfile)
					{
						ZION_FATAL("open %s failed\n", value.c_str());
					}
					CONFIG_ENABLE_RPC_REPLAYLOG = true;
					continue;
				}
				if(name=="-rpc_replay")
				{
					_rpc_replay = fopen(value.c_str(), "wt");
					if(!_rpc_replay)
					{
						ZION_FATAL("open %s failed\n", value.c_str());
					}
					continue;
				}
				if(name=="-pid")
				{
					FILE* fp = fopen(value.c_str(), "wt");
					if(!fp)
					{
						ZION_FATAL("open %s failed\n", value.c_str());
					}
					fprintf(fp, "%d", getpid());
					fclose(fp);
					continue;
				}

				return false;
			}

			if(CONFIG_DATABASE_COUNT_INIT<1) CONFIG_DATABASE_COUNT_INIT = 1;
			if(CONFIG_DATABASE_COUNT_MAX<CONFIG_DATABASE_COUNT_INIT) CONFIG_DATABASE_COUNT_MAX = CONFIG_DATABASE_COUNT_INIT;
			if(CONFIG_SINGLETHREAD)
			{
				CONFIG_DATABASE_COUNT_INIT = 1;
				CONFIG_DATABASE_COUNT_MAX = 1;
			}

			if(CONFIG_ENABLE_DB_REPLAYLOG || CONFIG_ENABLE_RPC_REPLAYLOG || _rpc_replay)
			{
				if(!CONFIG_SINGLETHREAD)
				{
					printf("must SINGLETHREAD mode\n");
					CONFIG_SINGLETHREAD = true;
				}
			}

			return true;
		}

		static void sig_exit_cb(uv_signal_t* handle, int signum)
		{
			uv_stop(uv_default_loop());
		}

		extern _U32 _GetAvatar_Count;
		extern _U32 _GetAvatar_Error;
		static void timer_cb(uv_timer_t* handle, int status)
		{
			static _U32 _last_count = 0;
			static _U32 _last_error = 0;
			_U32 count = _GetAvatar_Count - _last_count;
			_U32 error = _GetAvatar_Error - _last_error;
			printf("%12u, %12u\n", count, error);
			_last_count += count;
			_last_error += error;
		}


		int Main(int argc, char* argv[])
		{
			// step 1: parse command line
			if(!ParseArgs(argc, argv))
			{
				printf("invalid parameter\n");
				printf("\n");
				printf("Usage : %s\n", argv[0]);
				printf("		-db=[database string]\n");
				printf("		-rpcep=[127.0.0.1:1980]\n");
				return -1;
			}

			printf("CONFIG :\n");
			printf("	DATABASE = %s\n", CONFIG_DATABASE.c_str());
			printf("	DATABASE_COUNT_MAX = %u\n", CONFIG_DATABASE_COUNT_MAX);
			printf("	DATABASE_COUNT_INIT = %u\n", CONFIG_DATABASE_COUNT_INIT);
			printf("	RPCEP = %s\n", CONFIG_RPCEP.c_str());
			printf("	RPC_THREAD_COUNT = %s\n", CONFIG_SINGLETHREAD?"1":"MULTI");
			printf("\n");

			// step 2: init database
			printf("init database\n");
			if(!InitDatabase())
			{
				ZION_FATAL("FAILED InitDatabase");
			}

			// step 3: start rpc server
			RPCIMPL_Init();
			JsonRPC_Bind("echo",			JsonRPC_Echo);
			JsonRPC_Bind("createAvatar",	JsonRPC_CreateAvatar);
			JsonRPC_Bind("deleteAvatar",	JsonRPC_DeleteAvatar);
			JsonRPC_Bind("getAvatar",		JsonRPC_GetAvatar);
			JsonRPC_Bind("saveAvatar",		JsonRPC_SaveAvatar);
			JsonRPC_Bind("clearAvatar",		JsonRPC_ClearAvatar);
			JsonRPC_Bind("keepAlive",		JsonRPC_KeepAlive);
			JsonRPC_Bind("createObject",	JsonRPC_CreateObject);
			JsonRPC_Bind("updateObject",	JsonRPC_UpdateObject);
			JsonRPC_Bind("deleteObject",	JsonRPC_DeleteObject);
			JsonRPC_Bind("loadObject",		JsonRPC_LoadObjectFromDB);
			JsonRPC_Bind("executeBatch",	JsonRPC_ExecuteBatch);
			printf("start JsonRpc Server...\n");
#ifndef _WIN32
			signal(SIGPIPE, SIG_IGN);
#endif
			if(!_rpc_replay && !JsonRPC_Start(CONFIG_RPCEP.c_str(), CONFIG_SINGLETHREAD))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 4: wait process terminiate signal
			if(!_rpc_replay)
			{
				printf("server running...\n");
				uv_signal_t sig_exit;
				uv_signal_init(uv_default_loop(), &sig_exit);
				uv_signal_start(&sig_exit, sig_exit_cb, SIGINT);
				uv_timer_t echo_timer;
				uv_timer_init(uv_default_loop(), &echo_timer);
				uv_timer_start(&echo_timer, timer_cb, 1000, 1000);
				uv_run(uv_default_loop(), UV_RUN_DEFAULT);
			}
			else
			{
				ReplayRpc();
			}

			// step 5: stop rpc server
			printf("stoping JsonRpc server\n");
			if(!_rpc_replay)
			{
				JsonRPC_Stop();
			}

			// step 6: wait all cache data flush to database
			printf("flash all data to database\n");
			RPCIMPL_FlushAllData();

			// step 7: close database
			printf("fini database\n");
			FiniDatabase();

			// step 8: process exit
			printf("exit.\n");
			if(!_db_logfile)
			{
				fclose(_db_logfile);
			}
			if(!_rpc_logfile)
			{
				fclose(_rpc_logfile);
			}
			if(!_rpc_replay)
			{
				fclose(_rpc_replay);
			}
			return 0;
		}

		void WriteRPCLog(const char* method, const JsonValue& args)
		{
			String json;
			args.Stringify(json);
			fprintf(_db_logfile, "%s %s\n", method, json.c_str());
		}

		void WriteDBLog(const char* method, const JsonValue& args)
		{
			String json;
			args.Stringify(json);
			fprintf(_rpc_logfile, "%s %s\n", method, json.c_str());
		}

		void ReplayRpc()
		{
			static char buf[100000];
			_U32 lineno = 0;
			while(!feof(_rpc_replay))
			{
				buf[0] = '\0';
				if(!fgets(buf, sizeof(buf), _rpc_replay)) ZION_FATAL("read file error : line %u", lineno);
				if(buf[0]=='\0') continue;
				lineno += 1;

				char* pos = strchr(buf, ' ');
				if(!pos) ZION_FATAL("invalid line : line %u", lineno);
				String method;
				method.append(buf, pos-buf);
				JsonValue json;
				if(!json.Parse(pos+1, pos+strlen(pos+1))) ZION_FATAL("invalid json : line %u", lineno);

				if(method=="CreateAvatar") JsonRPC_CreateAvatar(json);
				else if(method=="CreateAvatar") JsonRPC_DeleteAvatar(json);
				else if(method=="GetAvatar") JsonRPC_GetAvatar(json);
				else if(method=="SaveAvatar") JsonRPC_SaveAvatar(json);
				else if(method=="KeepAlive") JsonRPC_KeepAlive(json);
				else if(method=="UpdateObject") JsonRPC_UpdateObject(json);
				else if(method=="DeleteObject") JsonRPC_DeleteObject(json);
				else if(method=="LoadObjectFromDB") JsonRPC_LoadObjectFromDB(json);
				else if(method=="ExecuteBatch") JsonRPC_ExecuteBatch(json);
				else ZION_FATAL("invalid method : line %u", lineno);
			}
		}

	}
}

int main(int argc, char* argv[])
{
	return Zion::DataCache::Main(argc, argv);
}
