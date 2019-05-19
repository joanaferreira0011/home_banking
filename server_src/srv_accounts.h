#pragma once
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"

srv_account_t srv_accounts[MAX_BANK_ACCOUNTS];
pthread_mutex_t mut_srv_accounts;

bank_office_t *srv_offices;
int n_srv_offices;

struct Queue *srv_request_queue;
sem_t srv_request_queue_empty, srv_request_queue_full;
