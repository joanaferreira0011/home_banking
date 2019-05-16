#include "init_bank.h"

//Creates threads for the bank offices
void create_bank_offices(uint32_t n_bank_offices, bank_office_t *offices)
{

  for (uint32_t t = 0; t < n_bank_offices; t++)
  {
    offices[t].number = t + 1;
  }

  for (uint32_t t = 0; t < n_bank_offices; t++)
  {

    pthread_create(&(offices[t].thread), NULL,
                   execute_bank_office,
                   &(offices[t].number));
  }


}

void init_bank_accounts()
{
  for (int i = 0; i < MAX_BANK_ACCOUNTS; i++)
  {
    srv_accounts[i].account_id = EMPTY_BANK_ACCOUNT_ID;
  }
}

void create_bank(init_bank_t bank)
{
  init_bank_accounts();
  pthread_mutex_init(&mut_srv_accounts, NULL);
  bank_account_t admin_account = create_account(ADMIN_ACCOUNT_ID, 0, bank.admin_password);
  add_account(admin_account);
  srv_offices = malloc(bank.n_bank_offices * sizeof(bank_office_t));
  create_bank_offices(bank.n_bank_offices, srv_offices);
  //create_secure_srv();
}

int add_account(bank_account_t account)
{
  for (int i = 0; i < MAX_BANK_ACCOUNTS; i++)
  {
    if (srv_accounts[i].account_id == EMPTY_BANK_ACCOUNT_ID)
    {
      srv_accounts[i] = account;
      return 0;
    }
  }
  return 1;
}
