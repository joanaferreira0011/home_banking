#include <pthread.h>
#include <stdio.h>
#include "init_bank.h"


void *PrintHello(void *threadnum)
{
  printf("Thread %d: Hello World!\n",
  *(int *)threadnum);
  pthread_exit(NULL);
}

void create_bank_offices(uint32_t n_bank_offices){
  printf("nbank: %u\n", n_bank_offices);
  int thrarg[n_bank_offices];
  pthread_t threads[n_bank_offices];
  for(uint32_t t=0; t<n_bank_offices; t++){
    thrarg[t] = t+1;
    printf("Creating thread %d\n", t);
    pthread_create(&threads[t], NULL,
    PrintHello,
    &thrarg[t]);
  }

}
