#include "init_bank.h"
#include "../auxiliary_src/sope.h"

bool end = false;
int server_logfile;

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
    logBankOfficeOpen(server_logfile, offices[t].number, offices[t].thread);
  }
}


void close_bank_offices(){

  for(int i=0; i<n_srv_offices; i++){

    pthread_join(srv_offices[i].thread, NULL);

    logBankOfficeClose(server_logfile, srv_offices[i].number, srv_offices[i].thread);
  }
  free(srv_offices);
}


void init_bank_accounts(){
  for (int i = 0; i < MAX_BANK_ACCOUNTS; i++)
  {
    srv_accounts[i].account.account_id = EMPTY_BANK_ACCOUNT_ID;
    pthread_mutex_init(&(srv_accounts[i].mut), NULL);

  }
}

int create_bank(init_bank_t bank)
{
  int sem_value;
  init_bank_accounts();
  srv_request_queue = createQueue(SRV_REQUEST_QUEUE_SIZE);
  sem_init(&srv_request_queue_empty, SHARED, 1); /* sem empty = 1 */
  sem_getvalue(&srv_request_queue_empty, &sem_value);
  logSyncMechSem(server_logfile, 0, SYNC_OP_SEM_INIT, SYNC_ROLE_PRODUCER, 0, sem_value);
  sem_init(&srv_request_queue_full, SHARED, 0);  /* sem full = 0 */
  sem_getvalue(&srv_request_queue_full, &sem_value);
  logSyncMechSem(server_logfile, 0, SYNC_OP_SEM_INIT, SYNC_ROLE_PRODUCER, 0, sem_value);
  create_account(ADMIN_ACCOUNT_ID, 0, bank.admin_password, 0);

  srv_offices = (bank_office_t*) malloc(bank.n_bank_offices * sizeof(bank_office_t));
  n_srv_offices= bank.n_bank_offices;
  create_bank_offices(bank.n_bank_offices, srv_offices);

  return 0;
}

int add_account(bank_account_t account)
{
  logSyncMech(server_logfile, pthread_self(), SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, account.account_id);
  pthread_mutex_lock(&srv_accounts[account.account_id].mut);


  if (srv_accounts[account.account_id].account.account_id == EMPTY_BANK_ACCOUNT_ID)
  {
    srv_accounts[account.account_id].account = account;
    logAccountCreation(server_logfile, pthread_self(), &account);
    pthread_mutex_unlock(&srv_accounts[account.account_id].mut);
    logSyncMech(server_logfile, pthread_self(), SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, account.account_id);
    return 0;
  }
  else
  {
    pthread_mutex_unlock(&srv_accounts[account.account_id].mut);
    logSyncMech(server_logfile, pthread_self(), SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, account.account_id);
    return 1;
  }
}

