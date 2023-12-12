#define _CRT_SECURE_NO_WARNINGS
#include"createAccount.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include"global.h"


static int containsSpecialChars(const char* name);
static void addExtension(char* result, const char* name);

void createAccount() {
START:
    printf("enter your user name:\n");
	char name[100];
    char accountPath[100];
	fgets(name, sizeof(name), stdin);
    removeNewLine(name);
    int length = strlen(name);
    int containsSpace = 0; // Flag to track if space is present
    for (int i = 0; i < length; ++i) {
        if (name[i] == ' ') {
            containsSpace = 1; // Space found, set the flag
            break;
        }
    }
    if (containsSpace) {
        PlaySound(TEXT("accessDenied.wav"), NULL, SND_FILENAME | SND_ASYNC);
        system("cls");
        printf("Your name cannot contain space.\n");
        printf("press any key to continue\n");
        _getch();
        system("cls");
        goto START;
    }
    if (containsSpecialChars(name)) {
        PlaySound(TEXT("accessDenied.wav"), NULL, SND_FILENAME | SND_ASYNC);
        system("cls");
        printf("Your name cannot contain special characters.\n");
        printf("press any key to continue\n");
        _getch();
        system("cls");
        goto START;
    }
    if (length>50) {
        PlaySound(TEXT("accessDenied.wav"), NULL, SND_FILENAME | SND_ASYNC);
        system("cls");
        printf("Your name cannot be longer than 50 words.\n");
        printf("press any key to continue\n");
        _getch();
        system("cls");
        goto START;
    }
    if (length <= 1) {
        PlaySound(TEXT("accessDenied.wav"), NULL, SND_FILENAME | SND_ASYNC);
        system("cls");
        printf("Your name cannot be nothing.\n");
        printf("press any key to continue\n");
        _getch();
        system("cls");
        goto START;
    }

    
    addExtension(accountPath, name);
    system("cls");
    FILE* file = fopen(accountPath, "w");
    if (file != NULL) {
        fprintf(file, "0\n");
        fprintf(file, "0\n");
        fprintf(file, "0\n");
        fprintf(file, "0\n");
        fprintf(file, "0");
        fclose(file);

        // Reopen the file in read mode to get its size
        FILE* reopenedFile = fopen(accountPath, "r");
        if (reopenedFile != NULL) {
            fseek(reopenedFile, 0, SEEK_END);
            int fileSize = ftell(reopenedFile);
            fclose(reopenedFile);
            printf("Account created successfully.\n");
            PlaySound(TEXT("accessGranted.wav"), NULL, SND_FILENAME);
        }
        else {
            perror("Error opening the file to get size");
            exit(1);
        }
    }
    else {
        perror("Error creating account");
        exit(1);
    }
}
int containsSpecialChars(const char* name) {
    int length = strlen(name);
    const char* specialChars = ". , / ! @ # $ % ^ & * ( ) { } [ ] \\ | / ; : - _ = + > < \" ' "; // List of special characters to check

    for (int i = 0; i < length; ++i) {
        if (strchr(specialChars, name[i]) != NULL) {
            return 1; // Special character found
        }
    }

    return 0; // No special characters found
}
void addExtension(char* result, const char* name) {
    strcpy(result, "accounts/"); // Add "accounts/" at the beginning
    strcat(result, name);       // Concatenate the user name
    strcat(result, ".txt");     // Add ".txt" at the end
}