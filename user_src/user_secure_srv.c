#include "user_secure_srv.h"

void user_open_secure_srv(char* msg)
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