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
    srv_accounts[i].account.account_id = EMPTY_BANK_ACCOUNT_ID;
    pthread_mutex_init(&(srv_accounts[i].mut), NULL);
  }
}

void create_bank(init_bank_t bank)
{
  init_bank_accounts();

  bank_account_t admin_account = create_account(ADMIN_ACCOUNT_ID, 0, bank.admin_password);
  add_account(admin_account);
  srv_offices = malloc(bank.n_bank_offices * sizeof(bank_office_t));
  create_bank_offices(bank.n_bank_offices, srv_offices);
  //create_secure_srv();
}

int add_account(bank_account_t account)
{
  pthread_mutex_lock(&srv_accounts[account.account_id].mut);
  if (srv_accounts[account.account_id].account.account_id == EMPTY_BANK_ACCOUNT_ID)
  {
    srv_accounts[account.account_id].account = account;
    pthread_mutex_unlock(&srv_accounts[account.account_id].mut);
    return 0;
  }
  else
  {
    pthread_mutex_unlock(&srv_accounts[account.account_id].mut);
    return 1;
  }
}

uint32_t check_balance(uint32_t id)
{
  return srv_accounts[id].account.balance;
}

int transfer(uint32_t src, u_int32_t dest, uint32_t amount)
{
  if (check_balance(src) < amount)
    return -1;

  srv_accounts[src].account.balance = srv_accounts[src].account.balance - amount;
  srv_accounts[dest].account.balance = srv_accounts[dest].account.balance + amount;
}