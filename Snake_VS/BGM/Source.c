#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<mmsystem.h>

int main() {
	PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	int dummy=1;
	scanf("%d",dummy);
	return 0;
}