int remove_account(uint32_t id)
{
  logSyncMech(server_logfile, pthread_self(), SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, id);
  pthread_mutex_lock(&srv_accounts[id].mut);
  if (srv_accounts[id].account.account_id == EMPTY_BANK_ACCOUNT_ID)
  {
    pthread_mutex_unlock(&srv_accounts[id].mut);
    pthread_mutex_unlock(&srv_accounts[id].mut);
    return 1;
  }
  else
  {
    pthread_mutex_unlock(&srv_accounts[id].mut);
    srv_accounts[id].account.account_id = EMPTY_BANK_ACCOUNT_ID;
    pthread_mutex_unlock(&srv_accounts[id].mut);
    return 0;
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

ret_code_t create_account(int id, float balance, char password[MAX_PASSWORD_LEN], uint32_t delay)
{
  usleep(delay);
  logSyncDelay(server_logfile, pthread_self(), id, delay);


    if (srv_accounts[id].account.account_id != EMPTY_BANK_ACCOUNT_ID)
      return RC_ID_IN_USE;


  if (balance > MAX_BALANCE || balance < MIN_BALANCE ||
      strlen(password) > MAX_PASSWORD_LEN || strlen(password) < MIN_PASSWORD_LEN)
    return RC_OTHER;


  bank_account_t a;
  a.account_id = id;
  a.balance = balance;
  fflush(stdout);
  strcpy(a.salt, generate_salt());
  fflush(stdout);
  char *aux = a.salt;
  strcat(aux, password);
  strcpy(a.hash, generate_hash(aux));
  printf("g added account \n");
  fflush(stdout);
  add_account(a);
  printf("added account \n");
  fflush(stdout);

  return RC_OK;
}

ret_code_t check_balance(uint32_t id, uint32_t delay, uint32_t *balance_buffer)
{
  if (id == ADMIN_ACCOUNT_ID)
    return RC_OP_NALLOW;


  pthread_mutex_lock(&srv_accounts[id].mut);
  logSyncMech(server_logfile, pthread_self(), SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, id);
  if(srv_accounts[id].account.account_id == EMPTY_BANK_ACCOUNT_ID)
    return RC_ID_NOT_FOUND;
  usleep(delay);
  (*balance_buffer) = srv_accounts[id].account.balance;
  pthread_mutex_unlock(&srv_accounts[id].mut);
  logSyncMech(server_logfile, pthread_self(), SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, id);
  return RC_OK;
}

ret_code_t transfer(uint32_t src, u_int32_t dest, uint32_t amount, uint32_t delay, uint32_t  *balance_buffer)
{
  if (src == ADMIN_ACCOUNT_ID)
    return RC_OP_NALLOW;

  usleep(delay);
  bool check_dest = false;
  for (int i = 0; i <= MAX_BANK_ACCOUNTS; i++)
  {
    if (srv_accounts[i].account.account_id == dest)
      check_dest = true;
  }

  if (!check_dest)
    return RC_ID_NOT_FOUND;

  if (src == dest)
    return RC_SAME_ID;

  if (src < dest) {
    pthread_mutex_lock(&srv_accounts[src].mut);
    pthread_mutex_lock(&srv_accounts[dest].mut);
  } else {
    pthread_mutex_lock(&srv_accounts[dest].mut);
    pthread_mutex_lock(&srv_accounts[src].mut);
  }

  if (srv_accounts[src].account.balance < amount)
  {
    pthread_mutex_unlock(&srv_accounts[src].mut);
    pthread_mutex_unlock(&srv_accounts[dest].mut);
    return RC_NO_FUNDS;
  }

  if (srv_accounts[dest].account.balance + amount > MAX_BALANCE)
    return RC_TOO_HIGH;

  srv_accounts[src].account.balance -= amount;
  srv_accounts[dest].account.balance += amount;
  (*balance_buffer) = srv_accounts[src].account.balance;

  if (src < dest) {
    pthread_mutex_unlock(&srv_accounts[src].mut);
    pthread_mutex_unlock(&srv_accounts[dest].mut);
  } else {
    pthread_mutex_unlock(&srv_accounts[dest].mut);
    pthread_mutex_unlock(&srv_accounts[src].mut);
  }

  return RC_OK;
}




ret_code_t shutdown(tlv_request_t request)
{
  if (request.value.header.account_id != ADMIN_ACCOUNT_ID)
    return RC_OP_NALLOW;

  if (request.type == OP_SHUTDOWN)
  {
    if (!(verify_account(request.value.header.account_id, request.value.header.password, srv_accounts[ADMIN_ACCOUNT_ID].account)))
      return RC_OTHER;
  }

  usleep(request.value.header.op_delay_ms);
  end = true;
  chmod(SERVER_FIFO_PATH, S_IRUSR|S_IRGRP|S_IROTH);

  return RC_OK;
}

ret_code_t process_request(tlv_request_t request)
{
  tlv_reply_t reply;
  char* fifo_name = malloc(USER_FIFO_PATH_LEN + 1);
  snprintf(fifo_name, USER_FIFO_PATH_LEN, "%s%05d", USER_FIFO_PATH_PREFIX,request.value.header.pid);

  int fd_user = open(fifo_name, O_WRONLY);

  if (!(verify_account(request.value.header.account_id, request.value.header.password, srv_accounts[ADMIN_ACCOUNT_ID].account))){
    reply.value.header.ret_code = RC_LOGIN_FAIL;
    printf("wrong login\n");
  }
  switch (request.type)
  {
  case OP_CREATE_ACCOUNT:
    if (request.value.header.account_id != ADMIN_ACCOUNT_ID){
      printf("not admin\n");
      reply.value.header.ret_code = RC_OP_NALLOW;
    }
    else{
      printf("oi\n");
      reply.value.header.ret_code = create_account(request.value.create.account_id, request.value.create.balance, request.value.create.password, request.value.header.op_delay_ms);
    }
    reply.length = sizeof(rep_header_t);
    reply.type = OP_CREATE_ACCOUNT;
    reply.value.header.account_id = request.value.create.account_id;
    break;
  case OP_BALANCE:
    reply.value.header.ret_code = check_balance(request.value.create.account_id,
      request.value.header.op_delay_ms, &(reply.value.balance.balance));
    reply.length = sizeof(rep_header_t) + sizeof(rep_balance_t);
    reply.type = OP_BALANCE;
    reply.value.header.account_id = request.value.header.account_id;
    break;
  case OP_TRANSFER:
    reply.value.header.ret_code = transfer(request.value.header.account_id,
      request.value.transfer.account_id, request.value.transfer.amount,
      request.value.header.op_delay_ms, &(reply.value.transfer.balance));
    reply.length = sizeof(rep_header_t) + sizeof(rep_transfer_t);
    reply.type = OP_TRANSFER;
    reply.value.header.account_id = request.value.header.account_id;
    break;
  case OP_SHUTDOWN:
    reply.value.header.ret_code = shutdown(request);
    reply.length = sizeof(rep_header_t) + sizeof(rep_shutdown_t);
    reply.type = OP_SHUTDOWN;
    reply.value.header.account_id = request.value.header.account_id;
    int active_offices = 0; //PRECISA DE IMPLEMENTAÇÃO
    reply.value.shutdown.active_offices = active_offices;
    break;
  default:
    break;
  }

  write(fd_user, &reply, sizeof(tlv_reply_t));
  logReply(server_logfile, pthread_self(), &reply);
  close(fd_user);
  return reply.value.header.ret_code;
}
