#include "protocol.h"
#include "string.h"
#include "../auxiliary_src/types.h"
#include "stdlib.h"

char *serialize_request(tlv_request_t request) {
    char *serialized = malloc(sizeof(tlv_request_t) + 1);
    memcpy(serialized, &request, sizeof(tlv_request_t));
    serialized[sizeof(tlv_request_t)] = '\0';
    return serialized;
}

tlv_reply_t deserialize_reply(char *serialized_reply);