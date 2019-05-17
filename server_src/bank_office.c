#include "bank_office.h"
#include "init_bank.h"
#include <stdio.h>


void *execute_bank_office(void *threadnum)
{
  // printf("Thread %d: Hello World!\n",
  //        *(int *)threadnum);
  // fflush(stdout);
  // pthread_exit(NULL);

  printf("Started thread %d\n",
         *(int *)threadnum);
     //pthread_mutex_lock(&mut_srv_accounts);
     // if (pos >= npos) {
     // pthread_mutex_unlock(&mut_srv_accounts);
     // return NULL;
     // }
     printf("thread %d entered critical zone\n",
            *(int *)threadnum);

     //create_account(*(uint32_t *)threadnum,0 , "oijsbhds");

     add_account(srv_accounts[ADMIN_ACCOUNT_ID].account);
     printf("thread %d added account\n",*(int *)threadnum);
     //pthread_mutex_unlock(&mut_srv_accounts);


     return NULL;
}
