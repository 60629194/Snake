#include"LeaderBoard.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
void LeaderBoard() {
    system("cls");
    int scores[] = { 1000, 850, 720, 650, 500 };
    const int numScores = sizeof(scores) / sizeof(scores[0]);

    printf(" Leaderboard\n");
    printf(" ------------\n");
    for (int i = 0; i < numScores; ++i) {
        printf(" %d. Score: %d\n", i + 1, scores[i]);
    }
    printf("\n Press 'q' to return to the Homepage...\n");

    char userInput;
    do {
        userInput = _getch();
    } while (userInput != 'q' && userInput != 'Q');
    PlaySound(TEXT("invertNavigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
    return;
}