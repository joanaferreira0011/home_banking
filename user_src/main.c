#include "communication.h"
#include "../auxiliary_src/types.h"
#include "../auxiliary_src/constants.h"
#include "stdlib.h"
#include "stdio.h"
#include "debug.h"
#include "unistd.h"
#include "args.h"

int set_up_alarm() {
    return 0;
}

int main(int argc, char *argv[])
{
    __debug_log("main: checking number of arguments");
    if (argc != 6) {
        __debug_log("main: argc was not 6, printing usage and quitting");
        printf("Usage: %s %s %s %s %s %s\n",
            argv[0],
            "account_id",
            "\"password\"",
            "op_delay_in_ms",
            "op_code",
            "\"op_arguments\"");
        exit(EXIT_FAILURE);
    }

    __debug_log("main: starting connection to server fifo");
    if (open_connection_to_srv()) {
        fprintf(stderr, "main: Could not connect to %s\n", SERVER_FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    __debug_log("main: opening local fifo");
    if (open_local_fifo()) {
        fprintf(stderr, "main: could not create %s%05d\n", USER_FIFO_PATH_PREFIX, getpid());
        exit(EXIT_FAILURE);
    }

    __debug_log("main: processing arguments");
    tlv_request_t request = process_arguments(argv);

    __debug_log("main: trying to send request");
    if (send_request(request)) {
        fprintf(stderr, "main: failed to send request");
        exit(EXIT_FAILURE);
    }

    __debug_log("main: closing connection to server fifo");
    if (close_connection_to_srv()) {
        fprintf(stderr, "main: could not close connection to %s\n", SERVER_FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    __debug_log("main: setting up alarm");
    set_up_alarm();

    __debug_log("main: starting alarm");
    alarm(FIFO_TIMEOUT_SECS);

    char *response_str = NULL;
    if (read_response(response_str)) {
        __debug_log("main: timed out");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}