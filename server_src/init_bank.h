#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "account.h"
#include "bank_office.h"
#include "srv_accounts.h"
#include "../auxiliary_src/types.h"

extern pthread_mutex_t mut_srv_accounts;
/**
 * @brief Init bank account information.
 */
typedef struct init_bank
{
  uint32_t n_bank_offices;
  char *admin_password;
} init_bank_t;

/**
 * @brief Add account to bank accounts array
 *
 * @return 1 if there is already an account with same name, 0 if added
 */
int add_account(bank_account_t account);

/**
 * @brief Remove account from bank accounts array
 *
 * @return 1 if account does not exist
 */
int remove_account(uint32_t id);
/**
 * @brief All the accounts are set too empty,
 *        adds the admin account and bank offices
 *        and starts the FIFO secure_srv
 *
 * @return
 */
bank_account_t create_bank(init_bank_t bank);

int shutdown(tlv_request_t request, bank_account_t admin_account);
