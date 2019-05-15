#include "secure_srv.h"

int create_secure_srv()
{
    int fd, n;
    char str[MAX_MSG_LEN];

    if (mkfifo(SERVER_FIFO_PATH, 0660) < 0)
        if (errno == EEXIST)
            printf("FIFO '/tmp/secure_srv' already exists\n");
        else
            printf("Can't create FIFO\n");
    else
        printf("FIFO '/tmp/secure_srv' sucessfully created\n");

    if ((fd = open(SERVER_FIFO_PATH, O_RDONLY)) != -1)
        printf("FIFO '/tmp/secure_srv' openned in READONLY mode\n");

    do
    {
        n = read(fd, str, MAX_MSG_LEN);
        if (n > 0)
            printf("%s has arrived\n", str);
    } while (strcmp(str, "SHUTDOWN") != 0);

    close(fd);
    
    if (unlink(SERVER_FIFO_PATH) < 0)
        printf("Error when destroying FIFO '/tmp/secure_srv'\n");
    else
        printf("FIFO '/tmp/secure_srv' has been destroyed\n");

    exit(0);
}