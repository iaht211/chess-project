#include <ncurses.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#define BOARD_SIZE 8
#define TILE_WIDTH 4
#define TILE_HEIGHT 2

// #define white_king   0x2654 // ♔
// #define white_queen  0x2655 // ♕
// #define white_rook   0x2656 // ♖
// #define white_bishop 0x2657 // ♗
// #define white_knight 0x2658 // ♘
// #define white_pawn   0x2659 // ♙
// #define black_king   0x265A // ♚
// #define black_queen  0x265B // ♛
// #define black_rook   0x265C // ♜
// #define black_bishop 0x265D // ♝
// #define black_knight 0x265E // ♞
// #define black_pawn   0x265F // ♟

#define white_king  'K'
#define white_queen 'Q'
#define white_rook   'R'
#define white_bishop 'B'
#define white_knight 'N'
#define white_pawn   'P'
#define black_king   'k'
#define black_queen  'q'
#define black_rook   'r'
#define black_bishop 'b'
#define black_knight 'n'
#define black_pawn   'p'

// Function to translate the chess piece character to a wide character symbol
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

void draw_board(char *board) {
    setlocale(LC_ALL, "en_US.UTF-8"); // Set locale to support Unicode explicitly

    initscr();            // Start ncurses mode
    start_color();        // Allow colors
    cbreak();             // Disable line buffering
    noecho();             // Don't echo input characters

    // Define color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);   // Red for white pieces
    init_pair(2, COLOR_BLUE, COLOR_BLACK);  // Blue for black pieces
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // Light tile
    init_pair(4, COLOR_BLACK, COLOR_WHITE); // Dark tile

    // Draw the chessboard tiles
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // Set background color for the tile (alternating pattern)
            if ((row + col) % 2 == 0) {
                attron(COLOR_PAIR(3)); // Light tile
            } else {
                attron(COLOR_PAIR(4)); // Dark tile
            }

            // Draw the tile background
            for (int y = 0; y < TILE_HEIGHT; y++) {
                for (int x = 0; x < TILE_WIDTH; x++) {
                    mvprintw(row * TILE_HEIGHT + y, col * TILE_WIDTH + x, " ");
                }
            }

            // Turn off tile color attributes
            if ((row + col) % 2 == 0) {
                attroff(COLOR_PAIR(3));
            } else {
                attroff(COLOR_PAIR(4));
            }
        }
    }

    // Draw the chess pieces
    /*for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board[row * BOARD_SIZE + col];
            wchar_t translated_piece = translate_piece(piece);

            // If there's no piece, continue
            if (translated_piece == L' ') {
                continue;
            }

            // Set color for the piece
            if (piece >= '1' && piece <= '6') {
                attron(COLOR_PAIR(1)); // Red for white pieces
            } else if (piece >= '7' && piece <= 'c') {
                attron(COLOR_PAIR(2)); // Blue for black pieces
            }

            // Print the wide character piece in the middle of the tile
            mvaddwstr(row * TILE_HEIGHT + TILE_HEIGHT / 2,
                      col * TILE_WIDTH + TILE_WIDTH / 2 - 1,
                      &translated_piece);

            // Turn off piece color attributes
            if (piece >= '1' && piece <= '6') {
                attroff(COLOR_PAIR(1));
            } else if (piece >= '7' && piece <= 'c') {
                attroff(COLOR_PAIR(2));
            }
        }
    }
    */
    refresh(); // Refresh the screen to display changes
    getch();   // Wait for user input before exiting
    endwin();  // End ncurses mode
}

int main() {
    char board[BOARD_SIZE * BOARD_SIZE] = {
        '1', '2', '3', '4', '5', '3', '2', '1',
        '6', '6', '6', '6', '6', '6', '6', '6',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c',
        '7', '8', '9', 'a', 'b', '9', '8', '7'};

    draw_board(board);
    return 0;
}
