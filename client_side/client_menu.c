#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h"

#include "client_menu.h"

#define BUFFER_SIZE 256

int loggedIn2 = 0;
char opponent[24];

volatile int invite_processing = 0;
int login_successful = 0;
void main_loop(int sockfd)
{
    char buffer[64];
    int n;

    while (1)
    {
        if (loggedIn2 == 0)
        {
            display_login_menu(&sockfd);
        }
        else if (loggedIn2 == 1)
        {
            display_main_menu(&sockfd);
        }
        else if (loggedIn2 == 2)
        {
            display_room_menu(&sockfd);
        }
        else if (loggedIn2 == 3)
        {
            pthread_t tid[1];

            pthread_create(&tid[0], NULL, &on_signal, &sockfd);

            while (1)
            {
                bzero(buffer, 64);
                fgets(buffer, 64, stdin);

                /* Send message to the server */
                n = write(sockfd, buffer, strlen(buffer));

                if (n < 0)
                {
                    perror("ERROR writing to socket");
                    exit(1);
                }
            }
        }
    }
}

void display_login_menu(void *sockfd)
{
    char *choices[] = {
        "Login",
        "Register",
        "Exit"};
    int num_choices = sizeof(choices) / sizeof(char *);
    int socket = *(int *)sockfd;
    handle_menu_choice(socket, choices, num_choices, handle_login_menu_selection);
    return;
}

void handle_login_menu_selection(int choice, int sock)
{
    switch (choice)
    {
    case 0:
        handle_login(sock);
        break;
    case 1:
        handle_register(sock);
        break;
    case 2:
        handle_exit(sock);
        exit(0);
        break;
    default:
        break;
    }
}

void handle_login(int socket)
{
    char username[50];
    char password[50];
    char buffer[BUFFER_SIZE];

    int n = write(socket, "login", 5);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Clear screen and prompt user for login information
    clear();
    echo();
    mvprintw(1, 2, "Enter Username: ");
    getstr(username);
    mvprintw(2, 2, "Enter Password: ");
    getstr(password);
    noecho();

    strcat(username, " ");
    strcat(username, password);
    n = write(socket, username, strlen(username));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    while (1)
    {
        bzero(buffer, 4);
        n = read(socket, buffer, 4);
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }

        if (buffer[0] == 't')
        {
            mvprintw(4, 2, "Login successful. Press any key to continue to the main menu...");
            getch();
            clear();
            loggedIn2 = 1;
            endwin();
            main_loop(socket);
        }
        else if (buffer[0] == 'f')
        {
            mvprintw(4, 2, "Login failed: %s", buffer);
            mvprintw(5, 2, "Press any key to try again...");
            getch();
            clear();
            main_loop(socket);
        }
    }
}

void handle_register(int socket)
{
    char username[50];
    char password[50];
    char buffer[BUFFER_SIZE];
    int n;

    // Gửi yêu cầu "register" đến server
    n = write(socket, "register", 8);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Hiển thị giao diện nhập thông tin
    clear();
    echo();
    mvprintw(1, 2, "Enter Username: ");
    getstr(username);
    mvprintw(2, 2, "Enter Password: ");
    getstr(password);
    noecho();

    // Gửi thông tin đăng ký tới server
    strcat(username, " ");
    strcat(username, password);
    n = write(socket, username, strlen(username));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Chờ phản hồi từ server
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        n = read(socket, buffer, sizeof(buffer));
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }

        if (buffer[0] == 't') // Đăng ký thành công
        {
            mvprintw(4, 2, "Registration successful! Press any key to return to the login menu...");
            getch();
            clear();
            display_login_menu(&socket); // Quay lại menu đăng nhập
            return;                      // Kết thúc hàm
        }
        else if (buffer[0] == 'f') // Đăng ký thất bại
        {
            mvprintw(4, 2, "Registration failed: %s", buffer);
            mvprintw(5, 2, "Press any key to try again...");
            getch();
            clear();
            display_login_menu(&socket); // Quay lại menu đăng ký
            return;                      // Kết thúc hàm
        }
    }
}

