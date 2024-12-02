#ifndef BOARD_H
#define BOARD_H

#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#define BOARD_SIZE 8
void debug_print_board(wchar_t **);
wchar_t ** create_board();
void initialize_board(wchar_t ** p_board);
char * create_od_board();
void to_one_dimension_char(wchar_t ** board, char * od_board);
void free_od_board(wchar_t * od_board);
void free_board(wchar_t ** board);
void debug_print_board(wchar_t ** board);
wchar_t translate_piece (char char_piece);
void print_board_buff(char * board);
void print_board_buff_inverted(char * board);

#endif // BOARD_H