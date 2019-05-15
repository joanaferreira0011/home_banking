#pragma once

#include <stdint.h>
#include "bank_office.h"
#include "../auxiliary_src/types.h"
#include "account.h"

typedef struct init_bank{
  uint32_t n_bank_offices;
  char* admin_password;
} init_bank_t;

void create_bank_offices(uint32_t n_bank_offices, bank_office_t *offices);

/**
 * @brief Add account to bank accounts array
 *
 * @return int 1 if array is full, 0 if added
 */
int add_account(bank_account_t account);

void create_bank(init_bank_t bank);
