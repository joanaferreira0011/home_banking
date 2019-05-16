#include "parser.h"

int get_bank_init_details(char *command[], init_bank_t *bank_details)
{
  if ((atoi(command[1]) > MAX_BANK_OFFICES) || (atoi(command[1]) < 1))
  {
    fprintf(stderr, "Error: the number of bank offices has to be between 1 and 99.\n");
    return -1;
  }
  else if ((strlen(command[2])> MAX_PASSWORD_LEN) || (strlen(command[2]) < MIN_PASSWORD_LEN))
  {
    fprintf(stderr, "Error: the password must have at least 8 and at most 20 characters.\n");
    return -1;
  }

  bank_details->n_bank_offices = atoi(command[1]);
  bank_details->admin_password = command[2];

  return 0;
}
