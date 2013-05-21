#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#else
#include <winsock2.h>
#endif

#include "ZionDefines.h"
#include "ZionSocket.h"

#ifndef WIN32
#define closesocket					close
#define SD_BOTH						SHUT_RD
#define WSAEINTR					EINTR
#define WSAGetLastError()			errno
#define WSAEWOULDBLOCK				EAGAIN

#else
#define socklen_t					int
#define MSG_NOSIGNAL				0
#endif

static int setreuseaddr(SOCK_HANDLE fd);

void sock_init()
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
}

void sock_final()
{
#ifdef WIN32
	WSACleanup();
#endif
}

char* sock_addr2str(const SOCK_ADDR* addr, char* str)
{
	sprintf(str, "%d.%d.%d.%d:%d",
		addr->ip&0xff,
		((addr->ip>>8)&0xff),
		((addr->ip>>16)&0xff),
		((addr->ip>>24)&0xff),
		(int)addr->port);
	return str;
}

SOCK_ADDR* sock_str2addr(const char* str, SOCK_ADDR* addr)
{
	char ip_str[30], *port_str, * flag_str;

	flag_str = (char*)strstr(str, ":");
	if(flag_str==NULL || (flag_str-str)>=sizeof(ip_str)) return NULL;

	memcpy(ip_str, str, flag_str-str);
	ip_str[flag_str-str] = '\0';
	port_str = flag_str+1;

	memset(addr, 0, sizeof(*addr));
	addr->ip = inet_addr(ip_str);
	addr->port = (u_short)atoi(port_str);

	return(addr->ip==INADDR_NONE?NULL:addr);
}

void sock_addr(SOCK_ADDR* addr, unsigned int ip, unsigned short port)
{
	memset(addr, 0, sizeof(*addr));
	addr->ip	= ip;
	addr->port	= port;
}

int sock_nonblock(SOCK_HANDLE fd)
{
#ifndef WIN32
	int flags;
	flags = fcntl(fd, F_GETFL);
	return(fcntl(fd, F_SETFL, flags|O_NONBLOCK)==0?0:errno);
#else
	int iMode = 1;
	return ioctlsocket(fd, FIONBIO, (u_long FAR*) &iMode)!=SOCKET_ERROR?0:WSAGetLastError();
#endif
}

void sock_default_tcp_option(SOCK_TCP_OPTION* option)
{
	option->recvbuf		= -1;
	option->sndbuf		= -1;
	option->keep_alive	= 0;
	option->linger		= -1;
}

void sock_default_udp_option(SOCK_UDP_OPTION* option)
{
	option->recvbuf		= -1;
	option->sndbuf		= -1;
	option->broadcast	= -1;
}

int sock_set_tcp_option(SOCK_HANDLE handle, const SOCK_TCP_OPTION* option)
{
	if(option->recvbuf!=-1) {
		setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *)&option->recvbuf, sizeof(option->recvbuf));
	}
	if(option->sndbuf!=-1) {
		setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *)&option->sndbuf, sizeof(option->sndbuf));
	}
	if(option->keep_alive) {
		setsockopt(handle, SOL_SOCKET, SO_KEEPALIVE, (const char *)&option->keep_alive, sizeof(option->keep_alive));
	}
/*	if(option->linger!=-1) {
		LINGER linger;
		linger.l_onoff = 1;
		linger.l_linger = option->linger;
		setsockopt(handle, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));
	}*/
	return 0;
}

int sock_set_udp_option(SOCK_HANDLE handle, const SOCK_UDP_OPTION* option)
{
	if(option->recvbuf!=-1) {
		setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *)&option->recvbuf, sizeof(option->recvbuf));
	}
	if(option->sndbuf!=-1) {
		setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *)&option->sndbuf, sizeof(option->sndbuf));
	}
	if(option->broadcast) {
		setsockopt(handle, SOL_SOCKET, SO_BROADCAST, (const char *)&option->broadcast, sizeof(option->broadcast));
	}
	return 0;
}

