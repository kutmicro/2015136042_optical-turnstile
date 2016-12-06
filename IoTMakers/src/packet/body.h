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


#ifndef IOTMAKERS_PKTBODY_H
#define IOTMAKERS_PKTBODY_H


#if defined(ARDUINO)
#include <Arduino.h>
#else
#define PROGMEM
#endif


#ifdef __cplusplus
extern "C"
{
#endif

int body_sizeof_devAuth(char *extrSysId, char *deviceId, char *athnRqtNo);
int body_sizeof_numdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val);
int body_sizeof_strdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val);
int body_sizeof_contrl_ack_resp();

int body_send_devAuth(char *extrSysId, char *deviceId, char *athnRqtNo);
int body_send_numdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val);
int body_send_strdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val);
int body_send_control_ack();


#ifdef __cplusplus
}
#endif


#endif

