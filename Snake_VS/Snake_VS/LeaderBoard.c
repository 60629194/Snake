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
#include<time.h>


#define FILENAME "leaderBoard.txt"

extern int scdata;

// �ק�ƾڵ��c�A�W�[����M�b������
struct LeaderboardEntry {
    int score;
    char date[20];  // ����r�Ŧ�A�o�̨ϥΩT�w����
    char account[20];  // �b�������r�Ŧ�A�o�̨ϥΩT�w����
};

static int compare(const void* a, const void* b) {
    // �ק�����ơA����������ơA�M�������
    const struct LeaderboardEntry* entryA = (const struct LeaderboardEntry*)a;
    const struct LeaderboardEntry* entryB = (const struct LeaderboardEntry*)b;

    if (entryA->score != entryB->score) {
        return entryB->score - entryA->score;
    }

    // �p�G���ƬۦP�A������
    return strcmp(entryB->date, entryA->date);
}

void LeaderboardWrite(const char* filepath, int score, const char* account) {
    // �����e����M�ɶ�
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d=%H:%M:%S", tm_info);


    // �Ы�LeaderboardEntry���c
    struct LeaderboardEntry entry;
    entry.score = score;
    strcpy(entry.date, dateStr);
    strcpy(entry.account, account);

    // Ū���ª��Ʀ�]�ƾ�
    struct LeaderboardEntry data[6];
    FILE* file = fopen(filepath, "r");


    if (file != NULL) {
        for (int i = 0; i < 5; ++i) {
            if (fscanf(file, "%d\t%s\t%s", &data[i].score, data[i].date, data[i].account) != 3) {
                // Ū�����ѡA�i�H�i����~�B�z
                printf("�L�kŪ���Ʀ�]�ƾڡC�渹�G%d\n", i + 1);
                //printf("�L�kŪ���Ʀ�]�ƾڡC\n");
                Sleep(3000);
                fclose(file);
                return;
            }
        }
        fclose(file);
    }
    
    // �N�s�ƾڴ��J�Ʋ�
    data[5] = entry;

    // �N�Ʋի��Ѥj��p�Ƨ�
    qsort(data, 6 , sizeof(struct LeaderboardEntry), compare);


    // �}�Ҥ奻���H�g�J�Ҧ�
    FILE* writefile = fopen(filepath, "w");

    // �ˬd�ɮ׬O�_���\�}��
    if (writefile == NULL) {
        printf("�L�k�}���ɮסC\n");
        Sleep(3000);
        return;
    }

    // �g�J�Ƨǫ᪺�ƾڨ�奻���
    for (int i = 0; i < 5 ; ++i) {
        fprintf(writefile, "%d\t%s\t%s", data[i].score, data[i].date, data[i].account);
        if (i < 4) {
            fprintf(writefile, "\n");
        }
    }

    // �����ɮ�
    fclose(writefile);
}


/*extern int scdata;

static int compare(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void LeaderboardWrite(const char* filepath) {

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
}
*/

void LeaderBoard(const char* filepath) {

    system("cls");
    int data[5];

    // �}�Ҥ奻���HŪ���Ҧ��A�Ω���ܱƦ�]
    FILE* file = fopen(filepath, "r");

    // �ˬd�ɮ׬O�_���\�}��
    if (file == NULL) {
        printf("�L�k�}���ɮסC\n");
        return;
    }
    printf("LeaderBoard\n");
    printf("rank    score       playing time        player\n");
    // �M���ɮת��C�@�����ܦb�׺ݾ��W
    char line[100];
    int rank = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        // �p�G�O�ťզ�A���L
        if (strlen(line) <= 1) {
            continue;
        }

        printf("------------------------------------------------\n");
        printf("%d.     %s", rank++, line); // %-5d is used for left-aligned numbering
    }

    //while (fgets(line, sizeof(line), file) != NULL) {
    //    printf("------------------------------------------------\n");
    //    printf("%d.     %s", rank++, line);// %-5d is used for left-aligned numbering
    //}
    printf("\n------------------------------------------------");
    fclose(file);
    char input;
    printf("\nPress 'q' to quit or any other key to continue: ");
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

