#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "parser.h"
#include "init_bank.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "../auxiliary_src/constants.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: %s <n_bank_offices> <admin_password>\n", argv[0]);
    return -1;
  }

  init_bank_t bank;

  if (get_bank_init_details(argv, &bank) == -1)
    exit(EXIT_FAILURE);

  server_logfile = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
  create_bank(bank);

  int fd, n;
  tlv_request_t request;

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
    n = read(fd, &request, sizeof(tlv_request_t));
    if (n ==sizeof(tlv_request_t))
    {


      sem_wait(&srv_request_queue_empty);
      push(srv_request_queue, request);
      // process_request(request);
      sem_post(&srv_request_queue_full);
    }
    sleep(5);
  } while (!end);

  for (int i = 0; i < atoi(argv[1]); i++)
  {
    pthread_join(srv_offices[i].thread, NULL);
  }

  close(fd);

  if (unlink(SERVER_FIFO_PATH) < 0)
    printf("Error when destroying FIFO '/tmp/secure_srv'\n");
  else
    printf("FIFO '/tmp/secure_srv' has been destroyed\n");


  exit(0);
}
