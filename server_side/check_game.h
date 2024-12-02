#ifndef CHECK_GAME_H
#define CHECK_GAME_H

bool is_diagonal(int, int);
void move_piece(wchar_t **board, int *move);
void freeAll(int *piece_team, int *x_moves, int *y_moves);
bool emit(int client, char *message, int message_size);
void translate_to_move(int *move, char *buffer);
void broadcast(wchar_t **board, char *one_dimension_board, int player_one, int player_two);
int getManitud(int origin, int dest);
bool is_diagonal_clear(wchar_t **board, int *move);
bool is_syntax_valid(int player, char *move);
int get_piece_team(wchar_t **board, int x, int y);
int get_piece_type(wchar_t piece);
bool eat_piece(wchar_t **board, int x, int y);
bool is_rect(int *move);
int is_rect_clear(wchar_t **board, int *move, int x_moves, int y_moves);
void promote_piece(wchar_t **board, int destX, int destY, int team);
bool is_diagonal(int x_moves, int y_moves);
bool is_move_valid(wchar_t **board, int player, int team, int *move);
bool check_end_game(wchar_t **board);

#endif // CHECK_GAME_H