void handle_exit(int socket)
{
    int n;
    char c;

    // Clear screen and show confirmation prompt
    clear();
    mvprintw(2, 2, "Are you sure you want to exit? (y/n): ");
    refresh();

    // Get confirmation
    c = getch();
    if (c != 'y' && c != 'Y')
    {
        clear();
        return;
    }

    // Show exit message
    clear();
    mvprintw(2, 2, "Exiting...");
    refresh();

    // Send exit message to server
    n = write(socket, "exit", 4);
    if (n < 0)
    {
        mvprintw(4, 2, "ERROR: Could not send exit message");
        mvprintw(5, 2, "Press any key to force exit...");
        refresh();
        getch();
        endwin(); // Clean up ncurses
        exit(1);
    }

    // Clean exit
    mvprintw(4, 2, "Goodbye!");
    refresh();
    napms(1500); // Brief delay to show message
    endwin();    // Clean up ncurses
    exit(0);
}

void display_main_menu(void *sock)
{
    char *choices[] = {
        "Create Room",
        "Waiting",
        "Change Password",
        "Logout"};
    int num_choices = sizeof(choices) / sizeof(char *);
    int socket = *(int *)sock;

    handle_menu_choice(socket, choices, num_choices, handle_main_menu_selection);
}

void handle_main_menu_selection(int choice, int sock)
{
    switch (choice)
    {
    case 0:
        handle_create_room(sock);
        break;
    case 1:
        handle_waiting(sock);
        break;
    case 2:
        handle_change_password(sock);
        break;
    case 3:
        handle_logout(sock);
        break;
    case 4:
        // Implement help function
        break;
    case 5:
        endwin();
        exit(0);
        break;
    default:
        break;
    }
}

void handle_create_room(int socket)
{
    char buffer[8];
    int n;

    // Clear screen before displaying new content
    clear();

    // Send create room request
    n = write(socket, "cre-room", 8);
    if (n < 0)
    {
        mvprintw(1, 2, "ERROR writing to socket");
        getch();
        exit(1);
    }

    while (1)
    {
        bzero(buffer, 8);
        n = read(socket, buffer, 8);
        if (n < 0)
        {
            mvprintw(1, 2, "ERROR reading from socket");
            getch();
            exit(1);
        }

        if (strcmp(buffer, "cre-true") == 0)
        {
            clear();
            mvprintw(2, 2, "Room created successfully!");
            mvprintw(4, 2, "Press any key to continue...");
            refresh();
            getch();
            clear();
            display_room_menu(&socket);
            loggedIn2 = 2;
            main_loop(socket);
        }
    }
}

