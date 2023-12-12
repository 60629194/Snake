#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include "global.h"
#include "sha256.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char skins[SKINNUMBER + 1] = { '%', '!', '#', '$', '&', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '~', '@', '?', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ':', '+', ';', '\0' };
const int  skinValue[SKINNUMBER + 1] = { 5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5 , '\0' };
bool SFX = true;
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
void removeNewLine(char* str) {
	size_t length = strlen(str);
	if (length > 0 && str[length - 1] == '\n') {
		str[length - 1] = '\0'; // Replace newline with null terminator
	}
}
void colorPrint(const char* text, int red, int green, int blue) {
	printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m", red, green, blue, text);
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
void updateSha256(char* accountPath) {

	char* content = NULL;
	char* temp;

	for (int i = 1; i <= 4; ++i) {
		temp = readObject(accountPath, i);
		if (temp != NULL && strcmp(temp, "File not found.") != 0 && strcmp(temp, "Line number exceeds file length.") != 0) {
			if (content == NULL) {
				content = strdup(temp);
			}
			else {
				content = realloc(content, strlen(content) + strlen(temp) + 1);
				strcat(content, temp);
			}
			free(temp);
		}
		else {
			system("cls");
			printf("sth went wrong");
			exit(1);
		}
	}
	char* sha = sha256(content);
	free(content);
	writeObject(accountPath, 5, sha);
	return;
}
void checksha(char* accountPath) {
	char* shaInFile = readObject(accountPath, 5);
	char* content = NULL;
	char* temp;

	for (int i = 1; i <= 4; ++i) {
		temp = readObject(accountPath, i);
		if (temp != NULL && strcmp(temp, "File not found.") != 0 && strcmp(temp, "Line number exceeds file length.") != 0) {
			if (content == NULL) {
				content = strdup(temp);
			}
			else {
				content = realloc(content, strlen(content) + strlen(temp) + 1);
				strcat(content, temp);
			}
			free(temp);
		}
		else {
			system("cls");
			printf("sth went wrong");
			exit(1);
		}
	}
	char* shaNow = sha256(content);
	removeNewLine(shaInFile);
	free(content);
	if (strcmp(shaInFile, shaNow) != 0) {
		printf("you are cheating\n");
		printf("%s\n", shaInFile);
		printf("%s", shaNow);
		exit(0);
	}
	else {
		return;
	}

}