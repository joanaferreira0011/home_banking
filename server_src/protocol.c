#include "protocol.h"
#include "string.h"
#include "../auxiliary_src/constants.h"
#include "stdlib.h"
#include "stdio.h"

#define WIDTH_TYPE 1
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define BUFFER_SIZE 512
#define VALUE_DELIMITER "|v|"
#define HEADER_DELIMITER "|h|"
#define W_TLV_LEN STRINGIFY(WIDTH_TLV_LEN)
#define W_TYPE STRINGIFY(WIDTH_TYPE)
#define W_ID STRINGIFY(WIDTH_ID)
#define W_ACC STRINGIFY(WIDTH_ACCOUNT)
#define W_DL STRINGIFY(WIDTH_DELAY)
#define W_BAL STRINGIFY(WIDTH_BALANCE)
#define WIDTH_OPEN_OFFICES 2
#define W_OFF STRINGIFY(WIDTH_OPEN_OFFICES)
#define WIDTH_RETURN_CODES 2
#define W_RC STRINGIFY(WIDTH_RETURN_CODES)

req_value_t deserialize_req_value(char *str, op_type_t type) {
    req_value_t value;
    char header_str[BUFFER_SIZE];
    switch (type) {
        case OP_CREATE_ACCOUNT: ;
            sscanf(str, "%s" VALUE_DELIMITER "%u" VALUE_DELIMITER "%u" VALUE_DELIMITER "%s",
            header_str, &(value.create.account_id), &(value.create.balance), value.create.password);
            break;
        case OP_TRANSFER: ;
            sscanf(str, "%s" VALUE_DELIMITER "%u" VALUE_DELIMITER "%u",
            header_str, &(value.transfer.account_id), &(value.transfer.amount));
            break;
        default: ;
            sscanf(str, "%s", header_str);
            break;
    }
    sscanf(header_str, "%d" HEADER_DELIMITER "%u" HEADER_DELIMITER "%s" HEADER_DELIMITER "%u",
    &(value.header.pid), &(value.header.account_id), value.header.password, &(value.header.op_delay_ms));
    return value;
}

char *trim_string(char *str);
char *serialize_value(rep_value_t value, op_type_t type);
char *serialize_reply(tlv_reply_t *reply) {
    char *value = serialize_value(reply->value, reply->type);
    reply->length = strlen(value);
    char* buffer = malloc(BUFFER_SIZE);
    snprintf(buffer, BUFFER_SIZE,
        "%" W_TYPE "u%0" W_TLV_LEN "u%s",
        reply->type, reply->length, value
    );
    char *result = trim_string(buffer);
    free (value); free(buffer);
    return result;
}

char *serialize_header(rep_header_t header);
char *serialize_value(rep_value_t value, op_type_t type) {
    char *buffer = malloc(BUFFER_SIZE);
    char *header = serialize_header(value.header);
    switch (type) {
        case OP_BALANCE: ;
            rep_balance_t balance = value.balance;
            snprintf(buffer, BUFFER_SIZE,
                "%s" VALUE_DELIMITER "%0" W_BAL "u",
                header, balance.balance
            );
            break;
        case OP_TRANSFER: ;
            rep_transfer_t transfer = value.transfer;
            snprintf(buffer, BUFFER_SIZE,
                "%s" VALUE_DELIMITER "%0" W_BAL "u",
                header, transfer.balance
            );
            break;
        case OP_SHUTDOWN: ;
            rep_shutdown_t shutdown = value.shutdown;
            snprintf(buffer, BUFFER_SIZE,
                "%s" VALUE_DELIMITER "%0" W_OFF "u",
                header, shutdown.active_offices
            );
            break;
        default: ;
            strcpy(buffer, header);
            break;
    }
    char *result = trim_string(buffer);
    free(buffer); free(header);
    return result;
}

char *serialize_header(rep_header_t header) {
    char *buffer = malloc(BUFFER_SIZE);
    snprintf(buffer, BUFFER_SIZE,
        "%0" W_ACC "u" HEADER_DELIMITER "%0" W_RC "u",
        header.account_id, header.ret_code
    );
    char *result = trim_string(buffer);
    free(buffer);
    return result;
}

char *trim_string(char *string) {
    size_t size = strlen(string) + 1;
    char *result = malloc(size);
    memcpy(result, string, size);
    return result;
}
