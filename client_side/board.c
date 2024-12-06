#include <ncurses.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"

#define white_king   0x2654 // ♔
#define white_queen  0x2655 // ♕
#define white_rook   0x2656 // ♖
#define white_bishop 0x2657 // ♗
#define white_knight 0x2658 // ♘
#define white_pawn   0x2659 // ♙
#define black_king   0x265A // ♚
#define black_queen  0x265B // ♛
#define black_rook   0x265C // ♜
#define black_bishop 0x265D // ♝
#define black_knight 0x265E // ♞
#define black_pawn   0x265F // ♟
#define BOARD_SIZE 8

static void debug_print_board(wchar_t **);

static wchar_t ** create_board() {
  // create dinamically malloc

  wchar_t ** board = (wchar_t **) malloc(sizeof(wchar_t *)*8);
  for (int i = 0; i < 8; i++) {
    board[i] = (wchar_t *) malloc(sizeof(wchar_t)*8);
  }

  return board;
}

static void initialize_board(wchar_t ** p_board) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      if (i == 0) {
        if (j == 0 || j == 7) p_board[i][j] = white_rook;
        if (j == 1 || j == 6) p_board[i][j] = white_knight;
        if (j == 2 || j == 5) p_board[i][j] = white_bishop;
        if (j == 3) p_board[i][j] = white_queen;
        if (j == 4) p_board[i][j] = white_king;
      }

      if (i == 1) p_board[i][j] = white_pawn;

      if (i > 1 && i < 6) p_board[i][j] = 0;

      if (i == 6) p_board[i][j] = black_pawn;
      if (i == 7) {
        if(j == 0 || j == 7) p_board[i][j] = black_rook;
        if(j == 1 || j == 6) p_board[i][j] = black_knight;
        if(j == 2 || j == 5) p_board[i][j] = black_bishop;
        if(j == 3) p_board[i][j] = black_queen;
        if(j == 4) p_board[i][j] = black_king;
      }
    }
  }
}

static char * create_od_board() {
  char * board = (char *) malloc(sizeof(char *)*8*8);
  return board;
}

static void to_one_dimension_char(wchar_t ** board, char * od_board) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == white_rook) od_board[8*i+j] = '1';
      else if (board[i][j] == white_knight) od_board[8*i+j] = '2';
      else if (board[i][j] == white_bishop) od_board[8*i+j] = '3';
      else if (board[i][j] == white_queen) od_board[8*i+j] = '4';
      else if (board[i][j] == white_king) od_board[8*i+j] = '5';
      else if (board[i][j] == white_pawn) od_board[8*i+j] = '6';
      else if (board[i][j] == black_rook) od_board[8*i+j] = '7';
      else if (board[i][j] == black_knight) od_board[8*i+j] = '8';
      else if (board[i][j] == black_bishop) od_board[8*i+j] = '9';
      else if (board[i][j] == black_queen) od_board[8*i+j] = 'a';
      else if (board[i][j] == black_king) od_board[8*i+j] = 'b';
      else if (board[i][j] == black_pawn) od_board[8*i+j] = 'c';
      else od_board[8*i+j] = '0';
    }
  }

}

static void free_od_board(wchar_t * od_board) {
  free(od_board);
}

static void free_board(wchar_t ** board) {
  for (int i = 0; i < 8; i++) {
    free(board[i]);
  }
  free(board);
}

static void debug_print_board(wchar_t ** board) {
  setlocale( LC_ALL, "en_US.UTF-8" );
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j] != 0) { printf("%lc", board[i][j]); }
      else { printf(" "); }
      printf(" ");
    }
    printf("\n");
  }
}

static wchar_t translate_piece (char char_piece) {
  if (char_piece == '1') return white_rook;
  else if (char_piece == '2') return white_knight;
  else if (char_piece == '3') return white_bishop;
  else if (char_piece == '4') return white_queen;
  else if (char_piece == '5') return white_king;
  else if (char_piece == '6') return white_pawn;
  else if (char_piece == '7') return black_rook;
  else if (char_piece == '8') return black_knight;
  else if (char_piece == '9') return black_bishop;
  else if (char_piece == 'a') return black_queen;
  else if (char_piece == 'b') return black_king;
  else if (char_piece == 'c') return black_pawn;
  else return ' ';
}

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

