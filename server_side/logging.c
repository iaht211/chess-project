#include <stdio.h>
#include "globals.h"
#include "logging.h"
#include "user.h"

void setLogEndGame(int a, int b, int status) {
    char *user1, *user2;
    for (int i = 0; i < numbers; i++) {
        if (users[i].client_socket == a && users[i].ongame == true) {
            user1 = users[i].name;
        }
        if (users[i].client_socket == b && users[i].ongame == true) {
            user2 = users[i].name;
        }
    }
    logEndGame(user1, user2, status);
}

void setLogStart(int a, int b) {
    char *user1, *user2;
    char logFileName[256];
    for (int i = 0; i < numbers; i++) {
        if (users[i].client_socket == a && users[i].ongame == true) {
            user1 = users[i].name;
        }
        if (users[i].client_socket == b && users[i].ongame == true) {
            user2 = users[i].name;
        }
    }
    snprintf(logFileName, sizeof(logFileName), "%s_%s.txt", user1, user2);
    logStart(user1, user2, logFileName);
}

void setLogOnGame(int a, int b, char *buffer, int status) {
    char *user1, *user2;
    char logFileName[256];
    buffer[5] = '\0';
    for (int i = 0; i < numbers; i++) {
        if (users[i].client_socket == a && users[i].ongame == true) {
            user1 = users[i].name;
        }
        if (users[i].client_socket == b && users[i].ongame == true) {
            user2 = users[i].name;
        }
    }
    snprintf(logFileName, sizeof(logFileName), "%s_%s.txt", user1, user2);
    if (status == 1) {
        logOnGame(user1, buffer, logFileName);
    } else {
        logOnGame(user2, buffer, logFileName);
    }
}