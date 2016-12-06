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

#if defined(ARDUINO)

#include <Arduino.h>
#include <Client.h>
#include "util/log.h"
#include "netio/sock_Arduino.h"
#include "netio/sock.h"

Client* g_client = NULL;

void im_sock_set_Arduino_client(Client* client)
{
	g_client = client;
}

int im_sock_connect(char *ip, unsigned short port)
{
	unsigned int flds[4];
	unsigned long ipaddr;

	Serial.print(F("connecting "));Serial.print(ip);Serial.print(F(":"));Serial.println(port);

	if(sscanf(ip,"%u.%u.%u.%u",flds,flds+1,flds+2,flds+3)==4){
#if SHIELD_CC3000 // CC3000		
		ipaddr = (*(flds+0)*0x1000000)+(*(flds+1)*0x10000)+(*(flds+2)*0x100)+*(flds+3);
#else // Arduino101
		ipaddr = (*(flds+3)*0x1000000)+(*(flds+2)*0x10000)+(*(flds+1)*0x100)+*(flds+0);
#endif
	}

	if ( g_client->connect(ipaddr, port) != (1) )	{
		Serial.println(F("sock fail connect"));
		return -1;
	}
	return 0;
}


int im_sock_disconnect()
{
	g_client->stop();
}

int im_sock_connected()
{
	if ( g_client->connected() ) return 1;

	Serial.println(F("sock not connected"));
	return 0;
}



int im_sock_available()
{
	return ((int)g_client->available()>0)?1:0;
}

void im_sock_flush()
{
	g_client->flush();	
}


#define READ_CHUNK_LEN	16

int im_sock_send(char *buff, int len)
{
	int lenSent = 0;
	char  ch[READ_CHUNK_LEN];
	
	if ( !im_sock_connected() )	{
		return -1;
	}

	int readCount = len / READ_CHUNK_LEN;
	int readRemain = len % READ_CHUNK_LEN;
	
	for (int i=0; i<readCount; i++)		{
		lenSent += g_client->write((const uint8_t*)buff+lenSent, READ_CHUNK_LEN);
	}

	if ( readRemain > 0 )	{
		lenSent += g_client->write((const uint8_t*)buff+lenSent, readRemain);
	}

	if ( len != lenSent ){
		Serial.println(F("sock fail write"));
		im_log_hex((unsigned char*)buff, len);
		g_client->stop();
		return -1;
	}

	return lenSent;
}

int im_sock_recv(char *buff, int len)
{
	int lenRead = 0;

	for (lenRead=0; lenRead<len; lenRead++ )	
	{
		if ( !g_client->available() )	
		{
			uint32_t previousMillis = millis();
			while(!g_client->available()) {
				uint32_t currentMillis = millis();
				if(currentMillis - previousMillis >= ((int32_t) IM_SOCKET_TIMEOUT * 1000)){
					Serial.println(F("sock read timeout"));
					return -1;
				}
				delay(10);
			}
		}
		
		char ch =  g_client->read();
		buff[lenRead] = ch;
	}

	return lenRead;
}

#endif	// ARDUINO
