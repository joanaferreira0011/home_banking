#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "parser.h"

int main(int argc, char *argv[])
{
  struct init_bank bank;
  if(get_bank_init_details(argv, &bank)==-1){
    fprintf(stderr, "Error: to many bank offices\n");
    return -1;
  }

  struct bank_account admin_account =create_account(ADMIN_ACCOUNT_ID,0, bank.admin_password);
  create_bank_offices(bank.n_bank_offices);
  return 0;
}
