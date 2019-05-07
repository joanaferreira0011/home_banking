#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__
#include <string.h>
#define WIDTH_ACCOUNT 4
#define WIDTH_BALANCE 10
#define  MAX_PASSWORD_LEN 20
#define SALT_LEN 64

struct account{
  int id;
  float balance;
  char salt[SALT_LEN];
  char* hash;
} balance;

struct account create_account(int id, float balance, char password[MAX_PASSWORD_LEN]);

#endif
