#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include "../auxiliary_src/constants.h"
#include "../auxiliary_src/types.h"
#include <string.h>

struct bank_account create_account(int id, float balance, char password[MAX_PASSWORD_LEN]);

#endif
