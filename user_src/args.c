#include "args.h"
#include "../auxiliary_src/sope.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "debug.h"

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

tlv_request_t process_arguments(char **argv)
{
    req_value_t value;
    value.header = new_header(
        argv[ACCID_INDEX],
        argv[PASSWORD_INDEX],
        argv[OPDELAY_INDEX]);
    op_type_t operation = __ATOUL(argv[OPCODE_INDEX]);
    char **opargs = separate_args(argv[OPARGS_INDEX]);
    switch (operation) {
        case OP_CREATE_ACCOUNT:
            value.create = new_create_account_message(opargs);
            break;
        case OP_TRANSFER:
            value.transfer = new_transfer_message(opargs);
            break;
        default:
            break;
    }
    string_array_free_elements(opargs);

    tlv_request_t request;
    request.type = operation;
    request.length = sizeof(req_value_t);
    request.value = value;
    return request;
}

char **separate_args(char *arg_str)
{
    __debug_log_str("args::separate_args: entered function.");
    __debug_print_str("Value of arg_str: ");
    __debug_log_str(arg_str);

    if (!(strlen(arg_str) > 0))
    {
        __debug_log_str("args::separate_args: strlen was not greater than 0, returning NULL");
        return NULL;
    }

    char **array = (char **) malloc(4 * sizeof(char *));
    if (array == NULL)
    {
        __debug_log_str("args::separate_args: could not allocate the string array, returning NULL");
        return NULL;
    }

    for (int i = 0; i < 4; ++i) {
        array[i] = (char *) malloc((MAX_PASSWORD_LEN + 1) * sizeof(char));
        if (array[i] == NULL) {
            __debug_log_str("args::separate_args: failed to allocate space for separated strings, freeing previous allocations and returning NULL");
            for (int j = 0; j < i; ++j) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }

    char *token = strtok(arg_str, " ");
    strncpy(array[0], token, MAX_PASSWORD_LEN + 1);
    int index = 1;
    while (((token = strtok(NULL, " ")) != NULL) && (index < 3))
    {
        strncpy(array[index], token, MAX_PASSWORD_LEN + 1);
        ++index;
    }

    array[3] = NULL;

    return array;
}

void string_array_free_elements(char **array)
{
    if (array == NULL)
    {
        return;
    }
    for (int i = 0; array[i] != NULL; ++i)
    {
        free(array[i]);
    }
    free(array);
}

req_header_t new_header(char *account_id, char *password, char *op_delay)
{
    req_header_t header;
    header.pid = getpid();
    header.account_id = __ATOUL(account_id);
    ;
    strncpy(header.password, password, MAX_PASSWORD_LEN);
    header.password[MAX_PASSWORD_LEN] = '\0';
    header.op_delay_ms = __ATOUL(account_id);
    ;
    return header;
}

req_create_account_t new_create_account_message(char **operation_arguments)
{
    req_create_account_t create;
    create.account_id = __ATOUL(operation_arguments[0]);
    create.balance = __ATOUL(operation_arguments[1]);
    strncpy(create.password, operation_arguments[2], MAX_PASSWORD_LEN);
    create.password[MAX_PASSWORD_LEN] = '\0';
    return create;
}

req_transfer_t new_transfer_message(char **operation_arguments)
{
    req_transfer_t transfer;
    transfer.account_id = __ATOUL(operation_arguments[0]);
    transfer.amount = __ATOUL(operation_arguments[1]);
    return transfer;
}
