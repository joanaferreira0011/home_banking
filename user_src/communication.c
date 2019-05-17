#include "communication.h"
#include "stdio.h"
#include "protocol.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "errno.h"
#include "debug.h"

static int server_fifo_fd = -1;
static int local_fifo_fd = -1;

int open_connection_to_srv() {
    if (server_fifo_fd != -1) {
        __debug_log("communication::open_connection_to_srv: fifo fd is -1");
        return 1;
    }
    if ((server_fifo_fd = open(SERVER_FIFO_PATH, O_WRONLY)) == -1) {
        __debug_log("communication::open_connection_to_srv: call to open failed");
        return 1;
    }
    return 0;
}

int close_connection_to_srv() {
    __debug_log("communication::close_connection_to_srv: entered function");
    if (server_fifo_fd == -1) {
        __debug_log("communication::close_connection_to_srv: connection is already closed");
        return 1;
    }
    if (close(server_fifo_fd) == -1) {
        __debug_log("communication::close_connection_to_srv: failed to close connection");
        return 1;
    }
    server_fifo_fd = -1;
    return 0;
}

void close_local_fifo(void) {
    __debug_log("communication::close_local_fifo: entered function");
    if (local_fifo_fd != -1) {
        close(local_fifo_fd);
    }
}

int open_local_fifo() {
    __debug_log("communication::open_local_fifo: entered function");
    if (local_fifo_fd != -1) {
        __debug_log("communication::open_local_fifo: fifo is already open");
        return 1;
    }
    char path[USER_FIFO_PATH_LEN];
    snprintf(path, USER_FIFO_PATH_LEN, "%s%05d", USER_FIFO_PATH_PREFIX, getpid());
    if ((local_fifo_fd = mkfifo(path, 0660)) == -1) {
        __debug_log("communication::open_local_fifo: mkfifo failed");
        return 1;
    }
    if (atexit(close_local_fifo)) {
        __debug_log("communication::open_local_fifo: atexit failed, closing fifo again");
        close(local_fifo_fd);
        return 1;
    }
    return 0;
}



int send_request(tlv_request_t request) {
    __debug_log("communication::send_request: entered function");
    if (server_fifo_fd == -1) {
        __debug_log("communication::send_request: not connected to server");
        return 1;
    }
    char *serialized = serialize_request(request);
    if (serialized == NULL) {
        __debug_log("communication::send_request: serialized is NULL");
        return 1;
    }
    if (dprintf(server_fifo_fd, "%s",serialized) == 0) {
        __debug_log("communication::send_request: failed to send serialized");
        free(serialized);
        return 1;
    }
    free(serialized);
    return 0;
}

int read_response(tlv_reply_t *buffer) {
    __debug_log("communication::read_response: entered function");
    if (local_fifo_fd == -1) {
        __debug_log("communication::read_response: local fifo is closed");
        return 1;
    }
    if (buffer == NULL) {
        __debug_log("communication::read_response: buffer is a NULL pointer");
        return 1;
    }
    op_type_t reply_type;
    uint32_t reply_length;
    rep_value_t reply_value;
    __debug_log("communication::read_response: trying to read reply_type, will block while nothing is received");
    if (read(local_fifo_fd, &reply_type, sizeof(op_type_t)) == -1) {
        __debug_log("communication::read_response: read failed, checking for signal interruption");
        if (errno == EINTR) {
            __debug_log("communication::read_response: EINTR detected, server probably timed out");
            return 1;
        }
    }
    __debug_log("communication::read_response: reading message length");
    read(local_fifo_fd, &reply_length, sizeof(uint32_t));
    __debug_log("communication::read_response: reading message value");
    read(local_fifo_fd, &reply_value, reply_length);
    __debug_log("communication::read_response: writing fields to buffer");
    buffer->type = reply_type;
    buffer->length = reply_length;
    buffer->value = reply_value;
    return 0;
}
