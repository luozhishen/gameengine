#include <map>

#include "AtlasDefines.h"
#include "AsyncSockIO.h"
#include "MemPool.h"
#include "DDL.h"
#include "AtlasUtils.h"
#include "AsyncRPC.h"
#include "AsyncRPCImpl.h"

namespace Atlas {

	#define	RPC_SERVER_NONE				0
	#define	RPC_SERVER_CONNECTING		1
	#define	RPC_SERVER_CONNECTED		2
	#define	RPC_SERVER_FAILED			3
	#define	RPC_SERVER_DISCONNECTED		4
	#define	RPC_SERVER_STOPPING			5

	struct RPC_SERVER {
		HCONNECT hConn;
		_S32 state;
		SOCKADDR sain;
		A_MUTEX lock;
		_U32 curlen;
		RPC_PACKET packet;

		bool MakeCall(_U16 iid, _U16 fid, _U32 len, const _U8* buf) {
			if(iid>=RPC_IFTBL_SIZE || fid>=lwrpc_cbinterface_table[iid].cbfcount) return false;
			RPC_INPUT_BUF rib(len, buf);
			return lwrpc_cbinterface_table[iid].cbfunc_tbl[fid]((HSERVER)this, rib);
		}
		bool MakeCall(RPC_PACKET* pp) {
			return MakeCall(pp->iid, pp->fid, pp->len, pp->buf);
		}
		bool MakeCall() {
			return MakeCall(packet.iid, packet.fid, packet.len, packet.buf);
		}
		void OnData(_U32 len, const _U8* buf) {
			if(!IsConnected(hConn)) return;
			do {
				if(curlen) {
					_U32 cpylen;
					if(curlen<RPC_PACKET_OVERHEAD) {
						cpylen = RPC_PACKET_OVERHEAD - curlen;
						if(len<cpylen) {
							cpylen = len;
						}
						memcpy(((_U8*)&packet)+curlen, buf, cpylen);
						curlen += cpylen;
						len -= cpylen;
						buf += cpylen;
					} else {
						if(packet.len>RPC_MAX_LEN) {
							Disconnect(hConn);
							return;
						}
						if(curlen==RPC_PACKET_OVERHEAD && len>=packet.len) {
							if(MakeCall(packet.iid, packet.fid, packet.len, buf)) {
								curlen = 0;
								len -= packet.len;
								buf += packet.len;
							} else {
								Disconnect(hConn);
								return;
							}
						} else {
							cpylen = RPC_PACKET_OVERHEAD + packet.len - curlen;
							if(len<cpylen) {
								cpylen = len;
							}
							memcpy(((_U8*)&packet)+curlen, buf, cpylen);
							curlen += cpylen;
							len -= cpylen;
							buf += cpylen;
							if(curlen==RPC_PACKET_OVERHEAD+packet.len) {
								if(MakeCall()) {
									curlen = 0;
								} else {
									Disconnect(hConn);
									return;
								}
							}
						}
					}
				} else {
					if(len<RPC_PACKET_OVERHEAD) {
						memcpy(&packet, buf, len);
						curlen = len;
						return;
					} else {
						RPC_PACKET* pp = (RPC_PACKET*)buf;
						if(pp->len>RPC_MAX_LEN) {
							Disconnect(hConn);
							return;
						}
						_U32 tlen = RPC_PACKET_OVERHEAD + pp->len;
						if(len>=tlen) {
							if(MakeCall(pp)) {
								len -= tlen;
								buf += tlen;
							} else {
								Disconnect(hConn);
								return;
							}
						} else {
							memcpy(&packet, buf, len);
							curlen = len;
							return;
						}
					}
				}
			} while(len);
		}
		bool init(_U32 ip, _U16 port) {
			A_MUTEX_INIT(&lock);
			sain.ip = ip;
			sain.port = port;
			state = RPC_SERVER_NONE;
			return true;
		}
		void fini() {
			if(InterlockedCompareExchange(&state, RPC_SERVER_STOPPING, RPC_SERVER_FAILED)==RPC_SERVER_FAILED) {
				while(state==RPC_SERVER_STOPPING) ATLAS_SLEEP(100);
			}
			if(state==RPC_SERVER_CONNECTED) {
				Disconnect(hConn);
				while(state==RPC_SERVER_CONNECTED) ATLAS_SLEEP(100);
			}
			if(state==RPC_SERVER_DISCONNECTED) CloseConn(hConn);
			A_MUTEX_DESTROY(&lock);
		}
		HCONNECT Lock() {
			A_MUTEX_LOCK(&lock);
			if(state==RPC_SERVER_CONNECTED) return hConn;
			if(state==RPC_SERVER_DISCONNECTED) {
				CloseConn(hConn);
				state = RPC_SERVER_NONE;
			}
			if(state==RPC_SERVER_NONE) {
				state = RPC_SERVER_CONNECTING;
				if(Connect()) {
					while(state==RPC_SERVER_CONNECTING) ATLAS_SLEEP(100);
					if(state==RPC_SERVER_CONNECTED) return hConn;
				} else {
					state = RPC_SERVER_NONE;
				}
			}
			A_MUTEX_UNLOCK(&lock);
			return NULL;
		}
		bool Connect();
	};

