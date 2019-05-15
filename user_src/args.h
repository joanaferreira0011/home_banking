#ifndef __HOME_BANKING_USER_ARGS_H__
#define __HOME_BANKING_USER_ARGS_H__

#include "../auxiliary_src/types.h"
#include "../auxiliary_src/constants.h"

#define EXPECTED_ARGC 6

tlv_request_t process_arguments(char **argv);

#endif /*__HOME_BANKING_USER_ARGS_H__*/
