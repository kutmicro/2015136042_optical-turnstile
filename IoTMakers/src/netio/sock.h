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


#ifndef IOTMAKERS_SOCK_H
#define IOTMAKERS_SOCK_H


#define IM_SOCKET_TIMEOUT	3

#ifdef __cplusplus
extern "C"
{
#endif

int im_sock_connect(char *ip, unsigned short port) ;
int im_sock_disconnect();

void im_sock_set_cb_recv_handler(void* cb);
int im_sock_set_read_timeout(int sec);
int im_sock_set_write_timeout(int sec);
int im_sock_set_timeout(int sec);

int im_sock_connected();
void im_sock_flush();
int im_sock_available();

int im_sock_send(char *data, int data_len);
int im_sock_recv(char *o_buff, int buff_len);

#ifdef __cplusplus
}
#endif


#endif
