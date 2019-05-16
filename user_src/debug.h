#ifndef __USER_DEBUG_H__
#define __USER_DEBUG_H__

#ifdef __DEBUG_ON
#include "stdio.h"
#define __debug_log(str) printf("%s\n", str)
#else
#define __debug_log(str)
#endif

#endif/*__USER_DEBUG_H__*/