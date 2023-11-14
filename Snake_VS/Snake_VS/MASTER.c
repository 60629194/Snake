#include <stdio.h>
#include <Windows.h>
#include<stdlib.h>
#include<stdbool.h>
#include<conio.h>
#include"GamePlay.h"
#include"LeaderBoard.h"
#include"Settings.h"
#include"Store.h"

int main() {
    int choice = 0;
    int key;

    do {
        system("cls"); // Clears the console screen (Windows-specific)

        printf(" Welcome to Snake\n");
        printf("   Play %s\n", (choice == 0) ? "<" : "  ");
        printf("   Store %s\n", (choice == 1) ? "<" : "  ");
        printf("   Leader Board %s\n", (choice == 2) ? "<" : "  ");
        printf("   Settings %s\n", (choice == 3) ? "<" : "  ");
        printf("   Exit %s\n", (choice == 4) ? "<" : "  ");

        // Non-blocking key detection for arrow keys
        if (_kbhit()) {
            key = _getch();
            if (key == 224) { // Arrow key detected
                key = _getch(); // Get the extended key code
                switch (key) {
                case 72: // Up arrow key
                    if (choice > 0) {
                        choice--;
                    }
                    break;
                case 80: // Down arrow key
                    if (choice < 4) {
                        choice++;
                    }
                    break;
                }
            }
        }
        Sleep(100);
    } while (key != 13); // Continue until Enter key is pressed (13 is Enter's ASCII code)

    // Perform action based on the chosen option (choice)
    switch (choice) {
    case 0:
        GamePlay();
        break;
    case 1:
        Store();
        break;
    case 2:
        LeaderBoard();
        break;
    case 3:
        Settings();
        break;
    case 4:
        printf("Exiting...\n");
        exit(0);
        break;
    }

    return 0;
}