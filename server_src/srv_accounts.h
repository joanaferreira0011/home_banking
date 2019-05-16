#pragma once
#include <pthread.h>
bank_account_t srv_accounts[MAX_BANK_ACCOUNTS];
pthread_mutex_t mut_srv_accounts; 

bank_office_t *srv_offices;
