#pragma once

#include <stdint.h>
#include "bank_office.h"

typedef struct init_bank{
  uint32_t n_bank_offices;
  char* admin_password;
} init_bank_t;


void create_bank_offices(uint32_t n_bank_offices, bank_office_t *offices);
