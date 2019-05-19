#include "bank_office.h"
#include "init_bank.h"
#include <stdio.h>


void *execute_bank_office(void *threadnum)
{


  printf("Started thread %d\n", *(int *)threadnum);

     tlv_request_t request;
     while(1){
       sem_wait(&srv_request_queue_full);
       printf("thread %d entered critical zone\n", *(int *)threadnum);
       fflush(stdout);
       request= pop(srv_request_queue);
       process_request(request);
       sem_post(&srv_request_queue_empty);
     }
     //pthread_mutex_unlock(&mut_srv_accounts);


     return NULL;
}
