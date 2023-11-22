#define _CRT_SECURE_NO_WARNINGS
#include"Store.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#define SKIN_NUMBER 48

static char* readObject(const char* filepath, int lineNumber);
static bool* convertLineToBoolArray(const char* line);
static char* createlockedSkins(char* characters, bool* skin, int charCount);
static int chooseSkin(char* lockedSkin,char *filepath, int* skinValues);
static char* createlockedSkinValue(int* values, bool* skin, int charCount);
static void writeObject(const char* filepath, int lineNumber, const char* content);
static void writeObjectForInt(const char* filepath, int lineNumber, int content);


void Store(const char* filepath) {
	char characters[SKIN_NUMBER + 1] = {'%', '!', '#', '$', '&', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '~', '@', '?', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ':', '+', ';', '\0'};
	int skinValue[SKIN_NUMBER+1] = {       5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5 , '\0'};
START:
	system("cls");
	char* lineOfLockedSkin = readObject(filepath, 3);
	bool* Bskin = convertLineToBoolArray(lineOfLockedSkin);
	char* lockedSkins = createlockedSkins(characters, Bskin, SKIN_NUMBER);
	int* lockedSkinValues = createlockedSkinValue(skinValue, Bskin, SKIN_NUMBER);
	int skinToBuy = chooseSkin(lockedSkins,filepath,lockedSkinValues);
	if (skinToBuy < 0) {
		// player chose return home
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
			else if (characters[i/2] == lockedSkins[skinToBuy]) {
				strcat(skinAfterBuy, " 1");
			}
			else {
				strcat(skinAfterBuy, " 0");
			}
			i++;
		}
		writeObject(filepath, 3, skinAfterBuy);
		char Cmoney[2000];
		_itoa(money, Cmoney, 10);
		writeObject(filepath, 2, Cmoney);
		goto START;
	}
    return;
}

char* readObject(const char* filepath, int lineNumber) {
	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		return "File not found.";
	}

	char* line = NULL;
	size_t len = 0;
	int currentLine = 0;
	int maxLineLength = 1000; // Adjust as needed

	while (currentLine < lineNumber && !feof(file)) {
		line = malloc(maxLineLength * sizeof(char));
		if (fgets(line, maxLineLength, file) != NULL) {
			currentLine++;
		}
		else {
			free(line);
			line = NULL;
			break; // Break loop on EOF or error
		}
	}

	fclose(file);

	if (line == NULL) {
		return "Line number exceeds file length.";
	}

	return line;
}
bool* convertLineToBoolArray(const char* line) {
	// Allocate memory for the boolean array
	bool* boolArray = malloc(SKIN_NUMBER * sizeof(bool));

	if (boolArray == NULL) {
		return NULL;
	}

	// Parse the line and populate the boolean array
	char* token = strtok(line, " "); // Split by space
	int index = 0;
	while (token != NULL && index < SKIN_NUMBER) {
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
	char* lockedSkins = (char*)malloc((SKIN_NUMBER + 1) * sizeof(char));
	if (lockedSkins == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}

	for (int i = 0; i < SKIN_NUMBER; i++) {
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
	int* lockedSkins = (int*)malloc((SKIN_NUMBER + 1) * sizeof(int));
	if (lockedSkins == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}

	for (int i = 0; i < SKIN_NUMBER; i++) {
		if (!skin[i]) {
			lockedSkins[lockedCount++] = values[i];
		}
	}

	lockedSkins[lockedCount] = '\0'; // Null-terminate the string

	return lockedSkins;
}
void writeObject(const char* filepath, int lineNumber, const char* content) {
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
			fprintf(tempFile, "%s\n", content);
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
void writeObjectForInt(const char* filepath, int lineNumber, int content) {
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
			fprintf(tempFile, "%d\n", content);
		}
		else {
			fprintf(tempFile, "%d", buffer);
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