int sock_wait_read(SOCK_HANDLE handle, int timeout)
{
#ifdef _WIN32
	int ret;
	struct fd_set fds;
	FD_ZERO(&fds);
	FD_SET((SOCKET)handle, &fds);
	if(timeout==SOCK_INFINITE) {
		ret = select(0, &fds, NULL, NULL, NULL);
	} else {
		struct timeval tv;
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ret = select(0, &fds, NULL, NULL, &tv);
	}
	if(ret==SOCKET_ERROR) return -1;
	return FD_ISSET(handle, &fds)?1:0;
#else
	int ret;
	struct pollfd ufds;
	ufds.fd = handle;
	ufds.events = POLLIN;
	ufds.revents = 0;
	ret = poll(&ufds, 1, timeout==SOCK_INFINITE?-1:timeout);
	if(ret==-1) return -1;
	return ufds.revents?1:0;
#endif
}

int sock_wait_write(SOCK_HANDLE handle, int timeout)
{
#ifdef _WIN32
	int ret;
	struct fd_set fds;
	FD_ZERO(&fds);
	FD_SET((SOCKET)handle, &fds);
	if(timeout==SOCK_INFINITE) {
		ret = select(0, NULL, &fds, NULL, NULL);
	} else {
		struct timeval tv;
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ret = select(0, NULL, &fds, NULL, &tv);
	}
	if(ret==SOCKET_ERROR) return -1;
	return FD_ISSET(handle, &fds)?1:0;
#else
	int ret;
	struct pollfd ufds;
	ufds.fd = handle;
	ufds.events = POLLOUT;
	ufds.revents = 0;
	ret = poll(&ufds, 1, timeout<=0?0:timeout);
	if(ret==-1) return -1;
	return ufds.revents?1:0;
#endif
}

int sock_wait_error(SOCK_HANDLE handle, int timeout)
{
#ifdef _WIN32
	int ret;
	struct fd_set fds;
	FD_ZERO(&fds);
	FD_SET((SOCKET)handle, &fds);
	if(timeout==SOCK_INFINITE) {
		ret = select(0, NULL, NULL, &fds, NULL);
	} else {
		struct timeval tv;
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ret = select(0, NULL, NULL, &fds, &tv);
	}
	if(ret==SOCKET_ERROR) return -1;
	return FD_ISSET(handle, &fds)?1:0;
#else
	int ret;
	struct pollfd ufds;
	ufds.fd = handle;
	ufds.events = POLLERR;
	ufds.revents = 0;
	ret = poll(&ufds, 1, timeout<=0?0:timeout);
	if(ret==-1) return -1;
	return ufds.revents?1:0;
#endif
}

