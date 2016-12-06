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

#ifndef IOTMAKERS_LOG_H
#define IOTMAKERS_LOG_H

#if defined(ARDUINO)
#include <Arduino.h>
#endif

#define  IM_LOGGING		(1)



#define LOG_LEVEL_ERROR   1                    /* Log level  : Error               */
#define LOG_LEVEL_INFO    2                    /* Log level  : Information         */
#define LOG_LEVEL_DEBUG   3                    /* Log level  : Debug               */

extern int __bas_util_log_level;

#ifdef __cplusplus
extern "C"
{
#endif

void im_log_set_level(int level) ;
void im_log(const char *fmt, ...) ;
void im_log_hex(unsigned char* data, int size) ;


#if IM_LOGGING

#if defined(ARDUINO)
#define IOTMAKERS_MAX_LOG_MSG_LEN			60
#define ERROR_LOG(format,...) do{if( __bas_util_log_level >= LOG_LEVEL_ERROR )im_log("E:"format,  ##__VA_ARGS__);}while(0)
#define INFO_LOG(format,...)  do{if( __bas_util_log_level >= LOG_LEVEL_INFO ) im_log("I:"format,  ##__VA_ARGS__);}while(0)
#define DEBUG_LOG(format,...) do{if( __bas_util_log_level >= LOG_LEVEL_DEBUG )im_log("D:"format,  ##__VA_ARGS__);}while(0)
#else
#define IOTMAKERS_MAX_LOG_MSG_LEN			100
#define BAS_LOG_PREFIX_FRM		"%-10.10s %04d %-10.10s"
#define ERROR_LOG(format,...) do{if( __bas_util_log_level >= LOG_LEVEL_ERROR ) im_log("ERR "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)
#define INFO_LOG(format,...)  do{if( __bas_util_log_level >= LOG_LEVEL_INFO  ) im_log("INF "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)
#define DEBUG_LOG(format,...) do{if( __bas_util_log_level >= LOG_LEVEL_DEBUG ) im_log("DBG "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)
#endif

#else	//NO LOGGING
#define ERROR_LOG(format,...) 
#define INFO_LOG(format,...) 
#define DEBUG_LOG(format,...) 
#define im_log_hex(x, y)
#endif	//IM_LOGGING


#ifdef __cplusplus
}
#endif



#endif
