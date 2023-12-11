#pragma once
#ifndef GLOBAL_H_
#define GLOBAL_H_
#include<stdbool.h>
#define SKINNUMBER 48
extern const char skins[SKINNUMBER+1] ;
extern const int  skinValue[SKINNUMBER+1];
extern bool SFX;

char* readObject(const char* filepath, int lineNumber);
void writeObject(const char* filepath, int lineNumber, const char* content);
void removeNewLine(char* str);
void colorPrint(const char* text, int red, int green, int blue);
char* sha256(const char* input);
void updateSha256(char* accountPath);
void checksha(char* accountPath);
#endif 