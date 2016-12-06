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
#include <stdio.h>
#endif

#include "util/util.h"


#if defined(LINUX)
#include <sys/time.h>
#include <time.h>
#elif defined(WIN32)
#include <windows.h>
#include <sys/timeb.h>
int gettimeofday (struct timeval *tv, void* tz)
{
    struct _timeb timebuffer;
    
    _ftime(&timebuffer);
    tv->tv_sec = timebuffer.time;
    tv->tv_usec = timebuffer.millitm * 1000;
	return (0);
} 
#endif


#if defined(LINUX) || defined(WIN32)
static struct timeval g_time_of_day;
#endif


long im_util_gettimeofday_as_sec()
{   
#if defined(LINUX) || defined(WIN32)
	gettimeofday(&g_time_of_day, NULL);
	return (time_t)g_time_of_day.tv_sec;
#endif
	return 0;
}

long im_util_gettimeofday_as_usec()
{   
#if defined(LINUX) || defined(WIN32)
	gettimeofday(&g_time_of_day, NULL);
	return (time_t)g_time_of_day.tv_usec;
#endif
	return 0;
}




unsigned long im_util_get_secs()
{
#if defined(ARDUINO)
	return (unsigned long)(millis()/1000);
#else
	return im_util_gettimeofday_as_sec();
#endif
}



#if defined(ARDUINO)
/*
borrowed from Print.cpp in Arduino and modifyed
*/
char *im_util_dtostr (double number, signed char width, unsigned char prec, char *sout) 
{
  size_t n = 0;
  uint8_t i;
  
  if (isnan(number)) return strcpy(sout, "0");
  if (isinf(number)) return strcpy(sout, "0");
  if (number > 4294967040.0) return strcpy(sout, "0");  // constant determined empirically
  if (number <-4294967040.0) return strcpy(sout, "0");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)  {
     n += sprintf(sout+n, "-");
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (i=0; i<prec; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += sprintf(sout+n, "%ld", int_part);

  // Print the decimal point, but only if there are prec beyond
  if (prec > 0) {
    n += sprintf(sout+n, "."); 
  }

  // Extract prec from the remainder one at a time
  while (prec-- > 0)
  {
    remainder *= 10.0;
    int toPrint = (int)(remainder);
    n += sprintf(sout+n, "%d", toPrint);
    remainder -= toPrint; 
  } 
  
  return sout;
}
#endif