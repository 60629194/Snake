#include"Settings.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
void switchAccount();
void deleteAccount();
void resetAccount();


int Settings() {
    int key=10;
    int choice = 0;
    do {
        system("cls"); // Clears the console screen (Windows-specific)

        printf("Settings\n");
        printf("   Switch account %s\n", (choice == 0) ? "<" : "  ");
        printf("   Delete account %s\n", (choice == 1) ? "<" : "  ");
        printf("   Reset account %s\n", (choice == 2) ? "<" : "  ");
        printf("   Return to home page %s\n", (choice == 3) ? "<" : "  ");

        // Non-blocking key detection for arrow keys
        if (_kbhit()) {
            key = _getch();
            if (key == 224) { // Arrow key detected
                key = _getch(); // Get the extended key code
                switch (key) {
                case 72: // Up arrow key
                    if (choice > 0) {
                        PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        choice--;
                    }
                    break;
                case 80: // Down arrow key
                    if (choice < 3) {
                        PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        choice++;
                    }
                    break;
                }
            }
        }
        Sleep(100);
    } while (key != 13); // Continue until Enter key is pressed (13 is Enter's ASCII code)
    switch (choice) {
    case 0:
        PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
        switchAccount();
        return 1;
        choice = 0;
        key = 10;
        break;
    case 1:
        PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
        deleteAccount();
        choice = 0;
        key = 10;
        break;
    case 2:
        PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
        resetAccount();
        choice = 0;
        key = 10;
        
        break;
    case 3:
        PlaySound(TEXT("invertNavigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
        return 0;
    }
}

void switchAccount() {
    system("cls");
    printf("switching account");
    Sleep(1000);
    return;
}
void deleteAccount() {
    printf("deleting account");
    Sleep(1000);
    return;
}
void resetAccount() {
    printf("resetting account");
    Sleep(1000);
    return;
}