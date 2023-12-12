#define _CRT_SECURE_NO_WARNINGS
#include"Store.h"
#include"global.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>


static bool* convertLineToBoolArray(const char* line);
static char* createlockedSkins(char* characters, bool* skin, int charCount);
static int chooseSkin(char* lockedSkin,char *filepath, int* skinValues);
static char* createlockedSkinValue(int* values, bool* skin, int charCount);

void Store(const char* filepath) {
START:
	system("cls");
	char* lineOfLockedSkin = readObject(filepath, 3);
	bool* Bskin = convertLineToBoolArray(lineOfLockedSkin);
	char* lockedSkins = createlockedSkins(skins, Bskin, SKINNUMBER);
	int* lockedSkinValues = createlockedSkinValue(skinValue, Bskin, SKINNUMBER);
	int skinToBuy = chooseSkin(lockedSkins,filepath,lockedSkinValues);
	if (skinToBuy < 0) {
		// player chose return home
		checksha(filepath);
		return;
	}
	int money = atoi(readObject(filepath, 2));
	//buy the skin!
	if (money < lockedSkinValues[skinToBuy]) {
		PlaySound(TEXT("accessDenied.wav"), NULL, SND_FILENAME | SND_ASYNC);
		system("cls");
		printf("You don't have enough money");
		Sleep(3000);
		goto START;
	}
	else {
		PlaySound(TEXT("accessGranted.wav"), NULL, SND_FILENAME | SND_ASYNC);
		money = money - lockedSkinValues[skinToBuy];
		char skinAfterBuy[100]="1";
		for (int i = 2;i < 96;i++) {
			if (lineOfLockedSkin[i] == '1') {
				strcat(skinAfterBuy, " 1");
			}
			else if (skins[i/2] == lockedSkins[skinToBuy]) {
				strcat(skinAfterBuy, " 1");
			}
			else {
				strcat(skinAfterBuy, " 0");
			}
			i++;
		}
		checksha(filepath);
		writeObject(filepath, 3, skinAfterBuy);
		char Cmoney[2000];
		_itoa(money, Cmoney, 10);
		writeObject(filepath, 2, Cmoney);
		updateSha256(filepath);
		goto START;
	}
    return;
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
char* createlockedSkins(char* characters, bool* skin, int charCount) {
	int lockedCount = 0;

	// Allocate memory for the unlockedSkins array dynamically
	char* lockedSkins = (char*)malloc((SKINNUMBER + 1) * sizeof(char));
	if (lockedSkins == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}

	for (int i = 0; i < SKINNUMBER; i++) {
		if (!skin[i]) {
			lockedSkins[lockedCount++] = characters[i];
		}
	}

	lockedSkins[lockedCount] = '\0'; // Null-terminate the string

	return lockedSkins;
}
int chooseSkin(char* lockedSkin,char * filepath,int *skinValues) {
	int choice = 0;
	char key = 72;
	int skinCount = calculateSkinCount(lockedSkin);

	do {
		system("cls"); // Clears the console screen (Windows-specific)
		int money = atoi(readObject(filepath, 2));
		printf("Choose a skin to buy :                 you have %d coins\n",money);
		for (int i = 0; i < skinCount; i++) {
			printf("%c %s %d\n", lockedSkin[i], (i == choice) ? "<" : " ",skinValues[i]);
		}
		printf("back to home page %s\n", (choice == skinCount) ? "<" : "");

		// Non-blocking key detection for arrow keys
		if (_kbhit()) {
			key = _getch();


			switch (key) {
			case 72: // Up arrow key
				if (choice == 0) {
					PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					choice = skinCount+1;
				}
				if (choice > 0) {
					PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					choice--;
				}
				break;
			case 80: // Down arrow key
				if (choice < skinCount+1) {
					PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					choice++;
				}
				if (choice == skinCount+1) {
					PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					choice=0;
				}
				break;
			}


		}
		Sleep(100);
	} while (key != 13); // Continue until Enter key is pressed (13 is Enter's ASCII code)

	if (choice == skinCount) {
		return -1;
	}

	return choice;
}
char* createlockedSkinValue(int* values, bool* skin, int charCount) {
	int lockedCount = 0;

	// Allocate memory for the unlockedSkins array dynamically
	int* lockedSkins = (int*)malloc((SKINNUMBER + 1) * sizeof(int));
	if (lockedSkins == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}

	for (int i = 0; i < SKINNUMBER; i++) {
		if (!skin[i]) {
			lockedSkins[lockedCount++] = values[i];
		}
	}

	lockedSkins[lockedCount] = '\0'; // Null-terminate the string

	return lockedSkins;
}