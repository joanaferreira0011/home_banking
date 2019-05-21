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

req_header_t new_header(char *account_id, char *password, char *op_delay);

tlv_request_t process_arguments(char **argv)
{
    __debug_log_str("args::process_arguments: entered function");
    req_value_t value;
    value.header = new_header(
        argv[ACCID_INDEX],
        argv[PASSWORD_INDEX],
        argv[OPDELAY_INDEX]);
    op_type_t operation = __ATOUL(argv[OPCODE_INDEX]);
    switch (operation) {
        case OP_CREATE_ACCOUNT:
            sscanf(argv[OPARGS_INDEX], "%u %u %s",
                &(value.create.account_id), &(value.create.balance), value.create.password
            );
            break;
        case OP_TRANSFER:
            sscanf(argv[OPARGS_INDEX], "%u %u",
                &(value.create.account_id), &(value.transfer.amount));
            break;
        default:
            break;
    }
    tlv_request_t request;
    request.type = operation;
    request.length = 0;
    request.value = value;
    return request;
}

req_header_t new_header(char *account_id, char *password, char *op_delay)
{
    __debug_log_str("args::new_header: entered function");
    req_header_t header;
    header.pid = getpid();
    header.account_id = __ATOUL(account_id);
    ;
    strncpy(header.password, password, MAX_PASSWORD_LEN);
    header.password[MAX_PASSWORD_LEN] = '\0';
    header.op_delay_ms = __ATOUL(op_delay);
    __debug_log_str("args::new_header: leaving function");
    return header;
}
