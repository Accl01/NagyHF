#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "board.h"
#include "leaderboard.h"
#include <ctype.h>
#include <string.h>

#define CLEAR "\033[2J\033[H"
//A jatekos kialakitasa. Egy nevbol, egy tablabol es egy tippekbol all. A headerben deffiniallatam.

//Talalat ellenorzeshez
#define JOBBRA nplayer->tabla[sor][oszlop + 1]
#define BALRA nplayer->tabla[sor][oszlop - 1]
#define FEL nplayer->tabla[sor - 1][oszlop]
#define LE nplayer->tabla[sor + 1][oszlop]

void hajolerakas(JATEKOS *player, char *input, bool *kilep){
    //Ellenorzes hogy kileptek e
    if(*kilep){
        return;
    }
    //Minden hajot lerakunk csokkeno sorrenben.
    for(int i = 6; i > 0; i--){
        if(i > 1){    
            bool ok = false;
            int h = i - 1;  // tenyleges hossza a lerakandonak
            while(!ok){
                //Mindig azzal kezdi hogy kiírja ki rakja le a hajoit és az o tablajat.
                printf(CLEAR);
                printf("\n%s hajólerakásköre", player->nev);
                printboard(player->tabla);
                printf("\nHova rakod le a %d hosszú hajót? ", h);
                scanf("%3s" , input);
                while(getchar() != '\n');

                if(input[0] == 'q'){
                    *kilep = true;
                    return;
                }
                //Atalakitjukj a bemenetet es elnevezzuk hogy hasznallhato legyen.
                int sor = toupper((unsigned char)input[0]) - 'A';
                int oszlop = input[1] - '0';
                char irany = toupper((unsigned char)input[2]);
                //Ezek utan elkezdjuk ellenorizni hogy hejes bemenet e.

                //Ellenorizzuk hogy csak 3 karakter a bemenet (+ \0).
                if(strlen(input) != 3){
                    printf("Hibás bemenet, próbáld újra (PL.: A1L) \n");
                    continue;
                }

                //Ellenorizuk hogy a megadott bemenetek a tablan belulre esnek.
                if(sor < 0 || sor >= PLAY_SIZE || oszlop < 0 || oszlop >= PLAY_SIZE || (irany != 'L' && irany != 'O')){
                    printf("Hibás bemenet, próbáld újra (PL.: A1L) \n");
                    continue;
                }

                // Ellenorizzuk hogy a bemenet minden resze belefer e a tablaba (a hajok vege nem log e ki a tablabol).
                if((irany == 'O' && sor + h - 1 >= PLAY_SIZE) || (irany == 'L' && oszlop + h - 1 >= PLAY_SIZE)){
                    printf("Kilóg a táblából, próbáld újra \n");
                    continue;
                }

                //Ellenorizzuk hogy a hajo amit leraknank nem utkozik e egy mar lerakott hajoval.
                bool ures = true;
                for(int j = 0; j < h; j++){
                    int x = sor;
                    int y = oszlop;
                    if(irany == 'O'){
                        x = sor + j;
                    }
                    else{
                        y = oszlop + j;
                    }
                    if(player->tabla[x][y] == '#'){
                        ures = false;
                        break;
                    }
                }
                if(!ures){
                        printf("Belelóg egy hajóba, próbáld újra \n");
                        continue;
                }

                //Ha mindennek megfeleltunk akkor feltoltjuk a tablat a hajoval.
                for(int j = 0; j < h; j++){
                    if(irany == 'O'){
                        player->tabla[sor + j][oszlop] = '#';
                    }
                    else{
                        player->tabla[sor][oszlop + j] = '#';
                    }
                }

                ok = true;     //Sikeres lerakás! Johet a kovetkezo hajo
            }
        }
        if(i == 1){
            printf(CLEAR);
            printf("\n%s lerakott hajói", player->nev);
            printboard(player->tabla);
            printf("\nNyomj Enter-t a továbblépéshez: ");
            
            int c;
            while((c = getchar()) != '\n'){
                getchar();
            }
        }
    }
}