SOCK_HANDLE sock_bind(SOCK_ADDR* addr, int flags)
{
	struct sockaddr_in sa;
	SOCK_HANDLE sock;
	socklen_t sa_len;

	sa.sin_family       = AF_INET;
	sa.sin_addr.s_addr  = addr->ip;
	sa.sin_port         = htons(addr->port);

	if((sock=(SOCK_HANDLE)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1)
		return(SOCK_INVALID_HANDLE);
	if(flags&SOCK_REUSEADDR && setreuseaddr(sock)!=0) {
		closesocket(sock);
		return(SOCK_INVALID_HANDLE);
	}
	if(bind(sock, (struct sockaddr*)&sa, sizeof(sa))!=0) {
		closesocket(sock);
		return(SOCK_INVALID_HANDLE);
	}
	if(listen(sock, SOMAXCONN)!=0) {
		closesocket(sock);
		return(SOCK_INVALID_HANDLE);
	}
	sa_len = sizeof(sa);
	if(getsockname(sock, (struct sockaddr*)&sa, &sa_len)!=0) {
		closesocket(sock);
		return(SOCK_INVALID_HANDLE);
	}
	if(flags&SOCK_NONBLOCK) {
		sock_nonblock(sock);
	}

	addr->ip = *((unsigned int*)&sa.sin_addr);
	addr->port = ntohs(sa.sin_port);

	return sock;
}

int sock_unbind(SOCK_HANDLE fd)
{
	closesocket(fd);
	return 0;
}

SOCK_HANDLE sock_accept(SOCK_HANDLE fd, SOCK_ADDR* addr)
{
	SOCK_HANDLE sock;
	struct sockaddr_in sa;
	socklen_t sa_len;
	for(;;) {
		sa_len = sizeof(sa);
		sock = (SOCK_HANDLE)accept(fd, (struct sockaddr*)&sa, &sa_len);
		if(sock!=-1) break;
		if(sock==-1 && WSAGetLastError()!=WSAEINTR) return SOCK_INVALID_HANDLE;
	}
	if(addr!=NULL) {
		addr->ip	= sa.sin_addr.s_addr;
		addr->port	= ntohs(sa.sin_port);
	}
	return sock;
}

SOCK_HANDLE sock_connect(const SOCK_ADDR* addr, int flags)
{
	struct sockaddr_in sa;
	SOCK_HANDLE sock;

	sa.sin_family       = AF_INET;
	sa.sin_addr.s_addr  = addr->ip;
	sa.sin_port         = htons(addr->port);

	if((sock=(SOCK_HANDLE)socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))==-1)
		return(SOCK_INVALID_HANDLE);
	if(flags&SOCK_NONBLOCK)
		sock_nonblock(sock);
	if(connect(sock, (struct sockaddr*)&sa, sizeof(sa))!=0) {
		if((flags&SOCK_NONBLOCK)==0 || WSAGetLastError()!=WSAEWOULDBLOCK) {
			closesocket(sock);
			return(SOCK_INVALID_HANDLE);
		}
	}

    return(sock);
}

int sock_disconnect(SOCK_HANDLE fd)
{
	return shutdown(fd, SD_BOTH)==-1?WSAGetLastError():0;
}

void sock_close(SOCK_HANDLE fd)
{
	closesocket(fd);
}

int sock_peername(SOCK_HANDLE fd, SOCK_ADDR* addr)
{
	struct sockaddr_in sa;
	socklen_t sa_len;

	sa_len = sizeof(sa);
	if(getpeername(fd, (struct sockaddr*)&sa, &sa_len)!=0) {
		return -1;
	}

	sock_addr(addr, sa.sin_addr.s_addr, ntohs(sa.sin_port));
	return 0;
}

int sock_sockname(SOCK_HANDLE fd, SOCK_ADDR* addr)
{
	struct sockaddr_in sa;
	socklen_t sa_len;

	sa_len = sizeof(sa);
	if(getsockname(fd, (struct sockaddr*)&sa, &sa_len)!=0) {
		return -1;
	}

	sock_addr(addr, sa.sin_addr.s_addr, ntohs(sa.sin_port));
	return 0;
}

int sock_read(SOCK_HANDLE fd, void* buf, int buf_len)
{
	int ret;

	for(;;) {
		ret = recv(fd, (char*)buf, buf_len, 0);
		if(ret>=0) return ret;
		switch(WSAGetLastError()) {
		case WSAEINTR: break;
		case WSAEWOULDBLOCK: return 0;
		default: return -1;
		}
	}
}

int sock_write(SOCK_HANDLE fd, const void* buf, int buf_len)
{
	int ret;

	for(;;) {
		ret = send(fd, (const char*)buf, buf_len, 0);
		if(ret>=0) return ret;
		switch(WSAGetLastError()) {
		case WSAEINTR: break;
		case WSAEWOULDBLOCK: return 0;
		default: return -1;
		}
	}
}

int sock_readbuf(SOCK_HANDLE fd, void* buf, int buf_len)
{
	int ret, size;

	for(size=0; size<buf_len; size+=ret) {
		ret = recv(fd, (char*)buf+size, buf_len-size, 0);
		if(ret==0) return -1;
		if(ret>0) break;
		if(WSAGetLastError()!=WSAEINTR) return -1;
	}

	return 0;
}

