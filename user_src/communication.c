#include "communication.h"

int open_connection_to_srv() {
    return 0;
}

int open_local_fifo() {
    return 0;
}

int send_request(tlv_request_t request) {
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