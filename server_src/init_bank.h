#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
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
 * @return 1 if array is full, 0 if added
 */
int add_account(bank_account_t account);

/**
 * @brief All the accounts are set too empty,
 *        adds the admin account and bank offices
 *        and starts the FIFO secure_srv
 *
 * @return
 */
int create_bank(init_bank_t bank);

ret_code_t create_account(int id, float balance, char *password);

int shutdown(tlv_request_t request, bank_account_t admin_account);

ret_code_t process_request(tlv_request_t request);
