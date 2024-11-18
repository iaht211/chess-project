#ifndef ELO_H
#define ELO_H

// Định nghĩa cấu trúc người chơi
typedef struct
{
    char name[50];
    int elo;
} Player;

// Hàm tính toán điểm Elo mới
int calculate_elo(int current_elo, int opponent_elo, double result, int k_factor);

// Hàm cập nhật điểm Elo cho hai người chơi
void update_elo(Player *player1, Player *player2, double result);

// Hàm ghi lịch sử trận đấu
void record_match(const char *filename, Player *player1, Player *player2, double result);

#endif
