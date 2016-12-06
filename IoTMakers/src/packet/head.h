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



#ifndef IOTMAKERS_PKTHEAD_H
#define IOTMAKERS_PKTHEAD_H

#if defined(ARDUINO)
#include <Arduino.h>
#else
#define PROGMEM
#endif


////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

int head_send_auth_device();
int head_send_keepalive();
int head_send_collection(char* authCode);
int head_send_control_ack(char* authCode);

int head_is_TypeDevAuth_res(char* head);
int head_is_TypeKeepAlive_res(char* head);
int head_is_TypeReport_res(char* head);
int head_is_TypeCtrl_req(char* head);


#ifdef __cplusplus
}
#endif


#endif

