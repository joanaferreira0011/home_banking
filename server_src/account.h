#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "pthread.h"
#include <sys/types.h>
#include "../auxiliary_src/constants.h"
#include "../auxiliary_src/types.h"

#define READ 0
#define WRITE 1

typedef struct srv_account{
  bank_account_t account;
  pthread_mutex_t mut;
} srv_account_t;

/**
 * @brief Creates bank account
 *
 * @return struct bank account with the created account data
 */
//struct bank_account create_account(int id, float balance, char password[MAX_PASSWORD_LEN]);


char *generate_hash(char *str);

char *generate_salt();

#endif
