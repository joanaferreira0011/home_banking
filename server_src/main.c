#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "parser.h"
#include "secure_srv.h"


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

// while(1){
//   for(int i=0; i< 20; i++){
//     printf("no: %u\n", srv_accounts[i].account.account_id);
//   }
//   sleep(5)
// }


}
