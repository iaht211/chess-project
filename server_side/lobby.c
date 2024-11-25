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

void *game_room()
{
    pthread_mutex_lock(&general_mutex);
    int player_one = rooms[roomNumbers].client1;
    int player_two = rooms[roomNumbers].client2;
    pthread_mutex_unlock(&general_mutex);
    int n;
    printf("%d-%d", player_one, player_two);
    char buffer[64];
    int *move = (int *)malloc(sizeof(int) * 4);

    // Create a new board
    wchar_t **board = create_board();
    char *one_dimension_board = create_od_board();
    initialize_board(board);

    if (send(player_one, "i-p1", 4, 0) < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    if (send(player_two, "i-p2", 4, 0) < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    sleep(1);

    // Broadcast the board to all the room players
    broadcast(board, one_dimension_board, player_one, player_two);

    sleep(1);

    bool syntax_valid = false;
    bool move_valid = false;
    setLogStart(player_one, player_two);

    while (1)
    {
        send(player_one, "i-tm", 4, 0);
        send(player_two, "i-nm", 4, 0);

        // Wait until syntax and move are valid
        printf("Waiting for move from player one (%d)... sending i\n", player_one);

        while (!syntax_valid || !move_valid)
        {
            bzero(buffer, 64);

            printf("Checking syntax and move validation (%d,%d)\n", syntax_valid, move_valid);
            if (read(player_one, buffer, 6) < 0)
            {
                perror("ERROR reading from socket");
                exit(1);
            }
            printf("Player one (%d) move: %s\n", player_one, buffer);

            if (buffer[0] == 'g' && buffer[1] == 'i')
            {
                send(player_two, "gyes", 4, 0);
                while (1)
                {
                    bzero(buffer, 6);
                    if (read(player_two, buffer, 4) < 0)
                    {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (buffer[0] == 'y' && buffer[1] == 'e')
                    {
                        send(player_one, "g-ok", 4, 0);
                        send(player_two, "w", 1, 0);
                        free(move);
                        free_board(board);
                        setLogEndGame(player_one, player_two, 2);
                        close(player_one);
                        close(player_two);
                        return 0;
                    }
                    else if (buffer[0] == 'n' && buffer[1] == 'o')
                    {
                        send(player_one, "g-no", 4, 0);
                        break;
                    }
                }
            }
            else if (buffer[0] == 0 && buffer[1] == 0)
            {
                send(player_two, "b", 1, 0);
                free(move);
                free_board(board);
                setLogEndGame(player_one, player_two, 2);
                close(player_one);
                close(player_two);
                return 0;
            }
            else if (buffer[0] == '\n')
            {
                continue;
            }
            else
            {
                syntax_valid = is_syntax_valid(player_one, buffer);
                translate_to_move(move, buffer); // Convert to move
                // TODO
                move_valid = is_move_valid(board, player_one, 1, move);
                setLogOnGame(player_one, player_two, buffer, 1);
            }
        }

        printf("Player one (%d) made move\n", player_one);

        syntax_valid = false;
        move_valid = false;
        // Apply move to board
        move_piece(board, move);
        sleep(0.5);

        // Send applied move board
        broadcast(board, one_dimension_board, player_one, player_two);

        if (check_end_game(board))
        {
            send(player_one, "w", 1, 0);
            send(player_two, "l", 1, 0);
            setLogEndGame(player_one, player_two, 1);
            close(player_one);
            close(player_two);
            break;
        }
        sleep(1);
        send(player_one, "i-nm", 4, 0);
        send(player_two, "i-tm", 4, 0);

        printf("Waiting for move from player two (%d)\n", player_two);

        while (!syntax_valid || !move_valid)
        {
            bzero(buffer, 64);
            if (read(player_two, buffer, 6) < 0)
            {
                perror("ERROR reading from socket");
                exit(1);
            }

            printf("Player two (%d) move: %s\n", player_two, buffer);

            if (buffer[0] == '\n')
            {
                continue;
            }
            else if (buffer[0] == 'g' && buffer[1] == 'i')
            {
                send(player_one, "gyes", 4, 0);
                while (1)
                {
                    bzero(buffer, 6);
                    if (read(player_one, buffer, 4) < 0)
                    {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (buffer[0] == 'y' && buffer[1] == 'e')
                    {
                        send(player_two, "g-ok", 4, 0);
                        send(player_one, "w", 1, 0);
                        free(move);
                        free_board(board);
                        setLogEndGame(player_one, player_two, 2);
                        close(player_one);
                        close(player_two);
                        return 0;
                    }
                    else if (buffer[0] == 'n' && buffer[1] == 'o')
                    {
                        send(player_two, "g-no", 4, 0);
                        break;
                    }
                }
            }
            else if (buffer[0] == 0 && buffer[1] == 0)
            {
                send(player_one, "b", 1, 0);
                free(move);
                free_board(board);
                setLogEndGame(player_one, player_two, 2);
                close(player_one);
                close(player_two);
                return 0;
            }
            else
            {
                syntax_valid = is_syntax_valid(player_two, buffer);
                translate_to_move(move, buffer); // Convert to move
                move_valid = is_move_valid(board, player_two, -1, move);
                setLogOnGame(player_one, player_two, buffer, 2);
            }
        }
        printf("Player two (%d) made move\n", player_two);

        syntax_valid = false;
        move_valid = false;

        // Apply move to board
        move_piece(board, move);
        sleep(0.5);

        // Send applied move board
        broadcast(board, one_dimension_board, player_one, player_two);

        if (check_end_game(board))
        {
            send(player_two, "w", 1, 0);
            send(player_one, "l", 1, 0);
            setLogEndGame(player_one, player_two, 2);
            close(player_one);
            close(player_two);
            break;
        }
        sleep(1);
    }

    /* delete board */
    free(move);
    free_board(board);
    close(player_one);
    close(player_two);
}

int lobby(int player, char *username)
{
    while (1)
    {
        char lobbychoose[9];
        bzero(lobbychoose, 9);
        if (read(player, lobbychoose, 8) < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
        lobbychoose[8] = '\0';
        if (strcmp(lobbychoose, "cre-room") == 0)
        {
            send(player, "cre-true", 8, 0);
            printf("Create Room\n");
            return 1;
        }
        else if (strcmp(lobbychoose, "changepa") == 0)
        {
            char dataPassword[48];
            bzero(dataPassword, 48);
            if (read(player, dataPassword, 48) < 0)
            {
                perror("ERROR reading from socket");
                exit(1);
            }
            printf("Change Password: %s\n", dataPassword);
            char *oldPassword = strtok(dataPassword, " ");
            char *newPassword = strtok(NULL, " ");

            if (changePassword(username, oldPassword, newPassword))
            {
                send(player, "t", 1, 0);
            }
            else
            {
                send(player, "f", 1, 0);
            }
        }
        else if (strcmp(lobbychoose, "log--out") == 0)
        {
            send(player, "log-true", 8, 0);
            printf("Logout\n");
            return 2;
        }
        else if (strcmp(lobbychoose, "waitting") == 0)
        {
            while (1)
            {
                bzero(lobbychoose, 6);
                if (read(player, lobbychoose, 6) < 0)
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                lobbychoose[6] = '\0';
                printf("Buffer waitInvite: %s\n", lobbychoose);
                if (strcmp(lobbychoose, "accept") == 0)
                {
                    // Chap nhan loi moi (accept)
                    char opponent[24];
                    bzero(opponent, 24);
                    if (read(player, opponent, 24) < 0)
                    {
                        perror("ERROR reading from socket");
                        exit(1);
                    }

                    char *token;
                    token = strtok(opponent, "\n");
                    for (int i = 0; i < numbers; i++)
                    {
                        if (strcmp(users[i].name, token) == 0 && users[i].ongame == false)
                        {
                            send(users[i].client_socket, "accept", 6, 0);
                            break;
                        }
                    }
                    return 3;
                }
                else if (strcmp(lobbychoose, "refuse") == 0)
                {
                    // Tu choi loi moi (refuse)
                    char opponent[24];
                    bzero(opponent, 24);
                    if (read(player, opponent, 24) < 0)
                    {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    char *token;
                    token = strtok(opponent, "\n");
                    for (int i = 0; i < numbers; i++)
                    {
                        if (strcmp(users[i].name, opponent) == 0 && users[i].ongame == false)
                        {
                            send(users[i].client_socket, "refuse", 6, 0);
                        }
                    }
                }
            }
        }
    }
}

int getAllUser(int player)
{
    while (1)
    {
        char datachoose[9];
        bzero(datachoose, 8);
        if (read(player, datachoose, 8) < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
        datachoose[8] = '\0';
        printf("Room Data Choose: %s\n", datachoose);
        if (strcmp(datachoose, "get-user") == 0)
        {
            // Get user
            char *users_string = malloc(sizeof(char) * 1024);
            strcpy(users_string, "");
            // Add users to string
            for (int i = 0; i < numbers; i++)
            {
                if (!users[i].ongame)
                {
                    char user_info[128]; // Assume the maximum length of stt and name is 128, adjust if needed
                    sprintf(user_info, "%d. %s\n", i + 1, users[i].name);
                    strcat(users_string, user_info);
                }
            }
            send(player, users_string, 1024, 0);
        }
        else if (strcmp(datachoose, "invite--") == 0)
        {
            while (1)
            {
                char player2[24];
                bzero(player2, 24);
                if (read(player, player2, 24) < 0)
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                printf("Invite: %s\n", player2);
                char username[24];
                for (int i = 0; i < numbers; i++)
                {
                    if (users[i].client_socket == player && users[i].ongame == false)
                    {
                        strcpy(username, users[i].name);
                    }
                }
                for (int i = 0; i < numbers; i++)
                {
                    if (strcmp(users[i].name, player2) == 0 && users[i].ongame == false)
                    {
                        send(users[i].client_socket, "invite", 6, 0);
                        send(users[i].client_socket, username, strlen(username), 0);
                    }
                }
                while (1)
                {
                    bzero(datachoose, 6);
                    if (read(player, datachoose, 6) < 0)
                    {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    datachoose[6] = '\0';
                    if (strcmp(datachoose, "accept") == 0)
                    {
                        // Chap nhan loi moi (accept)
                        for (int i = 0; i < numbers + 1; i++)
                        {
                            if (users[i].client_socket == player)
                            {
                                users[i].ongame = true;
                                break;
                            }
                        }
                        for (int i = 0; i < numbers + 1; i++)
                        {
                            if (strcmp(users[i].name, player2) == 0 && users[i].ongame == false)
                            {
                                users[i].ongame = true;
                                int player2_socket = users[i].client_socket;
                                // Create game....
                                printf("Connected player, creating new game room...\n");
                                pthread_t tid[1];
                                rooms[roomNumbers].client1 = player;
                                rooms[roomNumbers].client2 = player2_socket;

                                pthread_create(&tid[0], NULL, &game_room, 0);
                                return 1;
                            }
                        }
                    }
                    else if (strcmp(datachoose, "refuse") == 0)
                    {
                        // Tu choi loi moi (refuse)
                        break;
                    }
                }
                break;
            }
        }
    }
}

// bỏ do khi login thì người dùng có thể trùng tên với nhau mà password khác nhau
// loại bỏ sử dụng trong phần check login ở server_main
int checkLogged(char *username)
{
    for (int i = 0; i < numbers; i++)
    {
        if (strcmp(users[i].name, username) == 0)
        {
            return 0;
        }
    }
    return 1;
}
