#define _CRT_SECURE_NO_WARNINGS
#include"LeaderBoard.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include"GamePlay.h"
#include<string.h>



extern int scdata;

int compare(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void LeaderBoard(const char* filepath) {

    system("cls");
    int data[5];

    // Ū���ª��Ʀ�]�ƾ�
    FILE* file = fopen(filepath, "r");
    if (file != NULL) {
        for (int i = 0; i < 5; ++i) {
            if (fscanf(file, "%d", &data[i]) != 1) {
                // Ū�����ѡA�i�H�i����~�B�z
                printf("�L�kŪ���Ʀ�]�ƾڡC\n");
                fclose(file);
                return;
            }
        }
        fclose(file);
    }
    else {
        // �p�G��󤣦s�b�A��l�Ƽƾ�
        for (int i = 0; i < 5; ++i) {
            data[i] = 0;
        }
    }

    // �N�s�ƾڴ��J�Ʋ�
    data[4] = scdata;

    // �N�Ʋի��Ѥj��p�Ƨ�
    qsort(data, 5, sizeof(int), compare);

    // �}�Ҥ奻���H�g�J�Ҧ�
    FILE* writefile = fopen(filepath, "w");

    // �ˬd�ɮ׬O�_���\�}��
    if (writefile == NULL) {
        printf("�L�k�}���ɮסC\n");
        return;
    }

    // �g�J�Ƨǫ᪺�ƾڨ�奻���
    for (int i = 0; i < 5; ++i) {
        fprintf(writefile, "%d\n", data[i]);
    }

    // �����ɮ�
    fclose(writefile);

    // �}�Ҥ奻���HŪ���Ҧ��A�Ω���ܱƦ�]
    file = fopen(filepath, "r");

    // �ˬd�ɮ׬O�_���\�}��
    if (file == NULL) {
        printf("�L�k�}���ɮסC\n");
        return;
    }
    printf("LeaderBoard\n");
    // �M���ɮת��C�@�����ܦb�׺ݾ��W
    char line[100];
    int rank = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%d.  %s", rank++, line); // %-5d is used for left-aligned numbering
    }

    fclose(file);
    char input;
    printf("Press 'q' to quit or any other key to continue: ");
    while(1){
        if (_kbhit()) {
            input = _getch();
            if (input == 'q' || input == 'Q') {
                system("cls");
                break;
            }
        }
    }
    return;
}
