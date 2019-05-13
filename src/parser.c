#include "parser.h"
#include "string.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

int get_bank_init_details(char* command[], struct init_bank* bank_details){
  if(atoi(command[1])>MAX_BANK_OFFICES){
    fprintf(stderr, "Error: to many bank offices\n");
    return -1;
  }
  bank_details->n_bank_offices=atoi( command[1]);
  bank_details->admin_password=command[2];

  return 0;
}
