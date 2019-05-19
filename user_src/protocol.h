//#define __USER_PROTOCOL_H__ //disables header
#ifndef __USER_PROTOCOL_H__
#define __USER_PROTOCOL_H__

#include "../auxiliary_src/types.h"

char *serialize_request(tlv_request_t *request);

rep_value_t deserialize_rep_value(char *str, op_type_t type);

#endif/*__USER_PROTOCOL_H__*/
