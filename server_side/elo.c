#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 100

// Cấu trúc để lưu thông tin người chơi
typedef struct
{
    char name[50];
    int password;
    int elo;
    int matches;
} User;

// Hàm tính điểm Elo mới
double calculate_new_elo(int current_elo, int opponent_elo, double result, int k_factor)
{
    // double expected_score = 1.0 / (1.0 + pow(10, (double)(opponent_elo - current_elo) / 400));
    // return current_elo + k_factor * (result - expected_score);
    return 1.0;
}

// Hàm đọc dữ liệu từ file
int read_users(const char *filename, User users[], int *count)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Không thể mở file");
        return 0;
    }

    *count = 0;
    while (fscanf(file, "%s %d %d %d",
                  users[*count].name,
                  &users[*count].password,
                  &users[*count].elo,
                  &users[*count].matches) == 4)
    {
        (*count)++;
    }

    fclose(file);
    return 1;
}

// Hàm ghi dữ liệu người chơi vào file
void write_users(const char *filename, User users[], int count)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Không thể ghi file");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %d %d %d\n", users[i].name, users[i].password, users[i].elo, users[i].matches);
    }

    fclose(file);
}

// Hàm xử lý tính điểm Elo
void update_elo(User users[], int count)
{
    char name[50];
    int opponent_elo, k_factor = 20;
    double result;

    printf("Nhập tên người chơi: ");
    scanf("%s", name);

    // Tìm người chơi
    int index = -1;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(users[i].name, name) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Không tìm thấy người chơi!\n");
        return;
    }

    // Nhập thông tin trận đấu
    printf("Nhập Elo của đối thủ: ");
    scanf("%d", &opponent_elo);
    printf("Nhập kết quả (1: thắng, 0.5: hòa, 0: thua): ");
    scanf("%lf", &result);

    // Tính Elo mới
    int new_elo = (int)calculate_new_elo(users[index].elo, opponent_elo, result, k_factor);
    printf("Điểm Elo mới của %s: %d -> %d\n", users[index].name, users[index].elo, new_elo);

    // Cập nhật thông tin người chơi
    users[index].elo = new_elo;
    users[index].matches++;
}

// int main()
// {
//     User users[MAX_USERS];
//     int count;

//     // Đọc thông tin người chơi từ file
//     if (!read_users("../user.txt", users, &count))
//     {
//         return 1;
//     }

//     // Hiển thị thông tin người chơi
//     printf("Danh sách người chơi:\n");
//     for (int i = 0; i < count; i++)
//     {
//         printf("%s - Elo: %d, Trận đấu: %d\n", users[i].name, users[i].elo, users[i].matches);
//     }

//     // Cập nhật Elo
//     update_elo(users, count);

//     // Ghi thông tin người chơi trở lại file
//     write_users("user.txt", users, count);

//     return 0;
// }
