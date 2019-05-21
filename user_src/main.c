#include "communication.h"
#include "../auxiliary_src/types.h"
#include "../auxiliary_src/constants.h"
#include "../auxiliary_src/sope.h"
#include "stdlib.h"
#include "stdio.h"
#include "debug.h"
#include "unistd.h"
#include "args.h"
#include "signal.h"
#include "unistd.h"

void alarm_signal_handler(int signal) {
    __debug_log_str("main::alarm_signal_handler: handling SIGALRM");
}


int set_up_alarm() {
    __debug_log_str("main::set_up_alarm: entered function");
    signal(SIGALRM, alarm_signal_handler);
    siginterrupt(SIGALRM, 1);
    return 0;
}


int main(int argc, char *argv[])
{
    __debug_log_str("main: checking number of arguments");
    if (argc != 6) {
        __debug_log_str("main: argc was not 6, printing usage and quitting");
        printf("Usage: %s %s %s %s %s %s\n",
        argv[0],
        "account_id",
        "\"password\"",
        "op_delay_in_ms",
        "op_code",
        "\"op_arguments\"");
        exit(EXIT_FAILURE);
    }

    int log_fd = open(USER_LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0660);

    __debug_log_str("main: starting connection to server fifo");
    if (open_connection_to_srv()) {
        fprintf(stderr, "main: Could not connect to %s\n", SERVER_FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    __debug_log_str("main: creating local fifo");
    if (create_local_fifo()) {
        fprintf(stderr, "main: could not create %s%05d\n", USER_FIFO_PATH_PREFIX, getpid());
        exit(EXIT_FAILURE);
    }

    __debug_log_str("main: processing arguments");
    tlv_request_t request = process_arguments(argv);

    __debug_log_str("main: trying to send request");
    if (send_request(request)) {
        fprintf(stderr, "main: failed to send request\n");
        exit(EXIT_FAILURE);
    }
    logRequest(log_fd, getpid(), &request);

    __debug_log_str("main: closing connection to server fifo");
    if (close_connection_to_srv()) {
        fprintf(stderr, "main: could not close connection to %s\n", SERVER_FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    __debug_log_str("main: setting up alarm");
    set_up_alarm();

    __debug_log_str("main: starting alarm");
    alarm(FIFO_TIMEOUT_SECS);

    __debug_log_str("main: opening fifo");
    if (open_local_fifo()) {
        __debug_log_str("main: fifo opening failed, perhaps timed out");
        exit(EXIT_FAILURE);
    }

    tlv_reply_t reply;
    __debug_log_str("main::reading response");
    if (read_response(&reply)) {
        __debug_log_str("main: read failed");
    }
    logReply(log_fd, getpid(), &reply);

    exit(EXIT_SUCCESS);
}
