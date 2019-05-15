#ifndef __HOME_BANKING_USER_ARGS_H__
#define __HOME_BANKING_USER_ARGS_H__
#include "types.h"
#include "constants.h"

#define EXPECTED_ARGC 6

int process_arguments(char ** args, tlv_request_t *buffer);

#endif/*__HOME_BANKING_USER_ARGS_H__*/
