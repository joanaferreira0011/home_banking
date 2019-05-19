#ifndef __USER_DEBUG_H__
#define __USER_DEBUG_H__
#define __DEBUG_ON /* turns on debug messages */
#ifdef __DEBUG_ON
#include "stdio.h"
#define __debug_log_str(str) fprintf(stderr ,"%s\n", str)
#define __debug_print_str(str) fprintf(stderr, "%s", str)
#define __debug_log_int(prefix, num) fprintf(stderr,"%s%d\n", prefix, num)
#define __debug_log_uint(prefix, num) fprintf(stderr, "%s%u\n", prefix, num)
#else
#define __debug_log_str(arg1)
#define __debug_print_str(arg1)
#define __debug_log_int(arg1, arg2)
#define __debug_log_uint(arg1, arg2)
#endif

#endif/*__USER_DEBUG_H__*/
