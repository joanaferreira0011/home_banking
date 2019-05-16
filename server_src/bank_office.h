#pragma once
#include <pthread.h>

/**
 * @brief Bank office information.
 */
typedef struct bank_office
{
  uint32_t number;
  pthread_t thread;
} bank_office_t;
