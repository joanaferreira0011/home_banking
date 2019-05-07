#include <stdlib.h>
#include <stdio.h>
#include "account.h"

FILE *executeCommand(char *file, char *command)
{
  FILE *aux = NULL;
  char cmd[256];
  strcpy(cmd, command);
  strcat(cmd, file);
  aux = popen(cmd, "r");

  return aux;
}

char *getSha256Sum(char *str)
{
  char command[] = "sha256sum ";
  FILE *sha256sum = executeCommand(str, command);
  char aux[256] = "";
  fgets(aux, 255, sha256sum);

  char *sha256 = strndup(aux, strlen(aux) - strlen(str) - 3);

  char *pos;
  if ((pos = strchr(sha256, '\n')) != NULL)
    *pos = '\0';

  fclose(sha256sum);
  return sha256;
}

char *generate_salt()
{
  char *salt = malloc (sizeof (char) * SALT_LEN);
  const char *hex_digits = "0123456789ABCDEF";
  char character[2];
  int i;

  for (i = 0; i < SALT_LEN; i++)
  {
    character[0] = hex_digits[(rand() % 16)];
    //printf("%s\n", character);
    fflush(stdout);
    strcat(salt, character);
    //printf("%s\n", salt);
    fflush(stdout);
  }

  return salt;
}

struct account create_account(int id, float balance, char password[MAX_PASSWORD_LEN])
{
  printf("enter");
  fflush(stdout);
  struct account a;
  a.id = id;
  a.balance = balance;
  printf("balance");
  fflush(stdout);
  strcpy(a.salt, generate_salt());
  printf("salt");
  fflush(stdout);
  char * aux = a.salt;
  strcat(aux, password);
  a.hash = getSha256Sum(aux);

  return a;
}
