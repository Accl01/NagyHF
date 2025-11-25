#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaderboard.h"

#include "debugmalloc.h"

#define CLEAR "\033[2J\033[H"


//Ezekez a valtozokatt es fuggvenyt nem kell hogy a tobbi file lassa.
static PLAYER *leaderboard = NULL;
static int playerdb = 0;            // Tudjuk mennyi ember van a dicsoseglistaban
static int kapacitas = 0;           // Lefoglalt memoriaterulet

//Ha betelik a leaderboradnak a foglalt helye foglal meg.
static void ujrafoglal(){
    if(playerdb < kapacitas){
        return;
    }
    int ujkapacitas;
    if(kapacitas == 0){
        ujkapacitas = 10;
    }else{
        ujkapacitas = kapacitas*2;
    }

    PLAYER *ujleaderboard = realloc (leaderboard, ujkapacitas*sizeof(PLAYER));
    if(ujleaderboard == NULL){
        printf("Hibas memoriafoglalas.");
    }
    leaderboard = ujleaderboard;
    kapacitas = ujkapacitas;
}

//A qsort feltétele. Csükkenő sorrendben írja ki.
static int osszehasonlit(const void *a, const void *b){
    PLAYER *p1 = (PLAYER*)a;
    PLAYER *p2 = (PLAYER*)b;
    return p2->wins - p1->wins;
}

//Betolti a leaderboardot a txt filebol a dinamikusan foglalt memoriareszre.
void loadleaderboard(){
    FILE *file = fopen("leaderboard.txt", "r");
    if(!file){
        printf("Nincs leaderboard file. Új lesz letrehozva.\n");
        return;
    }
    char nev[25];
    int wins;
    while(fscanf(file, "%s %d", nev, &wins) == 2){
        ujrafoglal();
        strcpy(leaderboard[playerdb].nev, nev);
        leaderboard[playerdb].wins = wins;
        playerdb++;
    }
    fclose(file);
}

//Visszatolti a txtbe a leaderboardot.
void saveleaderboard(){
    FILE *file = fopen("leaderboard.txt", "w");
    if(!file){
        printf("Hiba trötént a mentés közben\n");
        return;
    }
    for(int i = 0; i < playerdb; i++){
        fprintf(file, "%s %d\n", leaderboard[i].nev, leaderboard[i].wins);
    }
    fclose(file);
}

//Az új nyertest belerakja a leaderboardba
void updateleaderboard(const char *nyertes){
    //Ha mar ez a nev szerepel a listaban
    for(int i = 0; i < playerdb; i++){
        if(strcmp(leaderboard[i].nev, nyertes) == 0){
            leaderboard[i].wins++;
            return;
        }
    }
    //Ha meg nem szerepel a nev a lsitaban
    ujrafoglal();
    strcpy(leaderboard[playerdb].nev, nyertes);
    leaderboard[playerdb].wins = 1;
    playerdb++;
}

//Kiirja a leaderboardot miutan szortirozta csokkeno sorrendbe.
void printleaderboard(){
    printf(CLEAR);
    if(playerdb == 0){
        printf("Nincs senki a leaderboardon.\n");
        return;
    }
    
    qsort(leaderboard, playerdb, sizeof(PLAYER), osszehasonlit);
    printf("\n//*----     Leaderboard    ----*\\\\\n");
    printf("//* %-21s%3s  *\\\\\n", "Név", "Nyerés");
    printf("//*-----------------------------*\\\\\n");
    for(int i = 0; i < playerdb; i++){
        printf("//* %-24s - %3d\n",leaderboard[i].nev, leaderboard[i].wins);
    }
    printf("//*-----------------------------*\\\\\n");

    printf("\nNyomd meg a q-t a kilépéshez: ");
    getchar();
    while(getchar() != 'q');
}   

void freeleaderboard(){
    free(leaderboard);
    leaderboard = NULL;
    playerdb = 0;
    kapacitas = 0;
}
