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

void placeItem(appleStruct* apple,snakeStruct* snake,int snakeLength) {
	int overlap=0;
	do {
		apple->x = rand() % WIDTH;
		apple->y = rand() % HEIGHT;
		// 檢查新生成的蘋果位置是否和蛇的位置重疊
		int overlap = 0;
		for (int i = 0; i < snakeLength; i++) {
			if (snake[i].x == apple->x && snake[i].y == apple->y) {
				overlap = 1;
				break;
			}
		}
	} while (overlap);
	/*apple->x = rand() % WIDTH;
	apple->y = rand() % HEIGHT;*/
}

void move(snakeStruct* snake, char* direction, int* snakeLength, int* ateApple, appleStruct* apple) {
	int key = 0;

	// 保存蛇头的原始位置
	int prevX = snake[0].x;
	int prevY = snake[0].y;

	// 根据方向移动蛇头
	if (_kbhit()) {
		int newDirection = _getch();
		if ((newDirection == 'w' && *direction != 's') ||
			(newDirection == 's' && *direction != 'w') ||
			(newDirection == 'a' && *direction != 'd') ||
			(newDirection == 'd' && *direction != 'a')) {
			key = 1;
			*direction = newDirection;
		}
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

	// 移动蛇的身体
	for (int i = *snakeLength - 1; i > 0; i--) {
		if (snake[i].x != -1 && snake[i].y != -1) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
	}

	// 將蛇的頭和第一節連接起來
	snake[1].x = prevX;
	snake[1].y = prevY;

	// 如果蛇的长度大于1，且按键，且吃到了苹果，则增加蛇的长度
	if (*snakeLength > 0 && key && checkEat(&snake[0], apple)) {
		printf("Key is pressed, snake ate the apple!\n");
		snake[*snakeLength].x = -1;
		snake[*snakeLength].y = -1;

		// 增加蛇的长度
		(*snakeLength)++;
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
	return(snake[0].x == apple->x && snake[0].y == apple->y);
}

int checkBoundary(snakeStruct* snake) {
	return(snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT);
}

void GamePlay(const char* filepath) {
	snakeStruct snake[100];
	appleStruct apple;
	for (int i = 0; i < 100; i++) {
		snake[i].x = -1;  // 使用一個特殊的值，表示這個位置不包含蛇的一部分
		snake[i].y = -1;
	}

	snake->x = WIDTH / 2;
	snake->y = HEIGHT / 2;

	int snakeLength = 1;
	int ateApple = 0;
	placeItem(&apple, snake, snakeLength);

	while (1) {
		setDisplay(snake, &apple, snakeLength);
		move(snake, &direction, &snakeLength, &ateApple, &apple);

		if (checkEat(snake,&apple)) {
			placeItem(&apple, snake, snakeLength);
			ateApple = 0;
		}
		if (checkBoundary(snake)) {
			printf("Gameover");
			exit(0);
		}
		Sleep(1000);
	}
}

