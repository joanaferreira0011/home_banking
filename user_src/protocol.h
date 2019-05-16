#define __USER_PROTOCOL_H__ //disables header
#ifndef __USER_PROTOCOL_H__
#define __USER_PROTOCOL_H__

#include "../auxiliary_src/types.h"

char *serialize_request(tlv_request_t request);

tlv_reply_t deserialize_reply(char *serialized_reply);

#endif/*__USER_PROTOCOL_H__*/