	struct RPC_ENGINE {
		TURBO_ALLOCATOR<HIOPOOL> allocator;
		HWORKERS hworkers;
		A_MUTEX maplock;
		std::map<_U64, RPC_SERVER*> servers;

		RPC_ENGINE() {
			ASockIOInit();
			A_MUTEX_INIT(&maplock);
			hworkers = CreateWorkers(1);
			ATLAS_ASSERT(hworkers);
		}
		~RPC_ENGINE() {
			for(std::map<_U64, RPC_SERVER*>::const_iterator citer=servers.begin(); citer!=servers.end(); ++citer) {
				if(citer->second) {
					citer->second->fini();
					ATLAS_ALIGN_FREE(citer->second);
				}
			}
			KillWorkers(hworkers);
			A_MUTEX_DESTROY(&maplock);
			ASockIOFini();
		}
		RPC_SERVER* Get(_U32 ip, _U16 port) {
			RPC_SERVER* *ret;
			_U64 key = ip;
			key <<= 32;
			key += port;
			A_MUTEX_LOCK(&maplock);
			try {
				ret = &servers[key];
			} catch(...) {
				ret = NULL;
			}
			if(ret) {
				if(!*ret) {
					*ret = (RPC_SERVER*)ATLAS_ALIGN_ALLOC(sizeof(RPC_SERVER));
					if(*ret) {
						if(!(*ret)->init(ip, port)) {
							(*ret)->fini();
							ATLAS_ALIGN_FREE(*ret);
							*ret = NULL;
						}
					}
				}
			}
			A_MUTEX_UNLOCK(&maplock);
			return (ret ? *ret : NULL);
		}
	};

	static RPC_ENGINE theRpcEngine;

	bool RPCClientOnConnect(HCONNECT hConn)
	{
		RPC_SERVER* srv = (RPC_SERVER*)KeyOf(hConn);
		srv->hConn = hConn;
		srv->state = RPC_SERVER_CONNECTED;
		srv->curlen = 0;
		_U8* buf = (_U8*)theRpcEngine.allocator.Alloc(RPC_PACKET_OVERHEAD+sizeof(_U16));
		*((_U32*)(buf+0)) = sizeof(_U16);
		*((_U16*)(buf+4)) = (_U16)-1;
		*((_U16*)(buf+6)) = (_U16)-1;
		*((_U16*)(buf+8)) = GetRPCBindPort();
		Send(hConn, RPC_PACKET_OVERHEAD+sizeof(_U16), buf);
		return true;
	}

