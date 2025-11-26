#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "board.h"
#include "game.h"
#include "leaderboard.h"
#include "debugmalloc.h"

#define CLEAR "\033[2J\033[H"

int main(void){
    #ifdef _WIN32 //terminalba beirod hogy chcp 65001
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif
    
    loadleaderboard();
    char valasztas;
    char winner[24];
    int kilepett = 0;

    do{
        printf(CLEAR);
        printf("\n");
        printf("//*-Torpedó játék menu-*\\\\ \n");
        printf("//*  Új játék! --- (1) *\\\\ \n");
        printf("//* Leaderboard -- (2) *\\\\ \n");
        printf("//* Kilépés ------ (q) *\\\\ \n");
        printf("\n");
        printf("Válassz egy opciót: ");
        scanf(" %c", &valasztas);

        switch(valasztas){
            case '1':
                printf(CLEAR);
                game_start(winner, &kilepett);
                //Csak akkor updateli a leaderboardot ha nem léptek ki
                if(kilepett == 0){
                    updateleaderboard(winner);
                }
                break;
            case '2':
                printf(CLEAR);
                printleaderboard();
                break;
            case 'q':
                saveleaderboard();
                freeleaderboard();
                printf(CLEAR);
                return 0;

            default:
                continue;
        }
    } while(valasztas != 'q');
    return 0;
}