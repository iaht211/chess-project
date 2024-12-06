#include <ncurses.h>
#include <stdio.h>

#define BOARD_SIZE 8  // Chessboard is 8x8
#define TILE_WIDTH 3  // Each tile is 3 characters wide

// Function to get the display character for a piece
char get_char_piece(char piece) {
    switch (piece) {
        case '1': return 'R'; // Rook
        case '2': return 'N'; // Knight
        case '3': return 'B'; // Bishop
        case '4': return 'Q'; // Queen
        case '5': return 'K'; // King
        case '6': return 'P'; // Pawn for one side
        case '7': return 'r'; // Rook for the other side
        case '8': return 'n'; // Knight for the other side
        case '9': return 'b'; // Bishop for the other side
        case 'a': return 'q'; // Queen for the other side
        case 'b': return 'k'; // King for the other side
        case 'c': return 'p'; // Pawn for the other side
        default: return ' ';  // Empty square
    }
}

void draw_chess_board(char *board){
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Black squares
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // White squares
    init_pair(5, COLOR_BLUE, COLOR_BLACK);  // White pieces on black squares
    init_pair(6, COLOR_RED, COLOR_BLACK);   // Black pieces on black squares
    init_pair(7, COLOR_BLUE, COLOR_WHITE);  // White pieces on white squares
    init_pair(8, COLOR_RED, COLOR_WHITE);   // Black pieces on white squares

    int start_x = 4, start_y = 2;
    int tile_width = 3, tile_height = 1;

    for (int j = 0; j < BOARD_SIZE; j++) {
        mvprintw(start_y - 1, start_x + j * tile_width + tile_width / 2, "%c", 'A' + j);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        mvprintw(start_y + i * tile_height + tile_height / 2, start_x - 2, "%d", BOARD_SIZE - i);
    }

    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            int x = start_x + j * tile_width;
            int y = start_y + i * tile_height;

            if ((i + j) % 2 == 0){
                attron(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
            }

            for(int dy = 0; dy < tile_height; dy++){
                for(int dx = 0; dx < tile_width; dx++){
                    mvprintw(y + dy, x + dx, " ");
                }
            }

            if ((i + j) % 2 == 0){
                attroff(COLOR_PAIR(1));
            } else {
                attroff(COLOR_PAIR(2));
            }
        }
    }

    // Print the pieces
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            char piece = board[i * BOARD_SIZE + j];
            if (piece == ' '){
                continue;
            }
            int x = start_x + j * tile_width + tile_width / 2;
            int y = start_y + i * tile_height + tile_height / 2;

            int WHITE_PIECE = 1;
            if (piece >= '1' && piece <= '6')
                WHITE_PIECE = 0;

            if ((i+j) % 2 == 0){
                if(WHITE_PIECE){
                    attron(COLOR_PAIR(5));
                } else {
                    attron(COLOR_PAIR(6));
                }
            } else {
                if(WHITE_PIECE){
                    attron(COLOR_PAIR(7));
                } else {
                    attron(COLOR_PAIR(8));
                }
            }

            mvprintw(y, x, "%c", get_char_piece(piece));
            if ((i+j) % 2 == 0){
                if(WHITE_PIECE){
                    attroff(COLOR_PAIR(5));
                } else {
                    attroff(COLOR_PAIR(6));
                }
            } else {
                if(WHITE_PIECE){
                    attroff(COLOR_PAIR(7));
                } else {
                    attroff(COLOR_PAIR(8));
                }
            }
        }
    }

    // Refresh the screen and wait for user input to exit
    refresh();
    getch();

    // End ncurses mode
    endwin();
}

void draw_inverted_chess_board(char *board) {
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Black squares
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // White squares
    init_pair(5, COLOR_BLUE, COLOR_BLACK);  // White pieces on black squares
    init_pair(6, COLOR_RED, COLOR_BLACK);   // Black pieces on black squares
    init_pair(7, COLOR_BLUE, COLOR_WHITE);  // White pieces on white squares
    init_pair(8, COLOR_RED, COLOR_WHITE);   // Black pieces on white squares

    int start_x = 4, start_y = 2;
    int tile_width = 3, tile_height = 1;

    for (int j = 0; j < BOARD_SIZE; j++) {
        mvprintw(start_y - 1, start_x + j * tile_width + tile_width / 2, "%c", 'A' + j);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        mvprintw(start_y + i * tile_height + tile_height / 2, start_x - 2, "%d", BOARD_SIZE - i);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int x = start_x + j * tile_width;
            int y = start_y + i * tile_height;

            if ((i + j) % 2 == 0) {
                attron(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
            }

            for (int dy = 0; dy < tile_height; dy++) {
                for (int dx = 0; dx < tile_width; dx++) {
                    mvprintw(y + dy, x + dx, " ");
                }
            }

            if ((i + j) % 2 == 0) {
                attroff(COLOR_PAIR(1));
            } else {
                attroff(COLOR_PAIR(2));
            }
        }
    }

    // Print the pieces inverted
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            char piece = board[(BOARD_SIZE - 1 - i) * BOARD_SIZE + (BOARD_SIZE - 1 - j)];
            if (piece == ' ') {
                continue;
            }
            int x = start_x + j * tile_width + tile_width / 2;
            int y = start_y + i * tile_height + tile_height / 2;

            int WHITE_PIECE = 1;
            if (piece >= '1' && piece <= '6')
                WHITE_PIECE = 0;

            if ((i + j) % 2 == 0) {
                if (WHITE_PIECE) {
                    attron(COLOR_PAIR(5));
                } else {
                    attron(COLOR_PAIR(6));
                }
            } else {
                if (WHITE_PIECE) {
                    attron(COLOR_PAIR(7));
                } else {
                    attron(COLOR_PAIR(8));
                }
            }

            mvprintw(y, x, "%c", get_char_piece(piece));
            if ((i + j) % 2 == 0) {
                if (WHITE_PIECE) {
                    attroff(COLOR_PAIR(5));
                } else {
                    attroff(COLOR_PAIR(6));
                }
            } else {
                if (WHITE_PIECE) {
                    attroff(COLOR_PAIR(7));
                } else {
                    attroff(COLOR_PAIR(8));
                }
            }
        }
    }

    // Refresh the screen and wait for user input to exit
    refresh();
    getch();

    // End ncurses mode
    endwin();
}

int main() {
    // Initialize ncurses and colors
    // Chessboard representation with new character set
    char board[BOARD_SIZE * BOARD_SIZE] = {
        '1', '2', '3', '4', '5', '3', '2', '1',
        '6', '6', ' ', '6', '6', '6', '6', '6',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '6', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', 'c', ' ', ' ', ' ', ' ', ' ',
        'c', 'c', ' ', 'c', 'c', 'c', 'c', 'c',
        '7', '8', '9', 'a', 'b', '9', '8', '7'
    };

    // Draw the chessboard without borders and no space between tiles
    // drawboard(board);
    // draw_chess_board(board);
    draw_inverted_chess_board(board);

    return 0;
}
