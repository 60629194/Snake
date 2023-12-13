#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "GamePlay.h"
#include "LeaderBoard.h"
#include "Settings.h"
#include "Store.h"
#include "createAccount.h"
#include "sha256.h"
#include "global.h"


#define MAX_MENU_ITEMS 100
#define MAX_FILENAME_LENGTH 100

extern int coinCount;
void writeObjectForChar(const char* filepath, int lineNumber, const char content);
long int findSize(char file_name[]);
void displayMenu(char menuItems[][MAX_FILENAME_LENGTH], int itemCount, int choice);
char* combineStrings(const char* str1, const char* str2);
void TrimFilePath(char* filepath);
char* createUnlockedSkins(char* characters, bool* skin, int charCount);
char chooseSkin(char* skin);
int calculateSkinCount(char* skin);
bool* convertLineToBoolArray(const char* line);
void updateAccountFile(const char* filepath, int coinCount);
void displaySnake();
void login(char* string);
void exitGame(char* string);

int main() {
	PlaySound(TEXT("gameStart.wav"), NULL, SND_FILENAME | SND_ASYNC);
	int red, green, blue;
	displaySnake();

START:

	system("checkAccount.bat");

	int choice = 0;
	int key = 10;
	char account[100];
	char accountPath[100];
	long int accountSize;


	FILE* file = fopen("log\\accountDATA.txt", "r");
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
					if (choice < itemCount + 1) {
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
		strcpy(accountPath, combineStrings("accounts/", account));
		// Remove the ".txt" extension from 'account'
		char* dot = strchr(account, '.');
		if (dot != NULL) {
			*dot = '\0'; // Null-terminate the string at the position of the dot
		}
		//printf("you chosen %s\n", account);
		accountSize = findSize(accountPath);
		//if account is new, initialize the account
		if (accountSize <= 20) {
			writeObject(accountPath, 1, account);
			writeObject(accountPath, 2, "0");
			char initialSkin[100] = "1 0";
			for (int i = 0;i < 46;i++) {
				strcat(initialSkin, " 0");
			}
			writeObject(accountPath, 3, initialSkin);
			writeObject(accountPath, 4, "%");
			updateSha256(accountPath);
		}
		else {
			checksha(accountPath);
		}
		char show[100];
		system("cls");
		PlaySound(TEXT("logInSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcpy(show, "you've logged in as ");
		strncat(show, account, sizeof(show) - strlen(show) - 1);
		login(show);
		checksha(accountPath);
		updateSha256(accountPath);
	}
	else if (choice == itemCount) {
		system("cls");
		PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
		createAccount();
		goto START;
	}
	else if (choice == itemCount + 1) {
		system("cls");
		PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
		exitGame("Have a good day!");
		exit(0);
	}
	
	choice = 0;
	key = 10;

	while (1) { // Loop to keep displaying the menu until the user chooses to exit
		do {
			
			system("cls"); // Clears the console screen (Windows-specific)

			printf("Welcome to Snake, %s\n", account);
			printf("   Play %s\n", (choice == 0) ? "<" : "  ");
			printf("   Skin %s\n", (choice == 1) ? "<" : "  ");
			printf("   Store %s\n", (choice == 2) ? "<" : "  ");
			printf("   Leader Board %s\n", (choice == 3) ? "<" : "  ");
			printf("   Settings %s\n", (choice == 4) ? "<" : "  ");
			printf("   Exit %s\n", (choice == 5) ? "<" : "  ");

			// Non-blocking key detection for arrow keys
			if (_kbhit()) {
				key = _getch();
				if (key == 224) { // Arrow key detected
					key = _getch(); // Get the extended key code
					switch (key) {
					case 72: // Up arrow key
						if (choice > 0) {
							if(SFX)
							{
								PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
							}
							choice--;
						}
						break;
					case 80: // Down arrow key
						if (choice < 5) {
							if(SFX)
							{
								PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
							}
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
			system("cls");
			if(SFX)
			{
				PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			GamePlay(accountPath);
			checksha(accountPath);
			updateAccountFile(accountPath, coinCount);
			updateSha256(accountPath);
			const char* acc = readObject(accountPath, 1);
			LeaderboardWrite("leaderBoard.txt",scdata,acc);
			choice = 0;
			key = 10;
			break;
		case 1:
			system("cls");
			if (SFX)
			{
				PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			char* line = readObject(accountPath, 3);
			bool* Bskin = convertLineToBoolArray(line);
			char* unlockedSkins = createUnlockedSkins(skins, Bskin, SKINNUMBER);
			char skinNow;
			skinNow = chooseSkin(unlockedSkins);
			checksha(accountPath);
			writeObjectForChar(accountPath, 4, skinNow);
			updateSha256(accountPath);
			choice = 0;
			key = 10;
			break;
		case 2:
			if(SFX)
			{
				PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			checksha(accountPath);
			Store(accountPath);
			updateSha256(accountPath);
			choice = 0;
			key = 10;
			break;
		case 3:
			if(SFX)
			{
				PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			LeaderBoard("leaderBoard.txt");
			choice = 0;
			key = 10;
			break;
		case 4:
			if(SFX)
			{
				PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			int temp;
			temp = Settings(accountPath);
			if (temp == 1) {
				goto START;
			}
			choice = 0;
			key = 10;
			break;
		case 5:
			system("cls");
			if(SFX)
			{
				PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			exitGame("Have a good day!");
			exit(0);
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
char* combineStrings(const char* str1, const char* str2) {
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	// Allocate memory for the combined string (+1 for the null terminator)
	char* combined = (char*)malloc((len1 + len2 + 1) * sizeof(char));
	if (combined == NULL) {
		printf("Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	strcpy(combined, str1); // Copy the constant string
	strcat(combined, str2); // Concatenate the non-constant string
	return combined;
}

void TrimFilePath(char* filepath) {
	// Find the last occurrence of "account/"
	char* accountStr = strstr(filepath, "accounts/");
	if (accountStr != NULL) {
		// Move the pointer ahead to exclude "account/"
		accountStr += strlen("accounts/");
		memmove(filepath, accountStr, strlen(accountStr) + 1); // +1 for null terminator
	}
}
bool* convertLineToBoolArray(const char* line) {
	// Allocate memory for the boolean array
	bool* boolArray = malloc(SKINNUMBER * sizeof(bool));

	if (boolArray == NULL) {
		return NULL;
	}

	// Parse the line and populate the boolean array
	char* token = strtok(line, " "); // Split by space
	int index = 0;
	while (token != NULL && index < SKINNUMBER) {
		int value = atoi(token); // Convert string to integer
		boolArray[index] = (value == 1) ? true : false;
		token = strtok(NULL, " ");
		index++;
	}

	return boolArray;
}
char* createUnlockedSkins(char* characters, bool* skin, int charCount) {
	int unlockedCount = 0;

	// Allocate memory for the unlockedSkins array dynamically
	char* unlockedSkins = (char*)malloc((SKINNUMBER + 1) * sizeof(char));
	if (unlockedSkins == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}

	for (int i = 0; i < SKINNUMBER; i++) {
		if (skin[i]) {
			unlockedSkins[unlockedCount++] = characters[i];
		}
	}

	unlockedSkins[unlockedCount] = '\0'; // Null-terminate the string

	return unlockedSkins;
}
char chooseSkin(char* unlockedSkin) {
	int choice = 0;
	char key = 72;
	int skinCount = calculateSkinCount(unlockedSkin);

	do {
		system("cls"); // Clears the console screen (Windows-specific)

		printf("Choose a skin:\n");
		for (int i = 0; i < skinCount; i++) {
			printf("%c %s\n", unlockedSkin[i], (i == choice) ? "<" : " ");
		}

		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case 72: // Up arrow key
				if (choice > 0) {
					if(SFX)
					{
						PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
					choice--;
				}
				break;
			case 80: // Down arrow key
				if (choice < skinCount - 1) {
					if (SFX)
					{
						PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
					choice++;
				}
				break;
			}
		}
		Sleep(100);
	} while (key != 13); // Continue until Enter key is pressed (13 is Enter's ASCII code)

	return unlockedSkin[choice];
}
int calculateSkinCount(char* skin) {
	int count = 0;
	while (skin[count] != NULL) {
		count++;
	}
	return count;
}
void writeObjectForChar(const char* filepath, int lineNumber, const char content) {
	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		printf("File '%s' not found!\n", filepath);
		exit(1);
		return;
	}

	// Create a temporary file
	FILE* tempFile = fopen("accounts/temp.txt", "w");
	if (tempFile == NULL) {
		fclose(file);
		printf("Unable to create a temporary file.\n");
		exit(1);
		return;
	}

	char buffer[1024];
	int lineCount = 0;

	while (fgets(buffer, sizeof(buffer), file)) {
		lineCount++;

		if (lineCount == lineNumber) {
			fprintf(tempFile, "%c\n", content);
		}
		else {
			fprintf(tempFile, "%s", buffer);
		}
	}

	fclose(file);
	fclose(tempFile);
	chdir("accounts");
	char tempfilepath[100];
	strcpy(tempfilepath, filepath);
	TrimFilePath(tempfilepath);
	// Remove the original file
	remove(tempfilepath);

	// Rename the temporary file to the original file name
	rename("temp.txt", tempfilepath);
	chdir("..");
}
void updateAccountFile(const char* filepath, int coinCount) {
	int originalMoney = atoi(readObject(filepath, 2));
	int newMoney = originalMoney + coinCount;
	char sMoney[100];
	itoa(newMoney, sMoney, 10);
	writeObject(filepath, 2, sMoney);
	return;
}
void displaySnake(){
	DWORD initial=GetTickCount();
	DWORD r=1,g=1,b=1;
	DWORD now=GetTickCount();
	while(g<256){
		now=GetTickCount();
		r=(now-initial)/16;
		g=(now-initial)/8;
		b=(now-initial)/16;
		colorPrint(" $$$$$                      $$              \n$$   $$            $$$$$    $$  $$    $$$$  \n $$$      $$$$$        $$   $$ $$    $$  $$ \n   $$$    $$  $$    $$$$$   $$$$     $$$$$$ \n$$   $$   $$  $$   $$  $$   $$ $$    $$     \n $$$$$    $$  $$    $$$$$   $$  $$    $$$$$ \n",r,g,b);
		Sleep(25);
		system("cls");
	}
	colorPrint(" $$$$$                      $$              \n$$   $$            $$$$$    $$  $$    $$$$  \n $$$      $$$$$        $$   $$ $$    $$  $$ \n   $$$    $$  $$    $$$$$   $$$$     $$$$$$ \n$$   $$   $$  $$   $$  $$   $$ $$    $$     \n $$$$$    $$  $$    $$$$$   $$  $$    $$$$$ \n",128,255,128);
	Sleep(1000);
	system("cls");
	return;
}
void login(char* string) {
	int len = strlen(string);
	Sleep(500);
	for (int i = 0; i < len; i++) {
		printf("%c", string[i]);
		fflush(stdout);
		Sleep(100); 
	}
	Sleep(500);
}
void exitGame(char* string) {
	int len = strlen(string);
	for (int i = 0; i < len; i++) {
		printf("%c", string[i]);
		fflush(stdout);
		Sleep(100);
	}
	Sleep(500);
}