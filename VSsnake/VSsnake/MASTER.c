#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<Windows.h>
#include "GamePlay.h"
#include "Store.h"
#include "LeaderBoard.h"


void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main() {
    int choice = 1;
    char ch;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
	while (1) {
        system("cls"); // Clear screen

        printf("# Welcome to Snake\n");
        printf("    ·Play %s\n", (choice == 1) ? "◀" : "");
        printf("    ·Store %s\n", (choice == 2) ? "◀" : "");
        printf("    ·Leader Board %s\n", (choice == 3) ? "◀" : "");
        printf("    ·Exit %s\n", (choice == 4) ? "◀" : "");

        // Check for arrow key press
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (choice > 1) {
                choice--;
            }
            Sleep(150); // Optional delay to control the speed of navigation
        } else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (choice < 4) {
                choice++;
            }
            Sleep(150); // Optional delay to control the speed of navigation
        } else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            // Handle selection based on the value of 'choice'
            switch (choice) {
                case 1:
                    // Play option selected
                    GamePlay();
                    break;
                case 2:
                    // Store option selected
                    Store();
                    break;
                case 3:
                    // Leader Board option selected
                    LeaderBoard();
                    break;
                case 4:
                    // Exit option selected
					exit(0);
            }
        }
    }
}