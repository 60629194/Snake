#include "GamePlay.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

int scdata;

int WIDTH = 20;
int HEIGHT = 10;
char direction = 'd';

typedef struct {
	int x, y;
}snakeStruct;

typedef struct {
	int x, y;
}appleStruct;

void placeItem(appleStruct* apple) {
	apple->x = rand() % WIDTH;
	apple->y = rand() % HEIGHT;
}

void move(snakeStruct* snake, char* direction, int* snakeLength, int* ateApple, appleStruct* apple) {
	// 移動身體
	int key = 0;

	for (int i = *snakeLength; i > 0; i--) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	// 根據方向移動頭部
	if (_kbhit()) {
		key = 1;
		*direction = _getch();
	}

	switch (*direction) {
	case 'w':
		snake[0].y--;
		break;
	case 's':
		snake[0].y++;
		break;
	case 'a':
		snake[0].x--;
		break;
	case 'd':
		snake[0].x++;
		break;
	}

	// 如果蛇的長度大於1，則增加身體長度
	if (*snakeLength > 1 && key && checkEat(&snake[0], apple)) {
		*ateApple = 1;
	}
}

void setDisplay(snakeStruct* snake, appleStruct* apple, int snakeLength) {
	system("cls"); // 清空控制台屏幕

	// 上
	for (int i = 0; i < WIDTH/2+2; i++) {
		printf("■");
	}
	printf("\n");

	
	for (int y = 0; y < HEIGHT; y++) {
		printf("■"); // 左
		for (int x = 0; x < WIDTH; x++) {
			int isSnakeBody = 0;

			for (int i = 0; i < snakeLength; i++) {
				if (snake[i].x == x && snake[i].y == y) {
					isSnakeBody = 1;
					break;
				}
			}

			if (isSnakeBody) {
				printf("O"); // 蛇
			}
			else if (x == apple->x && y == apple->y) {
				printf("A"); // apple
			}
			else {
				printf(" ");
			}
		}
		printf("■"); // 右
		printf("\n");
	}

	// 下
	for (int i = 0; i < WIDTH/2+2; i++) {
		printf("■");
	}
	printf("\n");
}

int checkEat(snakeStruct* snake, appleStruct* apple) {
	return(snake->x == apple->x && snake->y == apple->y);
}

int checkBoundary(snakeStruct* snake) {
	return(snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT);
}


void GamePlay(const char* filepath) {
	snakeStruct snake[100];
	appleStruct apple;

	snake->x = WIDTH / 2;
	snake->y = HEIGHT / 2;

	int snakeLength = 1;
	int ateApple = 0;
	placeItem(&apple);

	while (1) {
		setDisplay(snake, &apple, snakeLength);
		move(snake, &direction, &snakeLength, &ateApple, &apple);

		if (ateApple) {
			placeItem(&apple);
			ateApple = 0;
			//(*snakeLength)++;
		}
		if (checkBoundary(&snake[0])) {
			printf("Gameover");
			exit(0);
		}
		Sleep(1000);
	}
}

