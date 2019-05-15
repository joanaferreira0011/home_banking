#include "args.h"
#include "sope.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

#define ACCID_INDEX 1
#define PASSWORD_INDEX 2
#define OPDELAY_INDEX 3
#define OPCODE_INDEX 4
#define OPARGS_INDEX 5
#define __ATOUL(STR) strtoul(STR, NULL, 10)

/* Separates the arguments from an argument string */
char **separate_args(char *arg_str);

void string_array_free_elements(char **array);

req_header_t new_header(char *account_id, char *password, char *op_delay);

req_create_account_t new_create_account_message(char **operation_arguments);

req_transfer_t new_transfer_message(char **operation_arguments);


tlv_request_t process_arguments(char **argv) {
    req_value_t value;
    value.header = new_header (
        argv[ACCID_INDEX],
        argv[PASSWORD_INDEX],
        argv[OPDELAY_INDEX]
    );
    
    op_type_t operation = __ATOUL(argv[OPCODE_INDEX]);
    char **opargs = separate_args(argv[OPARGS_INDEX]);
    switch (operation) {
        case OP_CREATE_ACCOUNT:
            value.create = new_create_account_message(opargs);
            break;
        case OP_TRANSFER:
            value.transfer = new_transfer_message(opargs);
            break;
    }
    string_array_free_elements(opargs);

    tlv_request_t request;
    request.type = operation;
    request.length = sizeof(req_value_t);
    request.value = value;
    return request;
}

char **separate_args(char *arg_str) {
    if (!(strlen(arg_str) > 0)) {
        return NULL;
    }
    
    char **array = malloc(4*sizeof(char*));
    if (array == NULL) {
        return NULL;
    }

    array[0] = strtok(arg_str, " ");
    int index = 1;
    while (((array[index] = strtok(NULL, " ")) != NULL) && (index < 4)) {
        ++index;
    }

    array[3] = NULL;

    return array;
}

void string_array_free_elements(char **array) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; array(i) != NULL; ++i) {
        free(array[i]);
    }
    free(array);
}

req_header_t new_header(char *account_id, char *password, char *op_delay) {
    req_header_t header;
    header.pid = getpid();
    header.account_id = __ATOUL(account_id);;
    strncpy(header.password,password, MAX_PASSWORD_LEN);
    header.password[PASSWORD_LENGTH] = '\0';
    header.op_delay_ms = __ATOUL(account_id);;
    return header;
}

req_create_account_t new_create_account_message(char **operation_arguments) {
    req_create_account_t create;
    create.account_id = __ATOUL(operation_arguments[0]);
    balance.account_id = __ATOUL(operation_arguments[1]);
    strncpy(create.password, operation_arguments[2], MAX_PASSWORD_LEN);
    create.password[MAX_PASSWORD_LEN] = '\0';
    return create;
}

req_transfer_t new_transfer_message(char **operation_arguments) {
    req_transfer_t transfer;
    transfer.account_id = __ATOUL(operation_arguments[0]);
    transfer.amount = __ATOUL(operation_arguments[1]);
    return transfer;
}


