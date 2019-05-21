#include "bank_office.h"
#include "init_bank.h"
#include "../auxiliary_src/sope.h"
#include <stdio.h>


void *execute_bank_office(void *threadnum)
{


     tlv_request_t request;
     int sem_value;
     while(!end){

       sem_wait(&srv_request_queue_full);

       request= pop(srv_request_queue);
       logRequest(server_logfile, pthread_self(), &request);
       sem_getvalue(&srv_request_queue_empty, &sem_value);
       logSyncMechSem(server_logfile,  pthread_self(), SYNC_OP_SEM_WAIT, SYNC_ROLE_CONSUMER, request.value.header.pid, sem_value);
       process_request(request);

       sem_post(&srv_request_queue_empty);
       sem_getvalue(&srv_request_queue_full, &sem_value);

       logSyncMechSem(server_logfile,  pthread_self(), SYNC_OP_SEM_POST, SYNC_ROLE_CONSUMER, request.value.header.pid, sem_value);

     }
    pthread_exit(NULL);

    return NULL;

}