	void RPCClientOnDisconnect(HCONNECT hConn)
	{
		((RPC_SERVER*)KeyOf(hConn))->state = RPC_SERVER_DISCONNECTED;
	}

	void RPCClientOnData(HCONNECT hConn, _U32 len, const _U8* buf)
	{
		((RPC_SERVER*)KeyOf(hConn))->OnData(len, buf);
	}

	void RPCClientOnConnectFailed(RPC_SERVER* srv)
	{
		if(srv->state==RPC_SERVER_STOPPING) {
			srv->state = RPC_SERVER_NONE;
			return;
		}
		if(srv->state==RPC_SERVER_CONNECTING) {
			srv->state = RPC_SERVER_FAILED;
		}
		if(!srv->Connect()) {
			srv->state = RPC_SERVER_NONE;
		}
	}

	struct RPC_CLIENT {
		HCONNECT hConn;
		HSERVER hserver;
		_U32 curlen;
		RPC_PACKET packet;

		bool MakeCall(_U16 iid, _U16 fid, _U32 len, const _U8* buf) {
			if(iid==(_U16)-1 && fid==(_U16)-1) {
				ATLAS_ASSERT(len==2);
				hserver = GetRPCServer(GetRPCClientAddr(this), *((_U16*)buf));
				return true;
			} else if(iid==(_U16)-1 && fid==0xfffe) {
				RPC_OUTPUT_BUF rob(RPC_PACKET_OVERHEAD+_buffer_alignof(len));
				rob.Serialize(len, buf);
				rob.SetID((_U16)-1, 0xfffe);
				rob.SendTo(this);
				return true;
			}
			if(iid>=RPC_IFTBL_SIZE || fid>=lwrpc_interface_table[iid].fcount) return false;
			RPC_INPUT_BUF rib(len, buf);
			return lwrpc_interface_table[iid].func_tbl[fid]((HCLIENT)this, rib);
		}
		bool MakeCall(RPC_PACKET* pp) {
			return MakeCall(pp->iid, pp->fid, pp->len, pp->buf);
		}
		bool MakeCall() {
			return MakeCall(packet.iid, packet.fid, packet.len, packet.buf);
		}
		void OnData(_U32 len, const _U8* buf) {
			if(!IsConnected(hConn)) return;
			do {
				if(curlen) {
					_U32 cpylen;
					if(curlen<RPC_PACKET_OVERHEAD) {
						cpylen = RPC_PACKET_OVERHEAD - curlen;
						if(len<cpylen) {
							cpylen = len;
						}
						memcpy(((_U8*)&packet)+curlen, buf, cpylen);
						curlen += cpylen;
						len -= cpylen;
						buf += cpylen;
					} else {
						if(packet.len>RPC_MAX_LEN) {
							Disconnect(hConn);
							return;
						}
						if(curlen==RPC_PACKET_OVERHEAD && len>=packet.len) {
							if(MakeCall(packet.iid, packet.fid, packet.len, buf)) {
								curlen = 0;
								len -= packet.len;
								buf += packet.len;
							} else {
								Disconnect(hConn);
								return;
							}
						} else {
							cpylen = RPC_PACKET_OVERHEAD + packet.len - curlen;
							if(len<cpylen) {
								cpylen = len;
							}
							memcpy(((_U8*)&packet)+curlen, buf, cpylen);
							curlen += cpylen;
							len -= cpylen;
							buf += cpylen;
							if(curlen==RPC_PACKET_OVERHEAD+packet.len) {
								if(MakeCall()) {
									curlen = 0;
								} else {
									Disconnect(hConn);
									return;
								}
							}
						}
					}
				} else {
					if(len<RPC_PACKET_OVERHEAD) {
						memcpy(&packet, buf, len);
						curlen = len;
						return;
					} else {
						RPC_PACKET* pp = (RPC_PACKET*)buf;
						if(pp->len>RPC_MAX_LEN) {
							Disconnect(hConn);
							return;
						}
						_U32 tlen = RPC_PACKET_OVERHEAD + pp->len;
						if(len>=tlen) {
							if(MakeCall(pp)) {
								len -= tlen;
								buf += tlen;
							} else {
								Disconnect(hConn);
								return;
							}
						} else {
							memcpy(&packet, buf, len);
							curlen = len;
							return;
						}
					}
				}
			} while(len);
		}
	};

