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

extern int scdata;

// make a struct with three variable
struct LeaderboardEntry {
    int score;
    char date[20];
    char account[20];  
};

static int compare(const void* a, const void* b) {
    //compare score
    const struct LeaderboardEntry* entryA = (const struct LeaderboardEntry*)a;
    const struct LeaderboardEntry* entryB = (const struct LeaderboardEntry*)b;

    if (entryA->score != entryB->score) {
        return entryB->score - entryA->score;
    }

    // the same score->date
    return strcmp(entryB->date, entryA->date);
}

void LeaderboardWrite(const char* filepath, int score, const char* account) {
    // get the recent time
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d=%H:%M:%S", tm_info);


    // create LeaderboardEntry struct
    struct LeaderboardEntry entry;
    entry.score = score;
    strcpy(entry.date, dateStr);
    strcpy(entry.account, account);

    // read old data
    struct LeaderboardEntry data[6];
    FILE* file = fopen(filepath, "r");


    if (file != NULL) {
        for (int i = 0; i < 5; ++i) {
            if (fscanf(file, "%d\t%s\t%s", &data[i].score, data[i].date, data[i].account) != 3) {
                // read fail
                printf("can't read the txt line¡G%d\n", i + 1);
                Sleep(3000);
                fclose(file);
                return;
            }
        }
        fclose(file);
    }
    
    // insert new data
    data[5] = entry;

    // sort
    qsort(data, 6 , sizeof(struct LeaderboardEntry), compare);


    // write file
    FILE* writefile = fopen(filepath, "w");

    // check correctly open
    if (writefile == NULL) {
        printf("can't open a file\n");
        Sleep(3000);
        return;
    }

    // write the data in txt
    for (int i = 0; i < 5 ; ++i) {
        fprintf(writefile, "%d\t%s\t%s", data[i].score, data[i].date, data[i].account);
        if (i < 4) {
            fprintf(writefile, "\n");
        }
    }

    
    fclose(writefile);
}


void LeaderBoard(const char* filepath) {

    system("cls");
    int data[5];

    // read/show the file
    FILE* file = fopen(filepath, "r");

    // check correctly open
    if (file == NULL) {
        printf("can't open the file\n");
        return;
    }
    printf("LeaderBoard\n");
    printf("rank    score       playing time        player\n");
    
    char line[100];
    int rank = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        // if there is an empty line, skipped
        if (strlen(line) <= 1) {
            continue;
        }
        int score;
        sscanf(line, "%d", &score);

        // ¦pªG¤À??0¡A?¸õ?
        if (score == 0) {
            continue;
        }

        printf("------------------------------------------------\n");
        printf("%d.     %s", rank++, line); 
    }

    printf("\n------------------------------------------------");
    fclose(file);
    char input;
    printf("\nPress 'q' to quit: ");
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

