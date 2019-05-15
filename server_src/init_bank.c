#include <pthread.h>
#include <stdio.h>
#include "init_bank.h"
#include "srv_accounts.h"
#include <stdlib.h>
#include "secure_srv.h"

void *PrintHello(void *threadnum)
{
  printf("Thread %d: Hello World!\n",
  *(int *)threadnum);
  fflush(stdout);
  pthread_exit(NULL);
}

void create_bank_offices(uint32_t n_bank_offices, bank_office_t *offices){

  for(uint32_t t=0; t<n_bank_offices; t++){
    offices[t].number=t+1;
  }

  for(uint32_t t=0; t<n_bank_offices; t++){

    pthread_create(&(offices[t].thread), NULL,
    PrintHello,
    &(offices[t].number));
  }

}

void init_bank_accounts(){
  for(int i=0; i< MAX_BANK_ACCOUNTS; i++){
     srv_accounts[i].account_id= EMPTY_BANK_ACCOUNT_ID;
  }
}

int add_account(bank_account_t account){
  for(int i=0; i< MAX_BANK_ACCOUNTS; i++){
     if (srv_accounts[i].account_id== EMPTY_BANK_ACCOUNT_ID ){
       srv_accounts[i]= account;
       return 0;
     }
  }
  return 1;
}


void create_bank(init_bank_t bank){
  init_bank_accounts();
  bank_account_t admin_account =create_account(ADMIN_ACCOUNT_ID,0, bank.admin_password);
  add_account(admin_account);
  srv_offices = malloc(bank.n_bank_offices*sizeof(bank_office_t));
  create_bank_offices(bank.n_bank_offices, srv_offices);
  //create_secure_srv();
}
