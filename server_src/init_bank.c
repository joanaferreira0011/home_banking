#include <pthread.h>
#include <stdio.h>
#include "init_bank.h"


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
