#ifndef LEADERBOARD_H
#define LEADERBOARD_H

typedef struct PLAYER{
    char nev[25];
    int wins;
}PLAYER;

void loadleaderboard();
void saveleaderboard();
void updateleaderboard(const char *nyertes);
void printleaderboard();
void freeleaderboard();

#endif