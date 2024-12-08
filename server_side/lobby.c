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

#include "globals.h"

int lobby(int player, char *username) {
    while (1) {
        char lobbychoose[9];
        bzero(lobbychoose, 9);
        if (read(player, lobbychoose, 8) < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        lobbychoose[8] = '\0';
        if (strcmp(lobbychoose, "cre-room") == 0) {
            send(player, "cre-true", 8, 0);
            printf("Create Room\n");
            return 1;
        } else if (strcmp(lobbychoose, "changepa") == 0) {
            char dataPassword[48];
            bzero(dataPassword, 48);
            if (read(player, dataPassword, 48) < 0) {
                perror("ERROR reading from socket");
                exit(1);
            }
            printf("Change Password: %s\n", dataPassword);
            char *oldPassword = strtok(dataPassword, " ");
            char *newPassword = strtok(NULL, " ");

            if (changePassword(username, oldPassword, newPassword)) {
                send(player, "t", 1, 0);
            } else {
                send(player, "f", 1, 0);
            }
        } else if (strcmp(lobbychoose, "log--out") == 0) {
            send(player, "log-true", 8, 0);
            printf("Logout\n");
            return 2;
        } else if (strcmp(lobbychoose, "waitting") == 0) {
            while (1) {
                bzero(lobbychoose, 6);
                if (read(player, lobbychoose, 6) < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                lobbychoose[6] = '\0';
                printf("Buffer waitInvite: %s\n", lobbychoose);
                if (strcmp(lobbychoose, "accept") == 0) {
                    // Chap nhan loi moi (accept)
                    char opponent[24];
                    bzero(opponent, 24);
                    if (read(player, opponent, 24) < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }

                    char *token;
                    token = strtok(opponent, "\n");
                    for (int i = 0; i < numbers; i++) {
                        if (strcmp(users[i].name, token) == 0 && users[i].ongame == false) {
                            send(users[i].client_socket, "accept", 6, 0);
                            break;
                        }
                    }
                    return 3;
                } else if (strcmp(lobbychoose, "refuse") == 0) {
                    // Tu choi loi moi (refuse)
                    char opponent[24];
                    bzero(opponent, 24);
                    if (read(player, opponent, 24) < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    char *token;
                    token = strtok(opponent, "\n");
                    for (int i = 0; i < numbers; i++) {
                        if (strcmp(users[i].name, opponent) == 0 && users[i].ongame == false) {
                            send(users[i].client_socket, "refuse", 6, 0);
                        }
                    }
                }
            }
        }
    }
}

int checkLogged(char *username) {
    for (int i = 0; i < numbers; i++) {
        if (strcmp(users[i].name, username) == 0) {
            return 0;
        }
    }
    return 1;
}
