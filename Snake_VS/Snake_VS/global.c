#define _CRT_SECURE_NO_WARNINGS
#include "global.h"
#include<stdio.h>

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