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

  create_bank(bank);

  int fd, n;
  tlv_request_t request;
  ret_code_t rc = RC_OK;

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
    n = read(fd, &request.type, sizeof(op_type_t));
    if (n > 0)
    {
      read(fd, &request.length, sizeof(uint32_t));
      read(fd, &request.value, request.length);
      process_request(request);
    }
  } while (rc != shutdown(request));

  close(fd);

  if (unlink(SERVER_FIFO_PATH) < 0)
    printf("Error when destroying FIFO '/tmp/secure_srv'\n");
  else
    printf("FIFO '/tmp/secure_srv' has been destroyed\n");

  exit(0);

  // while(1){
  //   for(int i=0; i< 20; i++){
  //     printf("no: %u\n", srv_accounts[i].account.account_id);
  //   }
  //   sleep(5)
  // }
}
