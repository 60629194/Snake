#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "GamePlay.h"
#include "LeaderBoard.h"
#include "Settings.h"
#include "Store.h"
#include "createAccount.h"

#define MAX_MENU_ITEMS 100
#define MAX_FILENAME_LENGTH 100

long int findSize(char file_name[]) ;
void displayMenu(char menuItems[][MAX_FILENAME_LENGTH], int itemCount, int choice);

int main() {
    int choice =0;
    int key=10;
    char account[100];

    system("checkAccount.bat");

    long int accountSize=findSize("log\\accountDATA.txt");
    printf("%d",accountSize);
    if(accountSize<3){
        printf("create an account");
    }
    else{
        printf("choose an account");

        FILE *file = fopen("log\\accountDATA.txt", "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            return 1;
        }

        char menu[MAX_MENU_ITEMS][MAX_FILENAME_LENGTH];
        int itemCount = 0;

        // Read menu items from the file
        while (itemCount < MAX_MENU_ITEMS && fscanf(file, "%s", menu[itemCount]) == 1) {
            itemCount++;
        }
        fclose(file);

        do {
            displayMenu(menu, itemCount, choice);

            if (_kbhit()) {
                key = _getch();
                if (key == 224) {
                    key = _getch();
                    switch (key) {
                        case 72: // Up arrow key
                            if (choice > 0) {
                                PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
                                choice--;
                            }
                            break;
                        case 80: // Down arrow key
                            if (choice < itemCount+1) {
                                PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
                                choice++;
                            }
                            break;
                    }
                }
            }
            Sleep(100);
        } while (key != 13);

        if (choice < itemCount) {
            // Copy the selected account to 'account'
            strcpy(account, menu[choice]);

            // Remove the ".txt" extension from 'account'
            char* dot = strchr(account, '.');
            if (dot != NULL) {
                *dot = '\0'; // Null-terminate the string at the position of the dot
            }
            system("cls");
            printf("you chosen %s", account);
            Sleep(3000);
        }
        else if (choice == itemCount) {
            system("cls");
            createAccount();
            Sleep(3000);
        }
        else if(choice==itemCount+1){
            system("cls");
            printf("exit game");
            PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME);
            exit(0);
        }

    }


    choice=0;
    key=10;



    while (1) { // Loop to keep displaying the menu until the user chooses to exit
        do {
            system("cls"); // Clears the console screen (Windows-specific)

            printf("Welcome to Snake\n");
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
                            PlaySound(TEXT("navigateSFX.wav"),NULL,SND_FILENAME | SND_ASYNC);
                            choice--;
                        }
                        break;
                    case 80: // Down arrow key
                        if (choice < 4) {
                            PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
            PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
            GamePlay();
            choice = 0;
            key = 10;
            break;
        case 1:
            PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
            Store();
            choice = 0;
            key = 10;
            break;
        case 2:
            PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
            LeaderBoard();
            choice = 0;
            key = 10;
            break;
        case 3:
            PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
            Settings();
            choice = 0;
            key = 10;
            break;
        case 4:
            printf("Exiting...\n");
            PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME );
            exit(0);
            break;
        }
    }

    return 0;
}


long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 
  
    return res; 
}

void displayMenu(char menuItems[][MAX_FILENAME_LENGTH], int itemCount, int choice) {
    system("cls"); // Clears the console screen (Windows-specific)
    printf("Select or create account\n");
    int i;
    for (i = 0; i < itemCount; ++i) {
        // Removing the file extension ".txt" from the displayed menu
        char fileName[MAX_FILENAME_LENGTH];
        strcpy(fileName, menuItems[i]);
        char* dot = strchr(fileName, '.');
        if (dot != NULL) {
            *dot = '\0'; // Null-terminate the string at the position of the dot
        }

        printf("   %s%s\n", fileName, (choice == i) ? "<" : "  ");
    }
    printf("   create account%s\n", (choice == i) ? "<" : "  ");
    printf("   Exit%s\n", (choice == i + 1) ? "<" : "  ");
}