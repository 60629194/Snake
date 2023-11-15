#include "GamePlay.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

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
    initialize_plate(M);
    initialize_plate(T);

    M[3][1] = 1;
    int coin = 0;

    while (coin < 5) {
        int a = rand() % 7;
        int b = rand() % 9;

        M[a][b] = 2;

        print_plate(M);
    }


   
    return 0;
}