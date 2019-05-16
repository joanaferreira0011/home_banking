#include "communication.h"
#include "stdio.h"
#include "protocol.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"

static int server_fifo_fd = -1;
static int local_fifo_fd = -1;

int open_connection_to_srv() {
    if (server_fifo_fd != -1) {
        return 1;
    }
    if ((server_fifo_fd = open(SERVER_FIFO_PATH, O_WRONLY)) == -1) {
        return 1;
    }
    return 0;
}

int close_connection_to_srv() {
    if (server_fifo_fd == -1) {
        return 1;
    }
    if (close(server_fifo_fd) == -1) {
        return 1;
    }
    server_fifo_fd = -1;
    return 0;
}

void close_local_fifo(void) {
    if (local_fifo_fd != -1) {
        close(local_fifo_fd);
    }
}

int open_local_fifo() {
    if (local_fifo_fd != -1) {
        return 1;
    }
    char path[USER_FIFO_PATH_LEN];
    snprintf(path, USER_FIFO_PATH_LEN, "%s%05d", USER_FIFO_PATH_PREFIX, getpid());
    if ((local_fifo_fd = mkfifo(path, 0660)) == -1) {
        return 1;
    }
    if (atexit(close_local_fifo)) {
        close(local_fifo_fd);
        return 1;
    }
    return 0;
}



int send_request(tlv_request_t request) {
    if (server_fifo_fd == -1) {
        return 1;
    }
    char *serialized = serialize_request(request);
    if (dprintf(server_fifo_fd, "%s",serialized) == 0) {
        free(serialized);
        return 1;
    }
    free(serialized);
    return 0;
}

int read_response(char *response_str) {
    return 0;
}

void user_open_secure_srv(char *msg)
{
    int fd;
    fd = open(SERVER_FIFO_PATH, O_WRONLY);

    if (fd == -1)
    {
        printf("Oops !!! Server is closed !!!\n");
        exit(1);
    }

    printf("FIFO 'secure' openned in WRITEONLY mode\n");

    write(fd, msg, strlen(msg) + 1);
    close(fd);
}