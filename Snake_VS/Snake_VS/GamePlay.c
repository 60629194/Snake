#include "GamePlay.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

//print background 
void print_plate(int array[8][10]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            if (array[i][j] == 1) {
                printf("s ");//snake 
            }
            else if (array[i][j] == -1) {
                printf("c ");//coin 
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

//equalize array1=array2 
void equalize_plate(int array1[8][10], int array2[8][10]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            array1[i][j] = array2[i][j];
        }
    }
}

void GamePlay() {
    int T[8][10];
    int M[8][10];

    initialize_plate(T);

    M[3][1] = 1;    //start point
    int coin = 0;
    int a, b;
    int snakePositionv = 3;
    int snakePositionh = 1;
    char direction='d';  //set initial=+x way

    initialize_plate(M);


    while (1) {
        if (snakePositionv > 8 || snakePositionh > 10 || snakePositionv < -1 || snakePositionh < -1) {//boundary check2
            Sleep(3000);
            break;
        }
        srand(time(0));
        a = rand() % 7;
        b = rand() % 9;

        while (coin < 3) {
            if (_kbhit()) {
                char key = _getch();
                switch (key) {  //keyboardhit reaction to change direction
                case 'w':
                    direction = 'w';
                    break;
                case 'a':
                    direction = 'a';
                    break;
                case 's':
                    direction = 's';
                    break;
                case 'd':
                    direction = 'd';
                    break;
                }
            }
            initialize_plate(M);
            M[a][b] = -1;   //coinPos
            M[snakePositionv][snakePositionh] = 1;  //snakePos
            print_plate(M);

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
            if (snakePositionv > 8 || snakePositionh > 10 || snakePositionv < -1 || snakePositionh < -1) {//boundary check1;
                printf("game over");
                Sleep(3000);
                break;
            }
            Sleep(500);
            system("cls");

            if (a == snakePositionv && b == snakePositionh) {   //snakePos=coinPos
                coin++;
                break;
            }
        }
        if (coin == 3) {
            printf("Victory");
            Sleep(3000);
            break;
        }
    }

    return ;
}