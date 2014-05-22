#ifndef __ZION_SOCKET__
#define __ZION_SOCKET__

#ifdef __cplusplus
extern "C" {
#endif

typedef int		ASOCK_HANDLE;

#define ASOCK_READ			(1<<1)
#define ASOCK_WRITE			(1<<2)
#define ASOCK_ACCEPT			ASOCK_READ
#define ASOCK_REUSEADDR		(1<<3)
#define ASOCK_WAIT			(1<<4)
#define ASOCK_NONBLOCK		(1<<5)

#define ASOCK_INVALID_HANDLE	(-1)
#define ASOCK_LINEEND		("\r\n")
#define ASOCK_ERROR			(-1)
#define ASOCK_INFINITE		(-1)

typedef struct ASOCK_ADDR {
	unsigned int	ip;
	unsigned short	port;
} ASOCK_ADDR;

typedef struct ASOCK_TCP_OPTION {
	int		recvbuf;
	int		sndbuf;
	int		keepalive;
	int		keepalive_probes;
	int		keepalive_time;
	int		keepalive_intvl;
	int		linger;
} ASOCK_TCP_OPTION;

typedef struct ASOCK_UDP_OPTION {
	int		recvbuf;
	int		sndbuf;
	int		broadcast;
} ASOCK_UDP_OPTION;

void sock_init();
void sock_final();

char* sock_addr2str(const ASOCK_ADDR* addr, char* str);
ASOCK_ADDR* sock_str2addr(const char* str, ASOCK_ADDR* addr);
void sock_addr(ASOCK_ADDR* addr, unsigned int ip, unsigned short port);

int sock_nonblock(ASOCK_HANDLE fd);
void sock_default_tcp_option(ASOCK_TCP_OPTION* option);
void sock_default_udp_option(ASOCK_UDP_OPTION* option);
int sock_set_tcp_option(ASOCK_HANDLE handle, const ASOCK_TCP_OPTION* option);
int sock_set_udp_option(ASOCK_HANDLE handle, const ASOCK_UDP_OPTION* option);

int sock_wait_read(ASOCK_HANDLE handle, int timeout);
int sock_wait_write(ASOCK_HANDLE handle, int timeout);
int sock_wait_error(ASOCK_HANDLE handle, int timeout);

ASOCK_HANDLE sock_bind(ASOCK_ADDR* addr, int flags);
int sock_unbind(ASOCK_HANDLE fd);

ASOCK_HANDLE sock_accept(ASOCK_HANDLE fd, ASOCK_ADDR* addr);
ASOCK_HANDLE sock_connect(const ASOCK_ADDR* addr, int flags);
int sock_disconnect(ASOCK_HANDLE fd);
void sock_close(ASOCK_HANDLE fd);

int sock_peername(ASOCK_HANDLE fd, ASOCK_ADDR* addr);
int sock_sockname(ASOCK_HANDLE fd, ASOCK_ADDR* addr);

int sock_read(ASOCK_HANDLE fd, void* buf, int buf_len);
int sock_write(ASOCK_HANDLE fd, const void* buf, int buf_len);
int sock_readbuf(ASOCK_HANDLE fd, void* buf, int buf_len);
int sock_writebuf(ASOCK_HANDLE fd, const void* buf, int buf_len);

int sock_readline(ASOCK_HANDLE fd, char* buf, int buf_len);
int sock_writeline(ASOCK_HANDLE fd, const char* buf);

ASOCK_HANDLE sock_dgram_bind(ASOCK_ADDR* endpoint, int broadcast);
int sock_dgram_unbind(ASOCK_HANDLE handle);
int sock_dgram_send(ASOCK_HANDLE handle, const ASOCK_ADDR* addr, const char* buf, int buf_len);
int sock_dgram_recv(ASOCK_HANDLE handle, ASOCK_ADDR* addr, char* buf, int buf_len);

#ifdef __cplusplus
}
#endif

#endif
