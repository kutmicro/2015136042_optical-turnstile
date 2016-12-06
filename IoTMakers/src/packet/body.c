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


#include <string.h>
#include "packet/body.h"
#include "netio/sock.h"
//#include "util/log.h"

#define STOP_MARK	'\b'
#define NULL_MARK	'\0'
#define NUMVER_VAL	"\b"
#define STRING_VAL	"\"\b\""

#if 0
const char FMT_AUTH_DEV_REQ[] PROGMEM = "{\"extrSysId\":"STRING_VAL",\"devId\":"STRING_VAL",\"athnRqtNo\":"STRING_VAL"}" ;
const char FMT_COLL_NUM_REQ[] PROGMEM = "{\"extrSysId\":"STRING_VAL",\"devColecDataVOs\":[{\"m2mSvcNo\":0,\"devId\":"STRING_VAL",\"colecRowVOs\":[{\"snsnDataInfoVOs\":[{\"dataTypeCd\":"STRING_VAL",\"snsnVal\":"NUMVER_VAL"}],\"strDataInfoVOs\":[]}]}]}";
const char FMT_COLL_STR_REQ[] PROGMEM = "{\"extrSysId\":"STRING_VAL",\"devColecDataVOs\":[{\"m2mSvcNo\":0,\"devId\":"STRING_VAL",\"colecRowVOs\":[{\"snsnDataInfoVOs\":[],\"strDataInfoVOs\":[{\"snsnTagCd\":"STRING_VAL",\"strVal\":"STRING_VAL"}]}]}]}";
const char FMT_CTRL_ACK_RES[] PROGMEM = "{\"respCd\":\"100\",\"respMsg\":\"Success\"}";
#else
const char FMT_AUTH_DEV_REQ[] PROGMEM = "{\"extrSysId\":"STRING_VAL",\"devId\":"STRING_VAL",\"athnRqtNo\":"STRING_VAL"}" ;
const char FMT_COLL_NUM_REQ[] PROGMEM = "{\"extrSysId\":"STRING_VAL",\"devColecDataVOs\":[{\"m2mSvcNo\":0,\"devId\":"STRING_VAL",\"colecRowVOs\":[{\"snsnDataInfoVOs\":[{\"dataTypeCd\":"STRING_VAL",\"snsnVal\":"NUMVER_VAL"}]}]}]}";
const char FMT_COLL_STR_REQ[] PROGMEM = "{\"extrSysId\":"STRING_VAL",\"devColecDataVOs\":[{\"m2mSvcNo\":0,\"devId\":"STRING_VAL",\"colecRowVOs\":[{\"strDataInfoVOs\":[{\"snsnTagCd\":"STRING_VAL",\"strVal\":"STRING_VAL"}]}]}]}";
const char FMT_CTRL_ACK_RES[] PROGMEM = "{\"respCd\":\"100\",\"respMsg\":\"Success\"}";
#endif


/*
CONTROL EXAMPLES ON RECV
STR = {"mapHeaderExtension":{},"devCnvyDataVOs":[{"devId":"river4D1459818172685","cnvyRowVOs":[{"snsnDataInfoVOs":[],"sttusDataInfoVOs":[],"contlDataInfoVOs":[],"cmdDataInfoVOs":[],"binDataInfoVOs":[],"strDataInfoVOs":[{"snsnTagCd":"led","strVal":"aaa"}],"dtDataInfoVOs":[],"genlSetupDataInfoVOs":[],"sclgSetupDataInfoVOs":[],"binSetupDataInfoVOs":[],"mapRowExtension":{}}]}],"msgHeadVO":{"mapHeaderExtension":{}}}
NUM = {"mapHeaderExtension":{},"devCnvyDataVOs":[{"devId":"river4D1459818172685","cnvyRowVOs":[{"snsnDataInfoVOs":[{"dataTypeCd":"volume","snsnVal":999.0}],"sttusDataInfoVOs":[],"contlDataInfoVOs":[],"cmdDataInfoVOs":[],"binDataInfoVOs":[],"strDataInfoVOs":[],"dtDataInfoVOs":[],"genlSetupDataInfoVOs":[],"sclgSetupDataInfoVOs":[],"binSetupDataInfoVOs":[],"mapRowExtension":{}}]}],"msgHeadVO":{"mapHeaderExtension":{}}}
*/