int sock_writebuf(SOCK_HANDLE fd, const void* buf, int buf_len)
{
	int ret, size;

	for(size=0; size<buf_len; size+=ret) {
		ret = send(fd, (const char*)buf+size, buf_len-size, 0);
		if(ret==0) return -1;
		if(ret>0) break;
		if(WSAGetLastError()!=WSAEINTR) return -1;
	}

	return 0;
}

int sock_readline(SOCK_HANDLE fd, char* buf, int buf_len)
{
	int rlen, ret;

	rlen = 0;

	for(;;) {
		ret = recv(fd, buf+rlen, 1, 0);
		if(ret<0 && WSAGetLastError()==WSAEINTR) continue;
		if(ret<=0) return -1;

		rlen ++;

		if(rlen>=2 && buf[rlen-2]=='\r' && buf[rlen-1]=='\n') {
			buf[rlen-2] = '\0';
			return 0;
		}

		if(rlen==buf_len) return -1;
	}
}

int sock_writeline(SOCK_HANDLE fd, const char* buf)
{
	int ret;
	ret = sock_writebuf(fd, buf, (int)strlen(buf));
	if(ret!=0) return ret;
	ret = sock_writebuf(fd, "\r\n", 2);
	return ret;
}

SOCK_HANDLE sock_dgram_bind(SOCK_ADDR* ep, int broadcast)
{
	struct sockaddr_in sa;
	socklen_t sa_len;
	SOCK_HANDLE sock;

	sa.sin_family		= AF_INET;
	sa.sin_addr.s_addr	= ep->ip;
	sa.sin_port			= htons(ep->port);

	if((sock=(SOCK_HANDLE)socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) return(-1);
	if(bind(sock, (struct sockaddr*)&sa, sizeof(sa))!=0) { closesocket(sock); return(-1); }
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast))!=0) { closesocket(sock); return(-1); }
	sa_len = sizeof(sa);
	if(getsockname(sock, (struct sockaddr*)&sa, &sa_len)!=0) { closesocket(sock); return(-1); }
	ep->ip = sa.sin_addr.s_addr;
	ep->port = sa.sin_port;

	//done
	return(sock);
}

int sock_dgram_unbind(SOCK_HANDLE handle)
{
	shutdown(handle, SD_BOTH);
	closesocket(handle);
	return(0);
}

int sock_dgram_send(SOCK_HANDLE handle, const SOCK_ADDR* addr, const char* buf, int len)
{
	struct sockaddr_in sa;
	int ret;
	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = addr->ip;
	sa.sin_port = htons(addr->port);

	for(;;) {
		ret = sendto(handle, buf, len, 0, (struct sockaddr*)&sa, sizeof(sa));
		if(ret<0) {
			if(ret==WSAEINTR) continue;
			return(-1);
		} else {
			return(0);
		} 
	}
}

int sock_dgram_recv(SOCK_HANDLE handle, SOCK_ADDR* addr, char* buf, int len)
{
	struct sockaddr_in sa;
	socklen_t sa_len;
	int ret;

	for(;;) {
		sa_len = sizeof(sa);
		ret = recvfrom(handle, buf, len, 0, (struct sockaddr*)&sa, (socklen_t*)&sa_len);
		if(ret<0) {
			if(ret==WSAEINTR) continue;
			return(-1);
		} else {
			addr->ip = sa.sin_addr.s_addr;
			addr->port = ntohs(sa.sin_port);
			return(ret);
		} 
	}
}

int setreuseaddr(SOCK_HANDLE fd)
{
#ifndef WIN32
	int opt = 1;
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))==0?0:errno;
#else
	BOOL val = TRUE;
	return setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(char *)&val,sizeof(val))==0?0:WSAGetLastError();
#endif
}

