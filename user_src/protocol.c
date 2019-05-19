#include "protocol.h"
#include "string.h"
#include "../auxiliary_src/types.h"
#include "stdlib.h"
#include "stdio.h"

#define WIDTH_TYPE 1
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

char *stradd(char *str1, char *str2);

char *serialize_type(op_type_t type);
char *serialize_length(uint32_t length);
char *serialize_value(req_value_t value, op_type_t op_type);
char *serialize_request(tlv_request_t *request) {
    char *type = serialize_type(request->type);
    char *value = serialize_value(request->value, request->type);
    uint32_t length_num = strlen(value);
    char *length = serialize_length(length_num);
    char *type_and_length = stradd(type, length);
    free(type);
    free(length);
    char *request_str = stradd(type_and_length, value);
    free(type_and_length);
    free(value);
    return request_str;
}

char *serialize_type(op_type_t type) {
    size_t size = WIDTH_TYPE + 1;
    char *result = malloc(size);
    snprintf(result, size, "%0" STRINGIFY(WIDTH_TYPE) "d", type);
    return result;
}

char *serialize_length(uint32_t length) {
    size_t size = WIDTH_TLV_LEN + 1;
    char *result = malloc(size);
    snprintf(result, size, "%0" STRINGIFY(WIDTH_TLV_LEN) "d",  length);
    return result;
}

char *serialize_header(req_header_t header);
char *serialize_create(req_create_account_t create);
char *serialize_transfer(req_transfer_t transfer);
char *serialize_value(req_value_t value, op_type_t op_type) {
    char *result = NULL;
    char *header = serialize_header(value.header);
    switch (op_type) {
        case OP_CREATE_ACCOUNT:
            char *create = serialize_create(value.create);
            result = stradd(header, create);
            free(create);
            break;
        case OP_TRANSFER:
            char *transfer = serialize_transfer(value.transfer);
            result  = stradd(header, transfer);
            free(transfer);
            break;
        default:
            break;
    }
    free(header);
    return result;
}

char *serialize_pid(pid_t pid);
char *serialize_acc_id(uint_32_t acc_id);
char *copy_and_trim_pw(char *password);
char *serialize_delay(uint32_t delay);
char *serialize_header(req_header_t header) {
    char *pid = serialize_pid(header.pid);
    char *acc_id = serialize_acc_id(header.account_id);
    char *pid_acc_id = stradd(pid, acc_id);
    free(pid);
    free(acc_id);
    char *pw = copy_and_trim_pw(header.password);
    char *pid_acc_id_pw = stradd(pid_acc_id, pw);
    free(pid_acc_id);
    free(pw);
    char *delay = serialize_delay(header.delay);
    char *result = stradd(pid_acc_id_pw, delay);
    free(pid_acc_id_pw);
    free(delay);
    return result;
}

char *serialize_pid(pid_t pid) {
    size_t size = WIDTH_ID + 1;
    char *result = malloc(size);
    snprintf(result, size, "%0" STRINGIFY(WIDTH_ID) "d",  pid);
    return result;
}
char *serialize_acc_id(uint_32_t acc_id) {
    size_t size = WIDTH_ACCOUNT + 1;
    char *result = malloc(size);
    snprintf(result, size, "%0" STRINGIFY(WIDTH_ACCOUNT) "d", acc_id);
    return result;
}
char *copy_and_trim_pw(char *password) {
    size_t size = strlen(password);
    char *trimmed = malloc(size);
    memcpy(trimmed, password, size);
    return trimmed;
}
char *serialize_delay(uint32_t delay) {
    size_t size = WIDTH_DELAY + 1;
    char *result = malloc(size);
    snprintf(result, size, "%0" STRINGIFY(WIDTH_DELAY) "d", delay);
    return result;
}

char *serialize_balance(uint32_t ammount);
char *serialize_create(req_create_account_t create) {
    char *acc_id = serialize_acc_id(create.account_id);
    char *balance = serialize_balance(create.balance);
    char *pw = copy_and_trim_pw(create.password);
    char *acc_id_balance = stradd(acc_id, balance);
    char *result = stradd(acc_id_balance, pw);
    free(acc_id); free(balance);
    free(pw); free(acc_id_balance);
    return result;
}

char *serialize_transfer(req_transfer_t transfer) {
    char *acc_id = serialize_acc_id(transfer.account_id);
    char *amount = serialize_balance(amount);
    char *result = stradd(acc_id, amount);
    free(acc_id); free(amount);
    return result;
}
