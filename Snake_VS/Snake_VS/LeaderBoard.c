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

// 修改數據結構，增加日期和帳號身分
struct LeaderboardEntry {
    int score;
    char date[20];  // 日期字符串，這裡使用固定長度
    char account[20];  // 帳號身分字符串，這裡使用固定長度
};

static int compare(const void* a, const void* b) {
    // 修改比較函數，首先比較分數，然後比較日期
    const struct LeaderboardEntry* entryA = (const struct LeaderboardEntry*)a;
    const struct LeaderboardEntry* entryB = (const struct LeaderboardEntry*)b;

    if (entryA->score != entryB->score) {
        return entryB->score - entryA->score;
    }

    // 如果分數相同，比較日期
    return strcmp(entryB->date, entryA->date);
}

void LeaderboardWrite(const char* filepath, int score, const char* account) {
    // 獲取當前日期和時間
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d=%H:%M:%S", tm_info);


    // 創建LeaderboardEntry結構
    struct LeaderboardEntry entry;
    entry.score = score;
    strcpy(entry.date, dateStr);
    strcpy(entry.account, account);

    // 讀取舊的排行榜數據
    struct LeaderboardEntry data[6];
    FILE* file = fopen(filepath, "r");


    if (file != NULL) {
        for (int i = 0; i < 5; ++i) {
            if (fscanf(file, "%d\t%s\t%s", &data[i].score, data[i].date, data[i].account) != 3) {
                // 讀取失敗，可以進行錯誤處理
                printf("無法讀取排行榜數據。行號：%d\n", i + 1);
                //printf("無法讀取排行榜數據。\n");
                Sleep(3000);
                fclose(file);
                return;
            }
        }
        fclose(file);
    }
    
    // 將新數據插入數組
    data[5] = entry;

    // 將數組按由大到小排序
    qsort(data, 6 , sizeof(struct LeaderboardEntry), compare);


    // 開啟文本文件以寫入模式
    FILE* writefile = fopen(filepath, "w");

    // 檢查檔案是否成功開啟
    if (writefile == NULL) {
        printf("無法開啟檔案。\n");
        Sleep(3000);
        return;
    }

    // 寫入排序後的數據到文本文件
    for (int i = 0; i < 5 ; ++i) {
        fprintf(writefile, "%d\t%s\t%s", data[i].score, data[i].date, data[i].account);
        if (i < 4) {
            fprintf(writefile, "\n");
        }
    }

    // 關閉檔案
    fclose(writefile);
}


/*extern int scdata;

static int compare(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void LeaderboardWrite(const char* filepath) {

    system("cls");
    int data[5];

    // 讀取舊的排行榜數據
    FILE* file = fopen(filepath, "r");
    if (file != NULL) {
        for (int i = 0; i < 5; ++i) {
            if (fscanf(file, "%d", &data[i]) != 1) {
                // 讀取失敗，可以進行錯誤處理
                printf("無法讀取排行榜數據。\n");
                fclose(file);
                return;
            }
        }
        fclose(file);
    }
    else {
        // 如果文件不存在，初始化數據
        for (int i = 0; i < 5; ++i) {
            data[i] = 0;
        }
    }

    // 將新數據插入數組
    data[4] = scdata;

    // 將數組按由大到小排序
    qsort(data, 5, sizeof(int), compare);

    // 開啟文本文件以寫入模式
    FILE* writefile = fopen(filepath, "w");

    // 檢查檔案是否成功開啟
    if (writefile == NULL) {
        printf("無法開啟檔案。\n");
        return;
    }

    // 寫入排序後的數據到文本文件
    for (int i = 0; i < 5; ++i) {
        fprintf(writefile, "%d\n", data[i]);
    }

    // 關閉檔案
    fclose(writefile);
}
*/

void LeaderBoard(const char* filepath) {

    system("cls");
    int data[5];

    // 開啟文本文件以讀取模式，用於顯示排行榜
    FILE* file = fopen(filepath, "r");

    // 檢查檔案是否成功開啟
    if (file == NULL) {
        printf("無法開啟檔案。\n");
        return;
    }
    printf("LeaderBoard\n");
    printf("rank    score       playing time        player\n");
    // 遍歷檔案的每一行並顯示在終端機上
    char line[100];
    int rank = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        // 如果是空白行，跳過
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

