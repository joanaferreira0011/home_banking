#ifndef __SERVER_PROTOCOL_H__
#define __SERVER_PROTOCOL_H__
#include "../auxiliary_src/types.h"

req_value_t deserialize_req_value(char *str, op_type_t type);

char *serialize_reply(tlv_reply_t *reply);

#endif/*__SERVER_PROTOCOL_H__*/