void print_board_buff(char *board){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            printf("%c ", board[i*8 + j]);
        }
        printf("\n");
    }
    // printf("Print normal board\n");
}

// void print_board_buff(char *board){
//     initscr();
//     noecho();
//     curs_set(FALSE);
//     start_color();
//     init_pair(1, COLOR_WHITE, COLOR_BLACK); // Black squares
//     init_pair(2, COLOR_BLACK, COLOR_WHITE); // White squares
//     init_pair(5, COLOR_BLUE, COLOR_BLACK);  // White pieces on black squares
//     init_pair(6, COLOR_RED, COLOR_BLACK);   // Black pieces on black squares
//     init_pair(7, COLOR_BLUE, COLOR_WHITE);  // White pieces on white squares
//     init_pair(8, COLOR_RED, COLOR_WHITE);   // Black pieces on white squares

//     int start_x = 4, start_y = 2;
//     int tile_width = 3, tile_height = 1;

//     for (int j = 0; j < BOARD_SIZE; j++) {
//         mvprintw(start_y - 1, start_x + j * tile_width + tile_width / 2, "%c", 'A' + j);
//     }

//     for (int i = 0; i < BOARD_SIZE; i++) {
//         mvprintw(start_y + i * tile_height + tile_height / 2, start_x - 2, "%d", BOARD_SIZE - i);
//     }

//     for (int i = 0; i < BOARD_SIZE; i++){
//         for (int j = 0; j < BOARD_SIZE; j++){
//             int x = start_x + j * tile_width;
//             int y = start_y + i * tile_height;

//             if ((i + j) % 2 == 0){
//                 attron(COLOR_PAIR(1));
//             } else {
//                 attron(COLOR_PAIR(2));
//             }

//             for(int dy = 0; dy < tile_height; dy++){
//                 for(int dx = 0; dx < tile_width; dx++){
//                     mvprintw(y + dy, x + dx, " ");
//                 }
//             }

//             if ((i + j) % 2 == 0){
//                 attroff(COLOR_PAIR(1));
//             } else {
//                 attroff(COLOR_PAIR(2));
//             }
//         }
//     }

//     // Print the pieces
//     for(int i = 0; i < BOARD_SIZE; i++){
//         for(int j = 0; j < BOARD_SIZE; j++){
//             char piece = board[i * BOARD_SIZE + j];
//             if (piece == ' '){
//                 continue;
//             }
//             int x = start_x + j * tile_width + tile_width / 2;
//             int y = start_y + i * tile_height + tile_height / 2;

//             int WHITE_PIECE = 1;
//             if (piece >= '1' && piece <= '6')
//                 WHITE_PIECE = 0;

//             if ((i+j) % 2 == 0){
//                 if(WHITE_PIECE){
//                     attron(COLOR_PAIR(5));
//                 } else {
//                     attron(COLOR_PAIR(6));
//                 }
//             } else {
//                 if(WHITE_PIECE){
//                     attron(COLOR_PAIR(7));
//                 } else {
//                     attron(COLOR_PAIR(8));
//                 }
//             }

//             mvprintw(y, x, "%c", get_char_piece(piece));
//             if ((i+j) % 2 == 0){
//                 if(WHITE_PIECE){
//                     attroff(COLOR_PAIR(5));
//                 } else {
//                     attroff(COLOR_PAIR(6));
//                 }
//             } else {
//                 if(WHITE_PIECE){
//                     attroff(COLOR_PAIR(7));
//                 } else {
//                     attroff(COLOR_PAIR(8));
//                 }
//             }
//         }
//     }

//     // Refresh the screen and wait for user input to exit
//     refresh();
//     getch();

//     // End ncurses mode
//     endwin();
// }



// static void print_board_buff_inverted(char *board){
//     for(int i = 7; i >= 0; i--){
//         for(int j = 0; j < 8; j++){
//             printf("%c ", board[i*8 + j]);
//         }
//         printf("\n");
//     }
//     // printf("Print inverted board\n");  
// }


void print_board_buff_inverted(char *board) {
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
