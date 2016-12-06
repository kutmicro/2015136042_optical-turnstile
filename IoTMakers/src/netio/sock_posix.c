/*
 * GiGA IoTMakers version 2
 *
 *  Copyright (c) 2016 kt corp. All rights reserved.
 *
 *  This is a proprietary software of kt corp, and you may not use this file except in
 *  compliance with license agreement with kt corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of kt corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 */


#if defined(_IM_C_SOCK_) && !defined(ARDUINO)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


#ifdef WIN32
#include <wing_sockfd2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif


#include "util/log.h"
#include "netio/sock.h"

static int g_connected;
static int g_sockfd;

typedef void (*IMCbRecvMsgHandler)(char *data, int data_len);

static IMCbRecvMsgHandler _cb_onMsgarrvd_hndl = NULL;;

void im_sock_set_cb_recv_handler(void* cb)
{
	_cb_onMsgarrvd_hndl = (IMCbRecvMsgHandler)cb;
}

static int _net_sock_getaddr (struct sockaddr *addr, int sock_type, const char *host, int port_num)
{
    struct addrinfo hints, *res;
	char	port[32];
 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = sock_type;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;
 
	sprintf(port, "%d", port_num);
    if (getaddrinfo(host, port, &hints, &res))
        return -1;
 
    if (res == NULL)
        return -1;
 
    memcpy(addr, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    return 0;
}

static int _net_sock_getaddr_udp (struct sockaddr *addr, const char *host, int port_num)
{
	return _net_sock_getaddr(addr, SOCK_DGRAM, host, port_num);
}
static int _net_sock_getaddr_tcp (struct sockaddr *addr, const char *host, int port_num)
{
	return _net_sock_getaddr(addr, SOCK_STREAM, host, port_num);
}


int im_sock_set_read_timeout(int sec)
{
	struct timeval timeout;      
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	if (setsockopt (g_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		INFO_LOG("fail setsockopt SO_RCVTIMEO, g_sockfd=[%d]", g_sockfd);

	return 0;
}

int im_sock_set_write_timeout(int sec)
{
	struct timeval timeout;      
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	if (setsockopt (g_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		INFO_LOG("fail setsockopt SO_SNDTIMEO, g_sockfd=[%d]", g_sockfd);
	
	return 0;
}

int im_sock_set_timeout(int sec)
{
	im_sock_set_read_timeout(sec);
	im_sock_set_write_timeout(sec);

	return 0;
}

int im_sock_connect_timeout(char *remoteip, unsigned short port, int timeout_sec) 
{
	g_sockfd = -1;
	struct sockaddr_in remote_addr;

#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsa_data;

	WSAStartup(wVersionRequested, &wsa_data);
#endif

	g_sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( g_sockfd < 0 ) {
		ERROR_LOG("fail socket()");
		return -1;
	}

	im_sock_set_timeout(timeout_sec);

    if ( _net_sock_getaddr_tcp((struct sockaddr *)&remote_addr, remoteip, port) < 0 ) {
		ERROR_LOG("fail net_g_sockfd_getaddr_tcp(%s:%d)", remoteip, port);
		return -1;
	}

	INFO_LOG("connecting %s:%d", remoteip, port);
	int rc = connect(g_sockfd, (const struct sockaddr *)&remote_addr, sizeof(remote_addr));
	if ( rc ) {
		ERROR_LOG("fail connect(%s:%d)", remoteip, port);
		return -1;
	}

	g_connected = (1);

	return g_sockfd;
}

int im_sock_connect(char *ip, unsigned short port)
{
	return im_sock_connect_timeout(ip, port, 0);
}


int im_sock_disconnect()
{
	if ( g_sockfd <= 0 )	{
		return -1;
	}

#ifdef WIN32
	closesocket(g_sockfd); 
	WSACleanup();
#else
	close(g_sockfd);
#endif

	g_connected = (0);

	return 0;
}


int im_sock_connected()
{
	return g_connected;
}


void im_sock_flush()
{
	int nread;
	char receive[8];
	while( (im_sock_available()) && (nread = read(g_sockfd, receive, sizeof(receive)-1)) > 0) {
		INFO_LOG("g_sockfd=[%d]", g_sockfd);
		;
	}
}

/* ===============================
 * g_sockfd_available
 * > 0 : on read occured
 * = 0 : timeout
 * < 0 : select error
 * =============================== */
int im_sock_available()
{
    struct timeval tv;
    fd_set readfds;
    int state;

	if (g_sockfd < 0) return -1;

	FD_ZERO(&readfds);
	FD_SET(g_sockfd, &readfds);

/*
	tv.tv_sec = timeout_sec;
	tv.tv_usec = 0;
*/
	tv.tv_sec = 0;
	tv.tv_usec = 100;

	
	if ( tv.tv_sec <= 0 && tv.tv_usec <= 0) {
		state = select(g_sockfd+1, &readfds, (fd_set *)0, (fd_set *)0, (struct timeval*)0);
	}else{
		state = select(g_sockfd+1, &readfds, (fd_set *)0, (fd_set *)0, &tv);
	};

	if ( state < 0 ) {
		/* error on select */
		ERROR_LOG("g_sockfd_availavle=[%d]", state);
		perror("g_sockfd_availavle():");
		return -1;
	} else 	if ( state == 0 ){
		/* 
		ERROR_LOG("no data to read; state=[%d]", state);
		read timeout */
		return 0;
	};

	if ( FD_ISSET(g_sockfd, &readfds)){
		/* 
		DEBUG_LOG("data to read; state=[%d]", state);
		read event may occured */
		return 1;
	} else {
		ERROR_LOG("g_sockfd_availavle=[%d]", state);
		return -1;
	};
}


int im_sock_send(char *data, int data_len)
{
	if ( g_sockfd < 0 || g_sockfd > 32767 )	{
		INFO_LOG("invalid g_sockfd=[%d]", g_sockfd);
		sleep(1);
		return -1;
	}

	int nsend = send(g_sockfd, data, data_len, 0);
	if ( nsend < 0 )	{
		if (errno == EWOULDBLOCK || errno == EINTR) {
			return 0;
		} else {
			ERROR_LOG("no data sent; nsend=[%d]", nsend);
			return -1;
		}
	} else if ( nsend == 0 ) {
		ERROR_LOG("remote host may be closed");
		return 0;
	}
	
	return nsend;
}



int im_sock_recv(char *o_buff, int len)
{
	int lenRead = 0;

#if 1
	for (lenRead=0; lenRead<len; lenRead++ )	
	{
		if ( im_sock_available() <= 0 )		{
			unsigned int g_tickSec = im_util_get_secs();
			while(im_sock_available() <= 0) {
				unsigned int currentSecs = im_util_get_secs();
				if (currentSecs - g_tickSec >= IM_SOCKET_TIMEOUT ){
					ERROR_LOG("timeout im_sock_recv");
					return -1;
				}
			}
		}
		
		recv(g_sockfd, &o_buff[lenRead], 1, 0);
	}
	return lenRead;

#else
	if ( g_sockfd < 0 || g_sockfd > 32767 )	{
		INFO_LOG("invalid g_sockfd=[%d]", g_sockfd);
		sleep(1);
		return -1;
	}

	int nread = recv(g_sockfd, o_buff, buff_len, 0);
	if ( nread < 0 )	{
		if (errno == EWOULDBLOCK || errno == EINTR) {
			return 0;
		} else {
			//DEBUG_LOG("no data received; nread=[%d]", nread);
			return -1;
		}
	} else if ( nread == 0 ) {
		ERROR_LOG("remote host may be closed");
		g_connected = (0);
		return -1;
	}
	
	//o_buff[nread] = '\0';
	return nread;
#endif
}


#endif //_IM_C_SOCK_