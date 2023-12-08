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

    // 開啟文本文件以讀取模式，用於顯示排行榜
    file = fopen(filepath, "r");

    // 檢查檔案是否成功開啟
    if (file == NULL) {
        printf("無法開啟檔案。\n");
        return;
    }
    printf("LeaderBoard\n");
    // 遍歷檔案的每一行並顯示在終端機上
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
