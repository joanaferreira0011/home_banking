#include "account.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define READ 0
#define WRITE 1

void sig_pipe(int signo)
{
  printf("SIGPIPE caught\n");
  exit(1);
}

void err_sys(char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

void err_msg(char *msg)
{
  printf("%s\n", msg);
  return;
}

char *generate_salt()
{
  char *salt = malloc(sizeof(char) * SALT_LEN);
  const char *hex_digits = "0123456789ABCDEF";
  char character[2];
  int i;

  for (i = 0; i < SALT_LEN; i++)
  {
    character[0] = hex_digits[(rand() % 16)];
    fflush(stdout);
    strcat(salt, character);
    fflush(stdout);
  }

  return salt;
}

char *generate_hash(char *str)
{
  char *hash = malloc(sizeof(char) * (HASH_LEN);

  int fd1[2], fd2[2];
  pid_t pid;

  if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
    err_sys("signal error");

  if (pipe(fd1) < 0 || pipe(fd2) < 0)
    err_sys("pipe error");

  if ((pid = fork()) < 0)
    err_sys("fork error");

  if (pid == 0) /* child*/
  {
    close(fd2[WRITE]);
    close(fd1[READ]);
    dup2(fd2[READ], STDIN_FILENO);
    dup2(fd1[WRITE], STDOUT_FILENO);
    execlp("sha256sum", "sha256sum", NULL);
    exit(0);
  }

  else /*parent*/
  {
    close(fd1[WRITE]);
    close(fd2[READ]);
    write(fd2[WRITE], str, HASH_LEN);
    close(fd2[WRITE]);
    read(fd1[READ], hash, HASH_LEN);
    hash[HASH_LEN] = '\0';
  }

  return hash;
}

struct bank_account create_account(int id, float balance, char password[MAX_PASSWORD_LEN])
{
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

  return a;
}
