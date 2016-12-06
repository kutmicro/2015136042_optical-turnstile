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
#elif defined(LINUX)
#include <fcntl.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "util/log.h"


int __bas_util_log_level;

void im_log_set_level(int level)
{
	__bas_util_log_level  = level;
}


#if IM_LOGGING

void im_log(const char *fmt, ...)
{
	char    ebuf[IOTMAKERS_MAX_LOG_MSG_LEN];
	va_list vlst;

	va_start (vlst, fmt );
	vsnprintf(ebuf, sizeof(ebuf), fmt, vlst);
	va_end (vlst);

#if defined(ARDUINO)
  Serial.println(ebuf);
#elif defined(LINUX)
  printf("%s\n", ebuf);
#endif

	return;
}

void im_log_hex(unsigned char* data, int size)
{
#if !defined(ARDUINO)
	int i, rowsize, offset;
    char *bufpos, buffer[80];
  
	if (size<=0){
		return;
	}

	for( offset = 0; size;  )
	{
		rowsize = ( size > 16 ) ? 16 : size;
		sprintf( buffer, "%04x: ", offset );
		bufpos = buffer + strlen( buffer );
	
		if ( rowsize >= 8 ) {
			for( i = 0; i < 8; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
			strcat( bufpos, "| " ); bufpos += 2;
			for( ; i < rowsize; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
		}else{
			for( i = 0; i < rowsize; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
		}

		for( ; i < 16; i++ ) {
		    strcat( buffer, "   " );
		};

		strcat( buffer, "  " );
		bufpos = buffer + strlen( buffer );
		for( i = 0; i < rowsize; i++, bufpos++ ) {
			sprintf( bufpos, "%c", ( isprint(data[i]) && (data[i]!='%') ) ? data[i] : '.' );
		};

		size -= rowsize;
		data += rowsize;
		offset += rowsize;
	
		*bufpos = '\0';

#if defined(ARDUINO)
	  Serial.println(buffer);
#elif defined(LINUX)
		DEBUG_LOG("%s", buffer);
#endif
	}
#endif
}



#endif
