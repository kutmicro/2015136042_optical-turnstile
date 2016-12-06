/*
 * GiGA IoT Platform version 2.0
 *
 *  Copyright(c) 2016 kt corp. All rights reserved.
 *
 *  This is a proprietary software of kt corp, and you may not use this file except in
 *  compliance with license agreement with kt corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of kt corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 */

#ifndef IOTMAKERS_H
#define IOTMAKERS_H

#if defined(ARDUINO)
#include <Arduino.h>
#include <Client.h>
#endif

typedef void (*IMCbTagidNumDataHndl)(char *tagid, double val);
typedef void (*IMCbTagidStrDataHndl)(char *tagid, char *val);
typedef void (*IMCbDataRespHndl)(long long trxid, char *respCode);
typedef void (*IMCbErrorHndl)(int errCode);

class IoTMakers
{
  public:
    IoTMakers();

#if defined(ARDUINO)
	int init(const char *ip, int port, const char *deviceId, const char *devicePasswd, const char *gatewayId, Client& client);
	int init(const char *deviceId, const char *devicePasswd, const char *gatewayId, Client& client);
	int getFreeRAM();
#else
	int init(const char *ip, int port, const char *deviceId, const char *devicePasswd, const char *gatewayId);
	int init(const char *deviceId, const char *devicePasswd, const char *gatewayId);
#endif

	int connect();
	int disconnect();
	int auth_device();
	int send_numdata(const char* tagid, double val, long long trxid=0);
	int send_strdata(const char* tagid, char* val, long long trxid=0);
	void loop();

	void set_numdata_handler(IMCbTagidNumDataHndl cb_proc);
	void set_strdata_handler(IMCbTagidStrDataHndl cb_proc);
	void set_dataresp_handler(IMCbDataRespHndl cb_proc);
	void set_error_handler(IMCbErrorHndl cb_proc);
};

#endif
