#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "parser.h"
#include "secure_srv.h"
#include "srv_accounts.h"

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
}