void lepeskor(JATEKOS *tplayer, JATEKOS *nplayer , char *input, bool *kilep, char *nyertes){
    //Ellenorzes hogy kileptek e
    if(*kilep){
        return;
    }
    bool ervenyes = false;
    while(!ervenyes){
        printf(CLEAR);
        printf("\n%s tippelése:", tplayer->nev);
        printboard(tplayer->tippek);
        printf("\nMi a tipped? ");
        scanf("%3s" , input);
        while(getchar() != '\n');

        if(input[0] == 'q'){
            *kilep = true;
            return;
        }
        //Atalakitjukj a bemenetet es elnevezzuk hogy hasznallhato legyen.
        int sor = toupper((unsigned char)input[0]) - 'A';
        int oszlop = input[1] - '0';

        //Ellenorizzuk hogy csak 3 karakter a bemenet (+ \0).
        if(strlen(input) != 2){
            printf("Hibás bemenet, próbáld újra (PL.: A1) \n");
            continue;
        }
        //Ellenorizuk hogy a megadott bemenetek a tablan belulre esnek.
        if(sor < 0 || sor >= PLAY_SIZE || oszlop < 0 || oszlop >= PLAY_SIZE){
            printf("Hibás bemenet, próbáld újra (PL.: A1l) \n");
            continue;
        }

        if(tplayer->tippek[sor][oszlop] == 'X' || tplayer->tippek[sor][oszlop] == 'O'){
            printf("Ide már tippeltél!");
            continue;
        }

        ervenyes = true;

        if(nplayer->tabla[sor][oszlop] == '#'){
            tplayer->tippek[sor][oszlop] = 'X';
            tplayer->talalt++;
            printf(CLEAR);
            //Itt megvizsgalja hogy talalt vagy sullyedt is a hajo.
            //Az elso ketto sor azt vizsgalja hogy egy hosszu e a hajo
            if(((LE == ' ' && JOBBRA == ' ') && (FEL != '#' && BALRA != '#')) || ((LE == ' ' && BALRA == ' ') && (FEL != '#' && JOBBRA != '#')) || ((FEL == ' ' && BALRA == ' ') && (LE != '#' && JOBBRA != '#')) || ((FEL == ' ' && JOBBRA == ' ') && (LE != '#' && BALRA != '#'))){
                printf("\nTalált süllyedt!");
            }
            else if((sor == 9 && (FEL == ' ' && JOBBRA == ' ' && BALRA == ' ')) || (sor == 0 && (LE == ' ' && JOBBRA == ' ' && BALRA == ' ')) || (oszlop == 0 &&(JOBBRA == ' ' && FEL == ' ' && LE == ' ')) || (oszlop == 9 && (BALRA == ' ' && FEL == ' ' && LE == ' ')) || (FEL == ' ' && LE == ' ' && JOBBRA == ' ' && BALRA == ' ')){
                printf("\nTalált süllyedt!");
            }
            //Innentol az altalanos vizsgalat
            else if(((nplayer->tabla[sor + 1][oszlop] == '#') && (tplayer->tippek[sor + 1][oszlop] == ' ')) && ((nplayer->tabla[sor - 1][oszlop] == '#') && (tplayer->tippek[sor - 1][oszlop] == ' ')) || ((nplayer->tabla[sor][oszlop + 1] == '#') && (tplayer->tippek[sor][oszlop + 1] == ' ')) && ((nplayer->tabla[sor][oszlop - 1] == '#') && (tplayer->tippek[sor][oszlop - 1] == ' '))){
                printf("\nTalált!");
            }
            else if(((nplayer->tabla[sor + 1][oszlop] == '#') && (tplayer->tippek[sor + 1][oszlop] == ' ')) || ((nplayer->tabla[sor - 1][oszlop] == '#') && (tplayer->tippek[sor - 1][oszlop] == ' ')) || ((nplayer->tabla[sor][oszlop + 1] == '#') && (tplayer->tippek[sor][oszlop + 1] == ' ')) || ((nplayer->tabla[sor][oszlop - 1] == '#') && (tplayer->tippek[sor][oszlop - 1] == ' '))){
                printf("\nTalált!");
            }

            //Megvizsgalja hogy elsülyedt e a hajo. Megnezi hogy ha a vegehez ertel a hajonak és vegig megy a hajon amig nem hajo mezot talal.
            // Ha vegig az osszeset mar betippelte a jatekos akkor elsulyedt.
            else if(((nplayer->tabla[sor + 1][oszlop] == '#') && (tplayer->tippek[sor + 1][oszlop] == 'X'))){
                bool mindtalalt = true;
                for(int z = 1; nplayer->tabla[sor + z][oszlop] == '#'; z++){
                    if(tplayer->tippek[sor + z][oszlop] == ' '){
                        mindtalalt = false;
                    }
                }
                if(!mindtalalt){
                    printf("\nTalált!");
                }else{
                    printf("\nTalált süllyedt!");
                }
            }
            else if(((nplayer->tabla[sor - 1][oszlop] == '#') && (tplayer->tippek[sor - 1][oszlop] == 'X'))){
                bool mindtalalt = true;
                for(int z = 1; nplayer->tabla[sor - z][oszlop] == '#'; z++){
                    if(tplayer->tippek[sor - z][oszlop] == ' '){
                        mindtalalt = false;
                    }
                }
                if(!mindtalalt){
                    printf("\nTalált!");
                }else{
                    printf("\nTalált süllyedt!");
                }
            }
            else if(((nplayer->tabla[sor][oszlop + 1] == '#') && (tplayer->tippek[sor][oszlop + 1] == 'X'))){
                bool mindtalalt = true;
                for(int z = 1; nplayer->tabla[sor][oszlop + z] == '#'; z++){
                    if(tplayer->tippek[sor][oszlop + z] == ' '){
                        mindtalalt = false;
                    }
                }
                if(!mindtalalt){
                    printf("\nTalált!");
                }else{
                    printf("\nTalált süllyedt!");
                }
            }
            else if(((nplayer->tabla[sor][oszlop - 1] == '#') && (tplayer->tippek[sor][oszlop - 1] == 'X'))){
                bool mindtalalt = true;
                for(int z = 1; nplayer->tabla[sor][oszlop - z] == '#'; z++){
                    if(tplayer->tippek[sor][oszlop - z] == ' '){
                        mindtalalt = false;
                    }
                }
                if(!mindtalalt){
                    printf("\nTalált!");
                }else{
                    printf("\nTalált süllyedt!");
                }
            }

        //Ha nem hajo van ott akkor nemtalalt
        }else{
            tplayer->tippek[sor][oszlop] = 'O';
            printf(CLEAR);
            printf("\nNemtalált!");
        }

    }
    printboard(tplayer->tippek);

    if(tplayer->talalt == 15){
        strcpy(nyertes, tplayer->nev);
    }

    printf("\nNyomd meg az Enter-t a továbblépéshez! ");
    getchar();
}




