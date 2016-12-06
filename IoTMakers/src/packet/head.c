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


#include "packet/head.h"
#include "netio/sock.h"

// Length = Length info(4byte) + 35
const unsigned char head_01[] PROGMEM = {
  0x11, // Protocol Version
  0x01, // Message Header Type
  0x00, 0x23, // Message header Length 0x23 = 35
};

const unsigned char head_02_TypeDevAuth[] PROGMEM = {
  0x60, 0xe0, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};
const unsigned char head_02_TypeDevAuth_res[] = {
  0xa0, 0xe0, 
};

const unsigned char head_02_TypeReport[] PROGMEM = {
  0x61, 0x9b, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};
const unsigned char head_02_TypeReport_res[] = {
  0xa1, 0x9b, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};

const unsigned char head_02_TypeKeepAlive[] PROGMEM = {
  0x60, 0xe7, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};
const unsigned char head_02_TypeKeepAlive_res[] = {
  0xa0, 0xe7, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};

const unsigned char head_02_TypeCtrl_req[] = {
  0x62, 0x0d, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};
const unsigned char head_02_TypeAck[] PROGMEM = {
  0xa2, 0x0d, // Message Type(2bit) + MEP(2bit) + Method Type(12bit)
};


const unsigned char head_03[] PROGMEM = {
  0x00, 0x00, 0x01, 0x4e, 0x05, 0x5f, 0xdf, 0xce, // Transactio ID
};
const unsigned char head_04[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Channel Auth Token (16byte)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ..
};
const unsigned char head_05[] PROGMEM = {
  0x00, // Encryption Usage(1bit) + Encryption Method(7bit)
  0x00, // Compression Usage(1bit) + Compression method(7bit)
  0x03, // Encoding Type
  0x00, 0x00 // Result Code
};


int __head_send_P(char* PROGMEM data, int len)
{
	int sent = 0;
	char buff[8];
	char* p = (char*)data;
	unsigned char idx = 0;
	int i;

	for (i=0; i<len; i++)
	{
#if defined(ARDUINO)
		buff[idx++] = (char)pgm_read_byte_near(p++);
#else
		buff[idx++] = *p++;
#endif
		if ( idx == sizeof(buff) )	{
			sent += im_sock_send(buff, idx);
			idx=0;
		}
	};

	if ( idx != 0 )	{
		// send remain bytes.
		sent += im_sock_send(buff, idx);
		idx = 0;
	}

	return sent;
}


int head_send_auth_device()
{
	int sent = 0;
	sent += __head_send_P((char*)head_01, sizeof(head_01));
	sent += __head_send_P((char*)head_02_TypeDevAuth, sizeof(head_02_TypeDevAuth));
	sent += __head_send_P((char*)head_03, sizeof(head_03));
	sent += __head_send_P((char*)head_04, sizeof(head_04));
	sent += __head_send_P((char*)head_05, sizeof(head_05));
	return sent;
}
int head_send_keepalive()
{
	int sent = 0;
	sent += __head_send_P((char*)head_01, sizeof(head_01));
	sent += __head_send_P((char*)head_02_TypeKeepAlive, sizeof(head_02_TypeKeepAlive));
	sent += __head_send_P((char*)head_03, sizeof(head_03));
	sent += __head_send_P((char*)head_04, sizeof(head_04));
	sent += __head_send_P((char*)head_05, sizeof(head_05));
	return sent;
}
int head_send_collection(char* authCode)
{
	int sent = 0;
	sent += __head_send_P((char*)head_01, sizeof(head_01));
	sent += __head_send_P((char*)head_02_TypeReport, sizeof(head_02_TypeReport));
	sent += __head_send_P((char*)head_03, sizeof(head_03));
	sent += im_sock_send((char*)authCode, sizeof(head_04));
	sent += __head_send_P((char*)head_05, sizeof(head_05));
	return sent;
}

int head_send_control_ack(char* authCode)
{
	int sent = 0;
	sent += __head_send_P((char*)head_01, sizeof(head_01));
	sent += __head_send_P((char*)head_02_TypeAck, sizeof(head_02_TypeAck));
	sent += __head_send_P((char*)head_03, sizeof(head_03));
	sent += im_sock_send((char*)authCode, sizeof(head_04));
	sent += __head_send_P((char*)head_05, sizeof(head_05));
	return sent;
}


int head_is_TypeDevAuth_res(char* head)
{
	return (memcmp(head+4, head_02_TypeDevAuth_res, 2) == 0)?1:0;
}
int head_is_TypeKeepAlive_res(char* head)
{
	return (memcmp(head+4, head_02_TypeKeepAlive_res, 2) == 0)?1:0;
}
int head_is_TypeReport_res(char* head)
{
	return (memcmp(head+4, head_02_TypeReport_res, 2) == 0)?1:0;
}
int head_is_TypeCtrl_req(char* head)
{
	return (memcmp(head+4, head_02_TypeCtrl_req, 2) == 0)?1:0;
}

