#include "communication.h"
#include "stdio.h"
#include "protocol.h"
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
    if ((local_fifo_fd = mkfifo(local_fifo_path, 0660)) == -1) {
        __debug_log_str("communication::open_local_fifo: mkfifo failed");
        return 1;
    }
    if (atexit(close_local_fifo)) {
        __debug_log_str("communication::open_local_fifo: atexit failed, closing fifo again");
        close(local_fifo_fd);
        unlink(local_fifo_path);
        free(local_fifo_path);
        return 1;
    }
    return 0;
}



int send_request(tlv_request_t request) {
    __debug_log_str("communication::send_request: entered function");
    if (server_fifo_fd == -1) {
        __debug_log_str("communication::send_request: not connected to server");
        return 1;
    }
    char *serialized = serialize_request(&request);
    __debug_log_str("Serialized is");
    __debug_log_str(serialized);
    if (serialized == NULL) {
        __debug_log_str("communication::send_request: serialized is NULL");
        return 1;
    }
    if (write(server_fifo_fd, serialized, strlen(serialized))) {
        __debug_log_str("communication::send_request: failed to send serialized");
        free(serialized);
        return 1;
    }
    free(serialized);
    return 0;
}

int read_response(tlv_reply_t *buffer) {
    uint8_t type;
    if (read(local_fifo_fd, &type, WIDTH_TYPE) == -1) {
        if (errno == EINTR) {
            return 1;
        }
    }

    op_type_t op_type = (type - '0');
    char len[WIDTH_TLV_LEN + 1];
    len[WIDTH_TLV_LEN] = '\0';
    read(local_fifo_fd, len, WIDTH_TLV_LEN);
    uint32_t length = __ATOUL(len);

    char val[length + 1];
    read(local_fifo_fd, val, length);
    rep_value_t value = deserialize_rep_value(val, op_type);

    buffer->type = op_type;
    buffer->length = length;
    buffer->value = value;
    return 0;
}
