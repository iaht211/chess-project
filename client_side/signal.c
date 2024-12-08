#include "signal.h"
#include "board.h"

#include <ncurses.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"
#define GREEN "\x1B[32m"

void *on_signal(void *sockfd) {
    char buffer[64];
    int n;
    int socket = *(int *)sockfd;
    int *player = (int *)malloc(sizeof(int *));

    while (1) {
        bzero(buffer, 64);
        n = read(socket, buffer, 64);

        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        if (buffer[0] == 'i' || buffer[0] == 'e' || buffer[0] == '\0') {
            if (buffer[0] == 'i') {
                if (buffer[2] == 't') {
                    printf("\nMake your move: \n");
                }
                if (buffer[2] == 'n') {
                    printf("\nWaiting for opponent...\n");
                }
                if (buffer[2] == 'p') {
                    *player = atoi(&buffer[3]);
                    if (*player == 2) {
                        printf("You're blacks (%c)\n", buffer[3]);
                    } else {
                        printf("You're whites (%c)\n", buffer[3]);
                    }
                }
            } else if (buffer[0] == 'e') {
                // Syntax errors
                if (buffer[2] == '0') {
                    switch (buffer[3]) {
                        case '0':
                            printf(RED "  ↑ ('-' missing)\n" RESET);
                            break;
                        case '1':
                            printf(RED "↑ (should be letter)\n" RESET);
                            break;
                        case '2':
                            printf(RED "   ↑ (should be letter)\n" RESET);
                            break;
                        case '3':
                            printf(RED " ↑ (should be number)\n" RESET);
                            break;
                        case '4':
                            printf(RED " ↑ (out of range)\n" RESET);
                            break;
                        case '5':
                            printf(RED "   ↑ (should be number)\n" RESET);
                            break;
                        case '6':
                            printf(RED "   ↑ (out of range)\n" RESET);
                            break;
                        case '7':
                            printf(RED "(out of range)\n" RESET);
                            break;
                    }
                }
                printf("\nerror %s\n", buffer);
            }
            // Check if it's an informative or error message
        } else if (buffer[0] == 'w') {
            printf("You win\n");
            exit(1);
        } else if (buffer[0] == 'l') {
            printf("You lose\n");
            exit(1);
        } else if (buffer[0] == 'b') {
            printf("Your opponent has left the room. Please leave here\n");
            exit(1);
        } else if (buffer[0] == 'g' && buffer[2] == 'e') {
            printf("The opponent surrendered. Yes or No\n");
        } else if (buffer[0] == 'g' && buffer[2] == 'o') {
            printf("You lose\n");
            exit(1);
        } else if (buffer[0] == 'g' && buffer[2] == 'n') {
            printf("The opponent refused. Let's move\n");
            continue;
        } else {
            // Print the board
            system("clear");
            if (*player == 1) {
                print_board_buff(buffer);
            } else {
                print_board_buff_inverted(buffer);
            }
        }

        bzero(buffer, 64);
    }
}
