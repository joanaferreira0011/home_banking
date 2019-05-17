#ifndef __USER_DEBUG_H__
#define __USER_DEBUG_H__
#define __DEBUG_ON /* turns on debug messages */
#ifdef __DEBUG_ON
#include "stdio.h"
#define __debug_log(str) fprintf(stderr ,"%s\n", str)
#else
#define __debug_log(str)
#endif

#endif/*__USER_DEBUG_H__*/
