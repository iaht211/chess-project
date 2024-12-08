#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <ncurses.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

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

// Function declarations

/**
 * Debug print the chessboard.
 * @param board 2D array of the chess board.
 */
static void debug_print_board(wchar_t **board);

/**
 * Create a new empty chessboard with dynamic memory allocation.
 * @return Pointer to the 2D array representing the chess board.
 */
static wchar_t **create_board();

/**
 * Initialize the chess board with pieces in starting positions.
 * @param p_board Pointer to the board to be initialized.
 */
static void initialize_board(wchar_t **p_board);

/**
 * Create a one-dimensional array representation of the chessboard.
 * @return Pointer to the one-dimensional array.
 */
static char *create_od_board();

/**
 * Convert a 2D chessboard representation to a 1D array.
 * @param board 2D chessboard.
 * @param od_board 1D array to store the board.
 */
static void to_one_dimension_char(wchar_t **board, char *od_board);

/**
 * Free the dynamically allocated one-dimensional board.
 * @param od_board Pointer to the one-dimensional chessboard.
 */
static void free_od_board(wchar_t *od_board);

/**
 * Free the dynamically allocated 2D chessboard.
 * @param board Pointer to the 2D chessboard.
 */
static void free_board(wchar_t **board);

/**
 * Translate a piece's char representation into a wide character for display.
 * @param char_piece Character representing a chess piece.
 * @return Wide character corresponding to the piece.
 */
static wchar_t translate_piece(char char_piece);

/**
 * Get the character representation of a chess piece.
 * @param piece Character representing the piece.
 * @return Corresponding character.
 */
char get_char_piece(char piece);

/**
 * Print the chessboard using ncurses.
 * @param board One-dimensional array representation of the board.
 */
void print_board_buff(char *board);

void print_board_buff_inverted(char *board);

#endif // CHESS_BOARD_H
