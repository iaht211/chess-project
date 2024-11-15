#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <pthread.h>

struct User {
    char name[24];
    int client_socket;
    bool ongame;
};

struct Room {
    int client1;
    int client2;
};

extern struct User users[100];
extern struct Room rooms[100];
extern int numbers;
extern int roomNumbers;
extern pthread_mutex_t general_mutex;

#endif // GLOBALS_H