	 struct RPC_MANAGER {
		HWORKERS hworkers;
		HTCPEP hep;
		A_MUTEX maplock;
		std::map<HCONNECT, RPC_CLIENT*> conns;
		SOCKADDR sockaddr;

		bool Add(HCONNECT hConn, RPC_CLIENT* pclt) {
			bool res = true;
			A_MUTEX_LOCK(&maplock);
			try {
				conns[hConn] = pclt;
			} catch(...) {
				res =false;
			}
			A_MUTEX_UNLOCK(&maplock);
			return res;
		}
		void Del(HCONNECT hConn) {
			A_MUTEX_LOCK(&maplock);
			conns.erase(hConn);
			A_MUTEX_UNLOCK(&maplock);
		}
		bool init(_U32, _U16, _U32);
		void fini();
	};

	static RPC_MANAGER theRpcManager;

	bool RPCServerOnConnect(HCONNECT hConn)
	{
		RPC_CLIENT* pclt = (RPC_CLIENT*)ATLAS_ALIGN_ALLOC(sizeof(RPC_CLIENT));
		if(!pclt) return false;
		if(!theRpcManager.Add(hConn, pclt)) {
			ATLAS_ALIGN_FREE(pclt);
			return false;
		}
		pclt->hConn = hConn;
		pclt->hserver = NULL;
		pclt->curlen = 0;
		SetKey(hConn, pclt);
		return true;
	}

	void RPCServerOnDisconnect(HCONNECT hConn)
	{
		RPC_CLIENT* pclt = (RPC_CLIENT*)KeyOf(hConn);
		if(pclt) {
			theRpcManager.Del(hConn);
			ATLAS_ALIGN_FREE(pclt);
		}
		CloseConn(hConn);
	}

	void RPCServerOnData(HCONNECT hConn , _U32 len, const _U8* buf)
	{
		((RPC_CLIENT*)KeyOf(hConn))->OnData(len, buf);
	}

	bool RPC_MANAGER::init(_U32 ip, _U16 port, _U32 tcnt)
	{
		hworkers = NULL;
		hep = NULL;
		A_MUTEX_INIT(&maplock);
		hworkers = CreateWorkers(tcnt);
		if(!hworkers) return false;
		static ASOCKIO_HANDLER handler = {
			RPCServerOnConnect,
			RPCServerOnDisconnect,
			RPCServerOnData,
			NULL
		};
		sockaddr.ip = ip;
		sockaddr.port = port;
		hep = NewEP(sockaddr, handler, theRpcEngine.allocator.GetMaxPool(), hworkers, this);
		if(hep) {
			StartEP(hep);
			GetEpAddr(hep, sockaddr);
			return true;
		}
		return false;
	}

	void RPC_MANAGER::fini()
	{
		if(hep) {
			StopEP(hep);
			do {
				A_MUTEX_LOCK(&maplock);
				for(std::map<HCONNECT, RPC_CLIENT*>::const_iterator citer=conns.begin(); citer!=conns.end(); ++citer) {
					Disconnect(citer->first);
				}
				A_MUTEX_UNLOCK(&maplock);
				ATLAS_SLEEP(100);
			} while(!conns.empty());
			DelEP(hep);
			hep = NULL;
		}
		if(hworkers) {
			KillWorkers(hworkers);
			hworkers = NULL;
		}
		A_MUTEX_DESTROY(&maplock);
	}

	RPC_OUTPUT_BUF::RPC_OUTPUT_BUF(size_t size)
	{
		packet = (RPC_PACKET*)theRpcEngine.allocator.Alloc(size);
		if(packet) {
			packet_size = size;
			packet->len = 0;
		}
	}

