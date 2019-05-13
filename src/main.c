#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "parser.h"

int main(int argc, char *argv[])
{
   struct init_bank bank;
   if(get_bank_init_details(argv, &bank)!=-1)
      printf("n: %u, pass: %s\n",bank.n_bank_offices, bank.admin_password);
    return 0;
}
