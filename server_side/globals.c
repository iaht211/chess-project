#include <pthread.h>
#include "globals.h"


struct User users[100];
struct Room rooms[100];
int numbers = 0;
int roomNumbers = 0;

// Waiting player conditional variable
pthread_mutex_t general_mutex;