#include "bank_office.h"
#include "init_bank.h"
#include <stdio.h>


void *execute_bank_office(void *threadnum)
{


     tlv_request_t request;
     while(!end){
       sem_wait(&srv_request_queue_full);
       printf("caught %u\n", getpid());
       request= pop(srv_request_queue);
       process_request(request);
       fflush(stdout);
       sem_post(&srv_request_queue_empty);
     }
    pthread_exit(NULL);

    return NULL;

}
