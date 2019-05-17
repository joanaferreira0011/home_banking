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

int create_bank(init_bank_t bank)
{
  init_bank_accounts();

  create_account(ADMIN_ACCOUNT_ID, 0, bank.admin_password);
  srv_offices = malloc(bank.n_bank_offices * sizeof(bank_office_t));
  create_bank_offices(bank.n_bank_offices, srv_offices);

  return 0;
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

int verify_account(uint32_t id, char *password, bank_account_t acc)
{
  char *hash = malloc(sizeof(char) * HASH_LEN);

  char *aux = acc.salt;
  strcat(aux, password);

  hash = generate_hash(aux);

  if (id == acc.account_id && strcmp(hash, acc.hash) == 0)
    return 0;

  return -1;
}

ret_code_t create_account(int id, float balance, char password[MAX_PASSWORD_LEN])
{
  for (int i = 0; i <= MAX_BANK_ACCOUNTS; i++)
  {
    if (srv_accounts[i].account.account_id == id)
      return RC_ID_IN_USE;
  }

  if (balance > MAX_BALANCE || balance < MIN_BALANCE ||
      strlen(password) > MAX_PASSWORD_LEN || strlen(password) < MIN_PASSWORD_LEN)
    return RC_OTHER;

  fflush(stdout);
  bank_account_t a;
  a.account_id = id;
  a.balance = balance;
  fflush(stdout);
  strcpy(a.salt, generate_salt());
  fflush(stdout);
  char *aux = a.salt;
  strcat(aux, password);
  strcpy(a.hash, generate_hash(aux));

  add_account(a);
  return RC_OK;
}

ret_code_t check_balance(uint32_t id)
{
  if (id == ADMIN_ACCOUNT_ID)
    return RC_OP_NALLOW;

  pthread_mutex_lock(&srv_accounts[id].mut);
  uint32_t balance = srv_accounts[id].account.balance;
  pthread_mutex_unlock(&srv_accounts[id].mut);
  return RC_OK;
}

ret_code_t transfer(uint32_t src, u_int32_t dest, uint32_t amount)
{

  if (src == ADMIN_ACCOUNT_ID)
    return RC_OP_NALLOW;

  bool check_dest = false;
  for (int i = 0; i <= MAX_BANK_ACCOUNTS; i++)
  {
    if (srv_accounts[i].account.account_id == dest)
      check_dest = true;
  }

  if (!check_dest)
    return RC_ID_NOT_FOUND;

  pthread_mutex_lock(&srv_accounts[src].mut);
  pthread_mutex_lock(&srv_accounts[dest].mut);

  if (srv_accounts[src].account.balance < amount)
  {
    pthread_mutex_unlock(&srv_accounts[src].mut);
    pthread_mutex_unlock(&srv_accounts[dest].mut);
    return RC_NO_FUNDS;
  }

  srv_accounts[src].account.balance = srv_accounts[src].account.balance - amount;
  srv_accounts[dest].account.balance = srv_accounts[dest].account.balance + amount;
  pthread_mutex_unlock(&srv_accounts[src].mut);
  pthread_mutex_unlock(&srv_accounts[dest].mut);
  return RC_OK;
}

int shutdown(tlv_request_t request, bank_account_t admin_account)
{
  if (request.type == OP_SHUTDOWN)
  {
    if (verify_account(request.value.header.account_id, request.value.header.password, admin_account))
      return 0;
  }

  return -1;
}

ret_code_t process_request(tlv_request_t request)
{
  ret_code_t reply;

  if (!(verify_account(request.value.header.account_id, request.value.header.password, srv_accounts[ADMIN_ACCOUNT_ID].account)))
    reply = RC_LOGIN_FAIL;

  switch (request.type)
  {
  case OP_CREATE_ACCOUNT:
    if (request.value.header.account_id != ADMIN_ACCOUNT_ID)
      reply = RC_OP_NALLOW;
    else
      reply = create_account(request.value.create.account_id, request.value.create.balance, request.value.create.password);
    break;
  case OP_BALANCE:
    reply = check_balance(request.value.create.account_id);
    break;
  case OP_TRANSFER:
    reply = transfer(request.value.header.account_id, request.value.transfer.account_id, request.value.transfer.amount);
    break;
  default:
    break;
  }

  return reply;
}
