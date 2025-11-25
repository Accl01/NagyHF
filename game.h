#ifndef GAME_H
#include <stdbool.h>
#define GAME_H
#define PLAY_SIZE 10

typedef struct JATEKOS{
    char nev[25];
    char tabla[PLAY_SIZE][PLAY_SIZE];
    char tippek[PLAY_SIZE][PLAY_SIZE];
    int talalt;
} JATEKOS;

void game_start(char *winner, int *kilepett);
void hajolerakas(JATEKOS *player, char *input, bool *kilep);
void lepeskor(JATEKOS *tplayer, JATEKOS *nplayer , char *input, bool *kilep, char *nyertes);

#endif