int body_sizeof_devAuth(char *extrSysId, char *deviceId, char *athnRqtNo)
{
	int valCount = 3;
#if defined(ARDUINO)
	return strlen_P(FMT_AUTH_DEV_REQ) - valCount + strlen(extrSysId) + strlen(deviceId) + strlen(athnRqtNo);
#else
	return strlen(FMT_AUTH_DEV_REQ) - valCount + strlen(extrSysId) + strlen(deviceId) + strlen(athnRqtNo);
#endif
}

int body_sizeof_numdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val)
{
	int valCount = 4;
#if defined(ARDUINO)
	return strlen_P(FMT_COLL_NUM_REQ) - valCount + strlen(extrSysId) + strlen(deviceId) + strlen(tagid) + strlen(val);
#else
	return strlen(FMT_COLL_NUM_REQ) - valCount + strlen(extrSysId) + strlen(deviceId) + strlen(tagid) + strlen(val);
#endif
}

int body_sizeof_strdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val)
{
	int valCount = 4;

#if defined(ARDUINO)
	return strlen_P(FMT_COLL_STR_REQ) - valCount + strlen(extrSysId) + strlen(deviceId) + strlen(tagid) + strlen(val);
#else
	return strlen(FMT_COLL_STR_REQ) - valCount + strlen(extrSysId) + strlen(deviceId) + strlen(tagid) + strlen(val);
#endif
}

int body_sizeof_contrl_ack_resp()
{
#if defined(ARDUINO)
	return strlen_P(FMT_CTRL_ACK_RES);
#else
	return strlen(FMT_CTRL_ACK_RES);
#endif
}


static int __body_send_until_break(const char* PROGMEM data, char **next, unsigned char BREAK_CH)
{
	int sent = 0;
	char* p = (char*)data;
	char buff[8];
	unsigned char idx = 0;

#if defined(ARDUINO)
	while ( (buff[idx++] = (char)pgm_read_byte_near(p++)) != BREAK_CH )
#else
	while ( (buff[idx++] = *p++) != BREAK_CH )
#endif
	{
		if ( idx == sizeof(buff) )	{
			sent += im_sock_send(buff, idx);
			idx=0;
		}
	};

	if ( idx != 0 )	{
		// send remain bytes.
		sent += im_sock_send(buff, idx-1);
		idx = 0;
	}

	*next = p++;
	return sent;
}



int body_send_devAuth(char *extrSysId, char *deviceId, char *athnRqtNo)
{
	char* p = (char*)FMT_AUTH_DEV_REQ;
	int sent = 0;

	sent +=  __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(extrSysId, strlen(extrSysId));

	sent += __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(deviceId, strlen(deviceId));

	sent +=  __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(athnRqtNo, strlen(athnRqtNo));

	sent +=  __body_send_until_break(p, &p, NULL_MARK);

	return sent;
}


static int body_send_collection_data(const char* PROGMEM FMT_COLL_REQ, char *extrSysId, char *deviceId, char *tagid, char *val)
{
	char* p = (char*)FMT_COLL_REQ;
	int sent = 0;

	sent +=  __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(extrSysId, strlen(extrSysId));

	sent += __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(deviceId, strlen(deviceId));

	sent +=  __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(tagid, strlen(tagid));

	sent +=  __body_send_until_break(p, &p, STOP_MARK);
	sent += im_sock_send(val, strlen(val));

	sent +=  __body_send_until_break(p, &p, NULL_MARK);

	return sent;
}

int body_send_numdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val)
{
	return body_send_collection_data(FMT_COLL_NUM_REQ, extrSysId, deviceId, tagid, val);
}

int body_send_strdata_collection(char *extrSysId, char *deviceId, char *tagid, char *val)
{
	return body_send_collection_data(FMT_COLL_STR_REQ, extrSysId, deviceId, tagid, val);
}

int body_send_control_ack()
{
	char* p = (char*)FMT_CTRL_ACK_RES;
	int sent = 0;

	sent +=  __body_send_until_break(p, &p, NULL_MARK);

	return sent;
}



