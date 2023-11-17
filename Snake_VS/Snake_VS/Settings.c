#define _CRT_SECURE_NO_WARNINGS
#include"Settings.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
void switchAccount();
int deleteAccount(const char* filepath);
void resetAccount();
void trimFilePath(char* filepath);


int Settings(const char* filepath) {
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
        return 1;// return value of one will be directed to choose account page
        choice = 0;
        key = 10;
        break;
    case 1:
        PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
        int temp= deleteAccount(filepath);
        if (temp == 1) {
            return 1;
        }
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
int deleteAccount(const char* filepath) {
    char key[100];
    char userKey[100];
    system("cls");
    printf("WARNING\nare you sure to delete account, this action cannot be undone\ntype your account name to delete account.\n");
    strcpy(key, filepath);
    trimFilePath(key);
    scanf("%s", userKey);

    if (strcmp(key,userKey)==0) {
        system("cls");
        printf("deleting account.");
        Sleep(1000);
        printf(".");
        Sleep(1000);
        printf(".");
        Sleep(1000);
        remove(filepath);
        return 1;
    }
    else {
        system("cls");
        printf("user name does not match, you are being redirected to home page");
        Sleep(3000);
        return 0;
    }
}
void resetAccount() {
    printf("resetting account");
    Sleep(1000);
    return;
}

void trimFilePath(char* filepath) {
    // Trim off ".txt" from the filepath
    char* dotTxt = strstr(filepath, ".txt");
    if (dotTxt != NULL) {
        *dotTxt = '\0'; // Set the first character of ".txt" to null terminator
    }

    // Find the last occurrence of "account/"
    char* accountStr = strstr(filepath, "accounts/");
    if (accountStr != NULL) {
        // Move the pointer ahead to exclude "account/"
        accountStr += strlen("accounts/");
        memmove(filepath, accountStr, strlen(accountStr) + 1); // +1 for null terminator
    }
}