	RPC_OUTPUT_BUF::~RPC_OUTPUT_BUF()
	{
		if(packet) {
			theRpcEngine.allocator.Free(packet);
		}
	}

	RPC_RESULT RPC_OUTPUT_BUF::SendTo(HSERVER hserver)
	{
		HCONNECT hConn = ((RPC_SERVER*)hserver)->Lock();
		if(hConn) {
			Send(hConn, RPC_PACKET_OVERHEAD+packet->len, (_U8*)packet);
			packet = NULL;
			A_MUTEX_UNLOCK(&((RPC_SERVER*)hserver)->lock);
			return RPC_RES_NONE;
		}
		return RPC_RES_SNA;
	}

	RPC_RESULT RPC_OUTPUT_BUF::SendTo(HCLIENT hclient)
	{
		Send(((RPC_CLIENT*)hclient)->hConn, RPC_PACKET_OVERHEAD+packet->len, (_U8*)packet);
		packet = NULL;
		return RPC_RES_NONE;
	}

	bool StartRPCService(_U32 ip, _U16 port, _U32 tcnt)
	{
		return theRpcManager.init(ip, port, tcnt);
	}

	void StopRPCService()
	{
		theRpcManager.fini();
	}

	_U32 GetRPCClientAddr(HCLIENT hClient)
	{
		SOCKADDR sain;
		ATLAS_VERIFY(GetPeerAddr(((RPC_CLIENT*)hClient)->hConn, sain));
		return sain.ip;
	}

	_U32 GetRPCServerAddr(HSERVER hServer)
	{
		return ((RPC_SERVER*)hServer)->sain.ip;
	}

	_U16 GetRPCServerPort(HSERVER hServer)
	{
		return ((RPC_SERVER*)hServer)->sain.port;
	}

	LWRPC_IF lwrpc_interface_table[RPC_IFTBL_SIZE] = {0};
	LWRPC_CBIF lwrpc_cbinterface_table[RPC_IFTBL_SIZE] = {0};

	bool RPC_SERVER::Connect()
	{
		static ASOCKIO_HANDLER handler = {
			RPCClientOnConnect,
			RPCClientOnDisconnect,
			RPCClientOnData,
			(PFN_ON_CONNECTFAILED)RPCClientOnConnectFailed
		};
		return Atlas::Connect(sain, handler, theRpcEngine.allocator.GetMaxPool(), theRpcEngine.hworkers, this);
	}

	HSERVER GetRPCServer(const _STR ep)
	{
		SOCKADDR sa;
		if(!STR2ADDR(ep, sa)) return NULL;
		return GetRPCServer(sa.ip, sa.port);
	}

	HSERVER GetRPCServer(_U32 ip, _U16 port)
	{
		if(ip==0 || ip==0xffffffff ||port==0) return NULL;
		return theRpcEngine.Get(ip, port);
	}

	HSERVER GetRPCServer(HCLIENT hclient)
	{
		return hclient->hserver;
	}

	HSERVER GetRPCServer(HCLIENT hclient, _U16 port)
	{
		return GetRPCServer(GetRPCClientAddr(hclient), port);
	}

	static HSERVER _global_local_rpc = NULL;
	HSERVER GetLocalRPCServer()
	{
		if(!_global_local_rpc) {
			if(!theRpcManager.hep) return NULL;
			_global_local_rpc = theRpcEngine.Get(GetRPCBindIp()==0?0x0100007f:GetRPCBindIp(), GetRPCBindPort());
		}
		return _global_local_rpc;
	}

	_U32 GetRPCBindIp()
	{
		return theRpcManager.hep?theRpcManager.sockaddr.ip:0xffffffff;
	}

	_U16 GetRPCBindPort()
	{
		return theRpcManager.hep?theRpcManager.sockaddr.port:0;
	}

}
