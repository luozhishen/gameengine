#ifndef WITHOUT_ATLAS_ASYNCIO

#ifndef	__LWRPCIMP__
#define	__LWRPCIMP__

#ifndef	__cplusplus
#error	C++ compiler needed
#endif

#define	RPC_IFTBL_SIZE			256
#define RPC_MAX_LEN				(2*1024*1024)
#define	BUFFER_ALIGNMENT_TYPE	_U64
#define	BUFFER_ALIGNMENT		8
#define	_buffer_alignof(L)		(((L)&(BUFFER_ALIGNMENT-1))?(((L)&~(BUFFER_ALIGNMENT-1))+BUFFER_ALIGNMENT):(L))
#define	_aligned_sizeof(T)		_buffer_alignof(sizeof(T))

namespace Atlas {

	struct RPC_PACKET {
		_U32 len;
		_U16 iid, fid;
		_U8 buf[RPC_MAX_LEN];
	};

	#define	RPC_PACKET_OVERHEAD	(sizeof(_U32)+sizeof(_U16)*2)

	struct RPC_OUTPUT_BUF {
		RPC_PACKET* packet;
		size_t packet_size;

		RPC_OUTPUT_BUF(size_t size);
		~RPC_OUTPUT_BUF();
		RPC_RESULT SendTo(HSERVER);
		RPC_RESULT SendTo(HCLIENT);
		operator bool () const {
			return(packet!=NULL);
		}
		template <typename T> bool Serialize(const T& t) {
			if(packet->len+_aligned_sizeof(T)<=packet_size) {
				*(T *)(packet->buf+packet->len) = t;
				packet->len += _aligned_sizeof(T);
				return(true);
			}
			return(false);
		}
		template <typename T> bool Serialize(_U32 l, const T *tv) {
			l *= sizeof(T);
			_U32 alen = _buffer_alignof(l);
			if(packet->len+alen<=packet_size) {
				memcpy(packet->buf+packet->len, tv, l);
				packet->len += alen;
				return(true);
			}
			return(false);
		}
		void SetID(_U16 iid, _U16 fid) {
			packet->iid = iid;
			packet->fid = fid;
		}
	};

	struct RPC_INPUT_BUF {
		_U32 len, ptr;
		const _U8* buf;

		RPC_INPUT_BUF(_U32 l, const _U8* b) : len(l), ptr(0), buf(b) {}
		template <typename T> bool Serialize(T *& pt) {
			if(len-ptr>=_aligned_sizeof(T)) {
				pt = (T *)(buf+ptr);
				ptr += _aligned_sizeof(T);
				return(true);
			}
			return(false);
		}
		template <typename T> bool Serialize(_U32 l, T *& tv) {
			l *= sizeof(T);
			_U32 alen = _buffer_alignof(l);
			if(len-ptr>=alen) {
				tv = (T *)(buf+ptr);
				ptr += alen;
				return(true);
			}
			return(false);
		}
	};

	typedef bool (*RPC_FUNC_STUB)(HCLIENT, RPC_INPUT_BUF&);
	typedef RPC_FUNC_STUB *RPC_FUNC_TBL;
	typedef bool (*RPC_CBFUNC_STUB)(HSERVER, RPC_INPUT_BUF&);
	typedef RPC_CBFUNC_STUB *RPC_CBFUNC_TBL;

	struct LWRPC_IF {
		_U32 fcount;
		RPC_FUNC_TBL func_tbl;
	};

	struct LWRPC_CBIF {
		_U32 cbfcount;
		RPC_CBFUNC_TBL cbfunc_tbl;
	};

	extern LWRPC_IF lwrpc_interface_table[RPC_IFTBL_SIZE];
	extern LWRPC_CBIF lwrpc_cbinterface_table[RPC_IFTBL_SIZE];

}

#endif

#endif
