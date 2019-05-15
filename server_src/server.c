#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "parser.h"

int main(int argc, char *argv[])
{
  init_bank_t bank;
  if(get_bank_init_details(argv, &bank)==-1){
    fprintf(stderr, "Error: to many bank offices\n");
    return -1;
  }

  bank_account_t admin_account =create_account(ADMIN_ACCOUNT_ID,0, bank.admin_password);
  bank_office_t *offices = malloc(bank.n_bank_offices*sizeof(bank_office_t));
  create_bank_offices(bank.n_bank_offices, offices);

  return 0;
}