void handle_waiting(int socket)
{
    char invite[6];
    char username[24];
    int n, choice;

    // Clear screen and show waiting message
    clear();
    mvprintw(2, 2, "Waiting for invites...");
    mvprintw(4, 2, "Press 'q' to return to main menu");
    refresh();

    // Send waiting status to server
    n = write(socket, "waitting", 8);
    if (n < 0)
    {
        mvprintw(6, 2, "ERROR: Could not send waiting status");
        mvprintw(7, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    while (1)
    {
        // Check for quit command
        timeout(100);
        int c = getch();
        if (c == 'q' || c == 'Q')
        {
            clear();
            return;
        }

        // Check for invites
        bzero(invite, 6);
        n = read(socket, invite, 6);
        if (n < 0)
        {
            mvprintw(6, 2, "ERROR: Connection lost");
            mvprintw(7, 2, "Press any key to return...");
            refresh();
            getch();
            return;
        }

        if (invite[0] == 'i' && invite[1] == 'n')
        {
            // Read username of inviter
            bzero(username, 24);
            n = read(socket, username, 24);
            if (n < 0)
            {
                mvprintw(6, 2, "ERROR: Could not read username");
                mvprintw(7, 2, "Press any key to return...");
                refresh();
                getch();
                return;
            }
            strcpy(opponent, username);
            clear();
            display_invite(&socket);
        }
    }
}

void display_invite(void *sock)
{
    char *choices[] = {
        "1. Accept",
        "2. Refuse"};
    int num_choices = sizeof(choices) / sizeof(char *);
    int socket = *(int *)sock;
    handle_menu_choice(socket, choices, num_choices, handle_invite_selection);
}

void handle_invite_selection(int choice, int socket)
{
    switch (choice)
    {
    case 0:
        handle_accept(socket);
        break;
    case 1:
        handle_refuse(socket);
        break;
    default:
        printf("Invalid choice. Please select again.\n");
        break;
    }
}

void handle_accept(int socket)
{
    // Handle accept
    if (write(socket, "accept", 6) < 0)
    {
        mvprintw(9, 2, "ERROR: Could not send response");
        mvprintw(10, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    if (write(socket, opponent, 24) < 0)
    {
        mvprintw(9, 2, "ERROR: Could not send response");
        mvprintw(10, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    clear();
    refresh();
    mvprintw(2, 2, "Accepted invite");
    loggedIn2 = 3;

//    endwin(); // Kết thúc ncurses
    main_loop(socket);
}

void handle_refuse(int socket)
{
    // Handle decline
    if (write(socket, "refuse", 6) < 0)
    {
        mvprintw(9, 2, "ERROR: Could not send response");
        mvprintw(10, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    mvprintw(2, 2, "Declined invite");
    mvprintw(4, 2, "Returning to waiting...");
    clear();
    display_main_menu(&socket);
}

void handle_change_password(int socket)
{
    char oldPassword[50];
    char newPassword[50];
    char buffer[BUFFER_SIZE];
    int n;

    // Gửi yêu cầu "changepa" tới server
    n = write(socket, "changepa", 8);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Hiển thị giao diện nhập mật khẩu cũ và mới
    clear();
    echo();
    mvprintw(1, 2, "Enter Old Password: ");
    getstr(oldPassword);
    mvprintw(2, 2, "Enter New Password: ");
    getstr(newPassword);
    noecho();

    // Chuẩn bị chuỗi gửi tới server
    strcat(oldPassword, " ");
    strcat(oldPassword, newPassword);

    n = write(socket, oldPassword, strlen(oldPassword));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Chờ phản hồi từ server
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        n = read(socket, buffer, sizeof(buffer));
        if (n < 0)
        {
            mvprintw(10, 2, "ERROR: Could not read server response");
            mvprintw(11, 2, "Press any key to return...");
            refresh();
            getch();
            exit(1);
        }

        if (buffer[0] == 't') // Đổi mật khẩu thành công
        {
            mvprintw(4, 2, "Password changed successfully! Press any key to return to the main menu...");
            getch();
            clear();
            display_main_menu(&socket); // Quay lại menu chính
            return;
        }
        else if (buffer[0] == 'f') // Đổi mật khẩu thất bại
        {
            mvprintw(4, 2, "Password change failed: %s", buffer);
            mvprintw(5, 2, "Press any key to try again...");
            getch();
            clear();
            display_main_menu(&socket); // Quay lại menu đổi mật khẩu
            return;
        }
    }
}

void handle_logout(int socket)
{
    char buffer[8];
    int n;

    // Clear screen and show logout message
    clear();
    mvprintw(2, 2, "Logging out...");
    refresh();

    // Send logout request
    n = write(socket, "log--out", 8);
    if (n < 0)
    {
        mvprintw(4, 2, "ERROR: Could not send logout request");
        mvprintw(5, 2, "Press any key to exit...");
        refresh();
        getch();
        endwin();
        exit(1);
    }

    // Read server response
    bzero(buffer, 8);
    n = read(socket, buffer, 7);
    buffer[7] = '\0';

    clear();
    mvprintw(2, 2, "Logout game!");
    mvprintw(4, 2, "Press any key to continue...");
    refresh();
    getch();
    clear();
    display_login_menu(&socket);
    loggedIn2 = 2;
    return;
}

void display_room_menu(void *sock)
{
    char *choices[] = {
        "Get User Online",
        "Invite Players",
        "Remove Room"};
    int num_choices = sizeof(choices) / sizeof(char *);
    int socket = *(int *)sock;
    handle_menu_choice(socket, choices, num_choices, handle_room_menu_selection);
}

void handle_room_menu_selection(int choice, int socket)
{
    switch (choice)
    {
    case 0:
        handle_get_user_online(socket);
        break;
    case 1:
        handle_invite_players(socket);
        break;
    case 2:
        handle_remove_room(socket);
        break;
    default:
        printf("Invalid choice. Please select again.\n");
        break;
    }
}

void handle_get_user_online(int socket)
{
    char buffer[1024];
    char *user;
    int n;

    // Clear screen and show loading message
    clear();
    mvprintw(2, 2, "Getting online users...");
    refresh();

    // Send request to get online users
    n = write(socket, "get-user", 8);
    if (n < 0)
    {
        mvprintw(4, 2, "ERROR: Could not send request");
        mvprintw(5, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    // Clear screen for user list
    clear();
    mvprintw(2, 2, "Online Users:");
    refresh();

    // Read and display user list
    bzero(buffer, 1024);
    n = read(socket, buffer, 1024);
    if (n < 0)
    {
        mvprintw(4, 2, "ERROR: Connection lost");
        mvprintw(5, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    // Parse and display user list with proper formatting
    int row = 4;
    int count = 1;
    user = strtok(buffer, "\n");
    while (user != NULL)
    {
        mvprintw(row, 2, "%s", user);
        row += 2;
        count++;
        user = strtok(NULL, "\n");
    }

    // Wait for user input before returning
    mvprintw(row + 2, 2, "Press any key to return to menu...");
    refresh();
    getch();
    clear();
}

void handle_invite_players(int socket)
{
    char username[50];
    char dataread[7];
    char buffer[BUFFER_SIZE];
    int n;

    // Gửi yêu cầu "invite--" tới server
    n = write(socket, "invite--", 8);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Hiển thị giao diện nhập tên đối thủ
    clear();
    echo();
    mvprintw(1, 2, "Enter opponent's username: ");
    getstr(username);
    noecho();

    // Gửi tên đối thủ tới server
    n = write(socket, username, strlen(username));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Chờ phản hồi từ server
    while (1)
    {
        memset(dataread, 0, sizeof(dataread));
        n = read(socket, dataread, sizeof(dataread) - 1);
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
        dataread[n] = '\0';

        if (strcmp(dataread, "accept") == 0) // Đối thủ chấp nhận
        {
            mvprintw(3, 2, "The opponent accepted the invite!");
            mvprintw(4, 2, "Press any key to proceed...");
            if (write(socket, "accept", 6) < 0)
            {
                mvprintw(9, 2, "ERROR: Could not send response");
                mvprintw(10, 2, "Press any key to return...");
                refresh();
                getch();
                return;
            }
            getch();
            loggedIn2 = 3; // Cập nhật trạng thái đăng nhập
            endwin();      // Kết thúc ncurses
            main_loop(socket);
        }
        else if (strcmp(dataread, "refuse") == 0) // Đối thủ từ chối
        {
            mvprintw(3, 2, "The opponent refused the invite.");
            mvprintw(4, 2, "Press any key to try again...");
            getch();
            clear();
            return;
        }
    }
}

void handle_remove_room(int socket)
{
    int n = write(socket, "remove--", 8);
    if (n < 0)
    {
        mvprintw(4, 2, "ERROR: Could not remove room");
        mvprintw(5, 2, "Press any key to return...");
        refresh();
        getch();
        return;
    }

    // Show success message
    clear();
    mvprintw(2, 2, "Room removed successfully!");
    refresh();
    display_main_menu(&socket);
    loggedIn2 = 1; // Return to main menu state
    clear();
}

void handle_menu_choice(int sock, char *choices[], int num_choices, void (*handle_selection)(int, int))
{
    int highlight = 0;
    int choice = 0;
    int c;

    initscr(); // Start ncurses mode
    clear();
    noecho();
    cbreak();
    curs_set(0);          // Hide the cursor
    keypad(stdscr, TRUE); // Enable keypad input for arrow keys

    // Create the menu loop
    while (1)
    {
        if (invite_processing)
        {
            continue;
        }
        for (int i = 0; i < num_choices; ++i)
        {
            if (i == highlight)
            {
                attron(A_REVERSE); // Highlight the current choice
                mvprintw(i + 1, 2, choices[i]);
                attroff(A_REVERSE);
            }
            else
            {
                mvprintw(i + 1, 2, choices[i]);
            }
        }
        c = getch();

        switch (c)
        {
        case KEY_UP:
        case 'w':
        case 'W':
            highlight = (highlight - 1 + num_choices) % num_choices;
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            highlight = (highlight + 1) % num_choices;
            break;
        case 10: // Enter key
            choice = highlight;
            handle_selection(choice, sock);
            break;
        default:
            break;
        }
    }
}
