#ifndef USER_H
#define USER_H

#include <arpa/inet.h>
#include <locale.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

int checkLogin(char *username, char *password);
int registerAccount(char *username, char *password);
void logLogin(char *username);
void logEndGame(char *username1, char *username2, int status);
void logStart(char *username1, char *username2, char *filename);
void logOnGame(char *username, char *buffer, char *filename);
int changePassword(char *username, char *oldPassword, char *newPassword);

#endif // USER_H