#include "communication.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "errno.h"
#include "debug.h"
#include "../auxiliary_src/constants.h"

#define WIDTH_TYPE 1
#define __ATOUL(STR) strtoul(STR, NULL, 10)

static int server_fifo_fd = -1;
static int local_fifo_fd = -1;
static char* local_fifo_path = NULL;

int open_connection_to_srv() {
    if (server_fifo_fd != -1) {
        __debug_log_str("communication::open_connection_to_srv: fifo fd is -1");
        return 1;
    }
    if ((server_fifo_fd = open(SERVER_FIFO_PATH, O_WRONLY)) == -1) {
        __debug_log_str("communication::open_connection_to_srv: call to open failed");
        return 1;
    }
    return 0;
}

int close_connection_to_srv() {
    __debug_log_str("communication::close_connection_to_srv: entered function");
    if (server_fifo_fd == -1) {
        __debug_log_str("communication::close_connection_to_srv: connection is already closed");
        return 1;
    }
    if (close(server_fifo_fd) == -1) {
        __debug_log_str("communication::close_connection_to_srv: failed to close connection");
        return 1;
    }
    server_fifo_fd = -1;
    return 0;
}

void close_local_fifo(void) {
    __debug_log_str("communication::close_local_fifo: entered function");
    if (local_fifo_fd != -1) {
        close(local_fifo_fd);
        unlink(local_fifo_path);
        free(local_fifo_path);
    }
}

int open_local_fifo() {
    __debug_log_str("communication::open_local_fifo: entered function");
    if (local_fifo_fd != -1) {
        __debug_log_str("communication::open_local_fifo: fifo is already open");
        return 1;
    }
    local_fifo_path = malloc(USER_FIFO_PATH_LEN + 1);
    snprintf(local_fifo_path, USER_FIFO_PATH_LEN, "%s%05d", USER_FIFO_PATH_PREFIX, getpid());
    if (mkfifo(local_fifo_path, 0660)) {
        __debug_log_str("communication::open_local_fifo: mkfifo failed");
        return 1;
    }
    if ((local_fifo_fd = open(local_fifo_path, O_RDONLY | O_NONBLOCK)) == -1)
        return -1;
    atexit(close_local_fifo);
    return 0;
}



int send_request(tlv_request_t request) {
    __debug_log_str("communication::send_request: entered function");
    if (server_fifo_fd == -1) {
        __debug_log_str("communication::send_request: not connected to server");
        return 1;
    }

    if (write(server_fifo_fd, &request, sizeof(tlv_request_t)) < 0) {
        __debug_log_str("communication::send_request: failed to send serialized");
        return 1;
    }
    return 0;
}

int read_response(tlv_reply_t *buffer) {
    op_type_t type;
    uint32_t length;
    rep_value_t value;
    if (read(local_fifo_fd, &type, sizeof(op_type_t)) == -1) {
        if (errno == EINTR)
            return 1;
    }
    read(local_fifo_fd, &length, sizeof(uint32_t));
    read(local_fifo_fd, &value, length);
    buffer->type = type;
    buffer->length = length;
    buffer->value = value;
}
