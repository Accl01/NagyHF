#include "board.h"
#include <stdio.h>

#define PLAY_SIZE 10
#define PBOARD_W 45
#define PBOARD_H 23

#define BLUE "\033[1;34m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"
#define GRIDE "\u2523\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u254B\u2501\u2501\u2501\u252B"
//Jatékos tablaja amivel a szamolasok tortennek. Feltoltjuk ' ' el hogy a kiírásnál a tabla ne legyen elcsuszva.
void player_board(char board[PLAY_SIZE][PLAY_SIZE]){
    for(int i = 0; i < PLAY_SIZE; i++){
        for(int j = 0; j < PLAY_SIZE; j++){
            board[i][j] = ' ';
        }
    }
}


// A tabla kiirasa szepen vonalakkal.
void printboard(char board[PLAY_SIZE][PLAY_SIZE]){
    printf("\n");
    //Legfelso sor kiirasa. Csak vonalak.
    printf("\u250F");
    for(int j = 0; j < 11; j++){
        if(j != 10){   
            printf("\u2501\u2501\u2501\u2533");
        }
        else{
            printf("\u2501\u2501\u2501\u2513");
        }
    }
    printf("\n");
    //Fejléc kiirasa
    char BETU = 'A';
    for(int j = 0; j < 11; j++){
        if(j == 0){
            printf("\u2503   \u2503");
        }
        else{
            printf(YELLOW" %c "RESET, BETU);
            printf("\u2503");
            BETU++;
        }
    }
    printf("\n");
    //A tabla nagyreszenek kiirasa az oldalso szammokkal es a jatekos tenyleges tablajanak belefoglalasaval.
    int SZAM = 0;
    for(int i = 2; i < PBOARD_H - 1; i++){
        if(i % 2 == 0){
            printf(GRIDE"\n");
        }
        else{
            for(int j = 0; j < 11; j++){
                if(j == 0){ // Az oldalso kordinata szamok kiirasa.
                    printf("\u2503");
                    printf(YELLOW" %d "RESET, SZAM);
                    printf("\u2503");
                    SZAM++;
                }
                else{ // a jatekos tippjeinek vagy hajoinak a kiirasa
                    if(board[j - 1][SZAM - 1] == '#'){
                        printf(BLUE" %c "RESET, board[j - 1][SZAM - 1]); // Ha # akkor kek
                    }
                    else if(board[j - 1][SZAM - 1] == 'X'){
                        printf(RED" %c "RESET, board[j - 1][SZAM - 1]); // Ha X akkor piros
                    }
                    else if(board[j - 1][SZAM - 1] == 'O'){
                        printf(YELLOW" %c "RESET, board[j - 1][SZAM - 1]); // Ha O akkor sarga
                    }
                    else{
                        printf(" %c ", board[j - 1][SZAM - 1]); // Ha space akkor semmi
                    }
                    printf("\u2503");
                }
            }
            printf("\n");
        }
    }
    //Az utolso sor lerakasa, szinten csak vonalak.
    printf("\u2517");
    for(int j = 0; j < 11; j++){
        if(j != 10){   
            printf("\u2501\u2501\u2501\u253B");
        }
        else{
            printf("\u2501\u2501\u2501\u251B");
        }
    }
}
