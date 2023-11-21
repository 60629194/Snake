#include "GamePlay.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

int scdata;


//print background 
void print_plate(int array[8][10]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            if (array[i][j] == 1) {
                printf("s ");//snake 
            }
            else if (array[i][j] == -1) {
                printf("a ");//apple 
            }
            else if (array[i][j] == -2) {
                printf("c ");
            }
            else if (array[i][j] == 0) {
                printf("_ ");//empty 
            }
            else if (array[i][j] == 3) {
                printf("  ");
            }
        }
        printf("\n");
    }
}

//initialize  background
void initialize_plate(int array[8][10]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            array[i][j] = 0;
        }
    }
}


char nonBlockingInput() {
    if (_kbhit()) { // 安砞ㄏノ琌 conio.h い _kbhit ㄧ计
        return _getch(); // 安砞ㄏノ琌 conio.h い _getch ㄧ计
    }
    return '\0';
}

char keyboardhit(char input) {
    if (_kbhit()) {
        int key = _getch();
        if (key == 0xE0) {
            key = _getch();
            switch (key) {  //keyboardhit reaction to change direction
            case 0x48:
                input = 'w';
                break;
            case 0x4B:
                input = 'a';
                break;
            case 0x50:
                input = 's';
                break;
            case 0x4D:
                input = 'd';
                break;
            }
        }
    }
    return input;
}

void level(int level) {
    int M[8][10];
    int apple = 0;
    int coin = 0;
    int a, b, c, d;
    int snakePositionv = 3;
    int snakePositionh = 1;
    char direction = 'i';
    while (level < 20) {
        if (snakePositionv > 8 || snakePositionh > 10 || snakePositionv < 0 || snakePositionh < 0) {//boundary check3
            break;
        }

        apple = 0;
        snakePositionv = 3;
        snakePositionh = 1;
        direction = 'i';

        while (1) {
            if (snakePositionv > 8 || snakePositionh > 10 || snakePositionv < 0 || snakePositionh < 0) {//boundary check2
                Sleep(3000);
                break;
            }

            srand(time(0));
            a = rand() % 7;
            b = rand() % 9;
            c = rand() % 7;
            d = rand() % 9;
            while (c == a || b == d) {
                c = rand() % 7;
                d = rand() % 9;
            }

            while (apple < 3) {

                printf("level = %d\n", level + 1);
                printf("apple = %d\n", apple);
                printf("coin = %d\n", coin);

                initialize_plate(M);
                M[a][b] = -1;   //applePos
                M[c][d] = -2;   //coinPos
                M[snakePositionv][snakePositionh] = 1;  //snakePos
                
                char noninput = nonBlockingInput();
                if (noninput != '\0') {
                    direction = noninput;
                    switch (direction) {
                    case 'w':
                        snakePositionv--;
                        break;
                    case 'a':
                        snakePositionh--;
                        break;
                    case 's':
                        snakePositionv++;
                        break;
                    case 'd':
                        snakePositionh++;
                        break;
                    }
                    print_plate(M);
                }
                else {
                    switch (direction) {
                    case 'w':
                        snakePositionv--;
                        break;
                    case 'a':
                        snakePositionh--;
                        break;
                    case 's':
                        snakePositionv++;
                        break;
                    case 'd':
                        snakePositionh++;
                        break;
                    }
                    //print_plate(M);
                    system("cls");
                    print_plate(M);
                    Sleep(1000 / (level + 1));
                }
                /*switch (direction) {
                    case 'w':
                        snakePositionv--;
                        break;
                    case 'a':
                        snakePositionh--;
                        break;
                    case 's':
                        snakePositionv++;
                        break;
                    case 'd':
                        snakePositionh++;
                        break;
                    }
                print_plate(M);
                Sleep(1000/(level+1));
                system("cls");*/
                

                if (snakePositionv > 8 || snakePositionh > 10 || snakePositionv < 0 || snakePositionh < 0) {//boundary check1;
                    printf("game over");
                    Sleep(3000);
                    break;
                }

<<<<<<< Updated upstream
                Sleep(1000 / (level + 1));
                system("cls");

                if (a == snakePositionv && b == snakePositionh) {   //snakePos=coinPos
                    PlaySound(TEXT("eatSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
=======
                if (a == snakePositionv && b == snakePositionh) {   //snakePos=applePos
                    PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
>>>>>>> Stashed changes
                    apple++;
                    break;
                }
                if (c == snakePositionv && d == snakePositionh) {    //snakePos=coinPos
                    coin++;
                    c = rand() % 7;
                    d = rand() % 9;
                    while (c == a || b == d) {
                        c = rand() % 7;
                        d = rand() % 9;
                    }
                }
            }
            if (apple == 3) {
                level++;
                printf("Victory, 3 seconds for next stage");
                Sleep(3000);
                break;
            }
        }
    }
}


void GamePlay(const char* filepath) {
    int M[8][10];

    M[3][1] = 1;    //start point
    int apple = 0;
    int a, b;
    int sum;
    int stage = 0;
    int snakePositionv = 3;
    int snakePositionh = 1;
    char direction='d';  //set initial=+x way

    initialize_plate(M);

    level(stage);

<<<<<<< Updated upstream
    sum= stage * 50 + apple * 16;
    scdata = sum;
=======
>>>>>>> Stashed changes

    return ;
}

