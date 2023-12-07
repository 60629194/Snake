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
#include"sha256.h"

#define MAX_MENU_ITEMS 100
#define MAX_FILENAME_LENGTH 100
#define SKIN_NUMBER 48 // currently 48 skins now

void writeObjectForChar(const char* filepath, int lineNumber, const char content);
char* sha256(const char* input);
long int findSize(char file_name[]);
void displayMenu(char menuItems[][MAX_FILENAME_LENGTH], int itemCount, int choice);
char* combineStrings(const char* str1, const char* str2);
void colorPrint(const char* text, int red, int green, int blue);
void writeObject(const char* filepath, int lineNumber, const char* content);
char* readObject(const char* filepath, int lineNumber);
void TrimFilePath(char* filepath);
char* createUnlockedSkins(char* characters, bool* skin, int charCount);
char chooseSkin(char* skin);
int calculateSkinCount(char* skin);
bool* convertLineToBoolArray(const char* line);
void cls() {
	system("cls");
}

int main() {
	//ONLY FOR TESTING
	while (1) {
	GamePlay("accounts/jim.txt");
	}
	//ONLY FOR TESTING
	PlaySound(TEXT("gameStart.wav"), NULL, SND_FILENAME | SND_ASYNC);
	int red, green, blue;
	for (red = 0, green = 0, blue = 0;red < 256 && green < 256 && blue < 256;red++, green++, blue++) {
		colorPrint(" $$$$$                      $$              \n$$   $$            $$$$$    $$  $$    $$$$  \n $$$      $$$$$        $$   $$ $$    $$  $$ \n   $$$    $$  $$    $$$$$   $$$$     $$$$$$ \n$$   $$   $$  $$   $$  $$   $$ $$    $$     \n $$$$$    $$  $$    $$$$$   $$  $$    $$$$$ \n", red, green, blue);
		Sleep(8);
		cls();
		if (red % 2 == 1) {
			green++;
		}
	}
	char characters[SKIN_NUMBER + 1] = {
		'%', '!', '#', '$', '&', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '~', '@',
		'?', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ':', '+', ';', '\0'
	};
START:

	system("checkAccount.bat");

	int choice = 0;
	int key = 10;
	char account[100];
	char accountPath[100];
	long int accountSize = findSize("log\\accountDATA.txt");


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
			writeObject(accountPath, 4, "％");
		}
		char show[100];
		cls();
		PlaySound(TEXT("logInSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcpy(show, "you've logged in as ");
		strncat(show, account, sizeof(show) - strlen(show) - 1);
		colorPrint(show, 254, 254, 254);
		Sleep(500);
		cls();
		for (red = 254, green = 254, blue = 254;red > 0 && green > 0 && blue > 0;red--, green--, blue--) {
			colorPrint(show, red, green, blue);
			Sleep(4);
			if (red < 200) {
				red--;
				green--;
				blue--;
			}
			if (red < 100) {
				red--;
				green--;
				blue--;
			}
			cls();
		}

	}
	else if (choice == itemCount) {
		cls();
		createAccount();
		goto START;
	}
	else if (choice == itemCount + 1) {
		cls();
		PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
		for (red = 254, green = 254, blue = 254;red > 0 && green > 0 && blue > 0;red--, green--, blue--) {
			colorPrint("Have a good day!", red, green, blue);
			Sleep(4);
			cls();
			if (red < 200) {
				red--;
				green--;
				blue--;
			}
			if (red < 100) {
				red--;
				green--;
				blue--;
			}
		}
		exit(0);
	}

	char* line = readObject(accountPath, 3);
	bool* skin = convertLineToBoolArray(line);
	
	//free(unlockedSkins);
	choice = 0;
	key = 10;

	while (1) { // Loop to keep displaying the menu until the user chooses to exit
		do {
			
			cls(); // Clears the console screen (Windows-specific)

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
							PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
							choice--;
						}
						break;
					case 80: // Down arrow key
						if (choice < 5) {
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
			cls();
			PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			GamePlay(accountPath);
			choice = 0;
			key = 10;
			break;
		case 1:
			cls();
			char* unlockedSkins = createUnlockedSkins(characters, skin, SKIN_NUMBER);
			char skinNow;
			skinNow = chooseSkin(unlockedSkins);
			writeObjectForChar(accountPath, 4, skinNow);
			choice = 0;
			key = 10;
			break;
		case 2:
			PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			Store(accountPath);
			choice = 0;
			key = 10;
			break;
		case 3:
			PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			LeaderBoard(accountPath);
			choice = 0;
			key = 10;
			break;
		case 4:
			PlaySound(TEXT("enterSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			int temp;
			temp = Settings(accountPath);
			if (temp == 1) {
				goto START;
			}
			choice = 0;
			key = 10;
			break;
		case 5:
			cls();
			PlaySound(TEXT("exitSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
			for (red = 254, green = 254, blue = 254;red > 0 && green > 0 && blue > 0;red--, green--, blue--) {
				colorPrint("Have a good day!", red, green, blue);
				Sleep(4);
				cls();
				if (red < 200) {
					red--;
					green--;
					blue--;
				}
				if (red < 100) {
					red--;
					green--;
					blue--;
				}
			}
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
	cls(); // Clears the console screen (Windows-specific)
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
void colorPrint(const char* text, int red, int green, int blue) {
	if (red <= 0 || red >= 255 || green <= 0 || green >= 255 || blue <= 0 || blue >= 255) {
		//printf("Invalid color values. Please use values between 0 and 255.\n");
		return;
	}

	printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m\n", red, green, blue, text);
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
char* createUnlockedSkins(char* characters, bool* skin, int charCount) {
	int unlockedCount = 0;

	// Allocate memory for the unlockedSkins array dynamically
	char* unlockedSkins = (char*)malloc((SKIN_NUMBER + 1) * sizeof(char));
	if (unlockedSkins == NULL) {
		printf("Memory allocation failed.\n");
		return NULL;
	}

	for (int i = 0; i < SKIN_NUMBER; i++) {
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
	/*printf("%s\n", unlockedSkin);
	printf("%d", skinCount);
	exit(0);*/

	do {
		system("cls"); // Clears the console screen (Windows-specific)

		printf("Choose a skin:\n");
		for (int i = 0; i < skinCount; i++) {
			printf("%c %s\n", unlockedSkin[i], (i == choice) ? "<" : " ");
		}

		// Non-blocking key detection for arrow keys
		if (_kbhit()) {
			key = _getch();
			

				switch (key) {
				case 72: // Up arrow key
					if (choice > 0) {
						PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
						choice--;
					}
					break;
				case 80: // Down arrow key
					if (choice < skinCount - 1) {
						PlaySound(TEXT("navigateSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
char* sha256(const char* input) {
	BYTE hash[SHA256_BLOCK_SIZE]; // To store the hash output

	SHA256_CTX ctx;
	sha256_init(&ctx); // Initialize the SHA256 context
	sha256_update(&ctx, (BYTE*)input, strlen(input)); // Update with input data
	sha256_final(&ctx, hash); // Finalize and get the hash

	// Convert the hash bytes to a string representation
	char* hash_string = (char*)malloc((SHA256_BLOCK_SIZE * 2 + 1) * sizeof(char)); // Each byte represented by 2 characters in hexadecimal + '\0'
	if (hash_string == NULL) {
		// Handle memory allocation error
		return NULL;
	}

	for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
		sprintf(&hash_string[i * 2], "%02x", hash[i]); // Convert byte to two hexadecimal characters
	}
	hash_string[SHA256_BLOCK_SIZE * 2] = '\0'; // Null-terminate the string

	return hash_string;
}