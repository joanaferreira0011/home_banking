#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include "constants.h"
#include <string.h>

struct account{
  int id;
  float balance;
  char salt[SALT_LEN];
  char* hash;
} balance;

struct account create_account(int id, float balance, char password[MAX_PASSWORD_LEN]);

#endif