//A játék függvény. Meghivja a hajolerakast es a lepeskort..
void game_start(char *winner, int *kilepett){
    char input[4];
    char nyertes[24];
    bool kilep = false;
    //Jatek elinditasa. A ket jatekos letrehozasa es tablaik feltoltese ' ' el.
    printf(CLEAR);
    printf("\nJáték indítása...\n");
    JATEKOS player1, player2;
    
    for(int x = 0; x < PLAY_SIZE; x++){
        for(int y = 0; y < PLAY_SIZE; y++){
            player1.tabla[x][y] = ' ';
            player1.tippek[x][y] = ' ';
            player2.tabla[x][y] = ' ';
            player2.tippek[x][y] = ' ';
        }
    }
    player1.talalt = 0; 
    player2.talalt = 0;
    //Jatekosok nevenek megadasa és a hajok lerakasa.
    printf("\nELső játékos neve: ");
    scanf("%24s", player1.nev);
    printf(CLEAR);
    printf("\n");
    printf("\nMásodik játékos neve: ");
    scanf("%24s", player2.nev);
    printf(CLEAR);
    hajolerakas(&player1, input, &kilep);
    if(kilep){
        *kilepett = 1;
        return;
    }
    hajolerakas(&player2, input, &kilep);
    if(kilep){
        *kilepett = 1;
        return;
    }
    //A jatek tenyleges elkezdese. 
    //Kinek a koret meghatarozo valtozo.
    int kor = 1;
    while((player1.talalt < 15 && player2.talalt < 15) && kilep != true){
        //Ha paratlan a szam akkor az elso jatekos kore van, ha paros akkor a masodike.
        printf(CLEAR);
        if(kor % 2 != 0){
            lepeskor(&player1, &player2, input, &kilep, nyertes);
        }else{
            lepeskor(&player2, &player1, input, &kilep, nyertes);
        }
        kor++;
    }
    if(kilep){
        *kilepett = 1;
        return;
    }

    strcpy(winner, nyertes);
    printf(CLEAR);
    printf("\nGratulálunk!!\nA nyertes: %s\n", nyertes);
    printf("\nNyomj ENTER-t a kilépéshez: ");
    int c;
    while((c = getchar()) != '\n'){
        getchar();
    }
}