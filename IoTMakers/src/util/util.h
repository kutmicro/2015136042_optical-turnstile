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



#ifndef IOTMAKERS_UTIL_H
#define IOTMAKERS_UTIL_H


/*
 * ********************************************************************************
 * util.h
 * ********************************************************************************
 */

#if defined(ARDUINO)
#include <Arduino.h>

#define HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#define NTOHS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define HTONL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define NTOHL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define htons HTONS 
#define htonl HTONL 
#define ntohs NTOHS 
#define ntohl NTOHL 
#endif


#ifdef __cplusplus
extern "C"
{
#endif

unsigned long im_util_get_secs();

#if defined(ARDUINO)
char* im_util_dtostr (double number, signed char width, unsigned char prec, char *sout);
#endif

#ifdef __cplusplus
}
#endif


#endif
