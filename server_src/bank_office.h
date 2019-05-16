#pragma once
#include <pthread.h>
#include  <stdint.h>

/**
 * @brief Bank office information.
 */
typedef struct bank_office
{
  uint32_t number;
  pthread_t thread;
} bank_office_t;

void *execute_bank_office(void *threadnum);
