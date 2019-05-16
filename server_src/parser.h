#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "init_bank.h"
#include "../auxiliary_src/constants.h"

/**
 * @brief Fills the init_bank structure with the data provided by the command line
 *
 * @return 0 if sucessful and -1 otherwise 
 */
int get_bank_init_details(char *command[], init_bank_t *bank_details);
