#include "GamePlay.h"
#include <stdio.h>
#include <Windows.h>
#include<mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77

int scdata;

short int WIDTH = 20;
short int HEIGHT = 10;
char direction = 'n';

typedef struct {
	int x, y;
}snakeStruct;

typedef struct {
	int x, y;
}appleStruct;

typedef struct {
	int x, y;
}coinStruct;

static void colorPrint(const char* text, int red, int green, int blue);
static void placeApple(appleStruct* apple, snakeStruct* snake, int snakeLength);
static void placeCoin(coinStruct* coin, snakeStruct* snake, int snakeLength);
static void move(snakeStruct* snake, char* direction, int* snakeLength, int* ateApple, appleStruct* apple);
static void setDisplay(snakeStruct* snake, appleStruct* apple, coinStruct* coin, int snakeLength);
static int checkEatApple(snakeStruct* snake, appleStruct* apple);
static int checkEatCoin(snakeStruct* snake, coinStruct* coin);
static int checkBoundary(snakeStruct* snake);

void GamePlay(const char* filepath) {
	snakeStruct snake[100];
	appleStruct apple;
	coinStruct coin;
	for (int i = 0; i < 100; i++) {
		snake[i].x = -1; 
		snake[i].y = -1;
	}

	snake->x = WIDTH / 2;
	snake->y = HEIGHT / 2;

	int snakeLength = 1;
	int ateApple = 0;
	placeApple(&apple, snake, snakeLength);
	placeCoin(&coin, snake, snakeLength);
	printf("Apple position: (%d, %d)\n", apple.x, apple.y);

	while (1) {
		move(snake, &direction, &snakeLength, &ateApple, &apple);
		setDisplay(snake, &apple, &coin, snakeLength);


		if (checkEatApple(snake,&apple)) {
			placeApple(&apple, snake, snakeLength);
			printf("Apple position: (%d, %d)\n", apple.x, apple.y);
			ateApple = 0;
		}
		if (checkEatCoin(snake, &coin)) {
			placeCoin(&coin, snake, snakeLength);
			printf("Coin position: (%d, %d)\n", coin.x, coin.y);
		}
		if (checkBoundary(snake)) {
			printf("Gameover");
			break;
		}
		Sleep(1000);
	}
}

void colorPrint(const char* text, int red, int green, int blue) {
	printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m", red, green, blue, text);
}
void placeApple(appleStruct* apple, snakeStruct* snake, int snakeLength) {
	bool overlap;
	srand(time(0));
	do {
		apple->x = rand() % WIDTH;
		apple->y = rand() % HEIGHT;
		// 檢查新生成的蘋果位置是否和蛇的位置重疊
		overlap = false;
		for (int i = 0; i < snakeLength; i++) {
			if (snake[i].x == apple->x && snake[i].y == apple->y) {
				overlap = true;
				break;
			}
		}
	} while (overlap);
}
void placeCoin(coinStruct* coin, snakeStruct* snake, int snakeLength) {
	bool overlap;
	srand(time(0));
	do {
		coin->x = rand() % WIDTH;
		coin->y = rand() % HEIGHT;
		// 檢查新生成的蘋果位置是否和蛇的位置重疊
		overlap = false;
		for (int i = 0; i < snakeLength; i++) {
			if (snake[i].x == coin->x && snake[i].y == coin->y) {
				overlap = true;
				break;
			}
		}
	} while (overlap);
}
void move(snakeStruct* snake, char* direction, int* snakeLength, int* ateApple, appleStruct* apple) {
	//int key = 0;

	// 保存蛇头的原始位置
	int prevX = snake[0].x;
	int prevY = snake[0].y;

	printf("Before move - Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	printf("Before eat - Snake length: %d\n", *snakeLength);

	if (_kbhit()) {
		int newDirection = _getch();
		if ((newDirection == UPKEY) && *direction != 's') {
			//key = 1;
			*direction = 'w';
		}
		else if ((newDirection == DOWNKEY) && *direction != 'w') {
			//key = 1;
			*direction = 's';
		}
		else if ((newDirection == LEFTKEY) && *direction != 'd') {
			//key = 1;
			*direction = 'a';
		}
		else if ((newDirection == RIGHTKEY) && *direction != 'a') {
			//key = 1;
			*direction = 'd';
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
	if (*snakeLength > 0 && checkEatApple(&snake[0], apple)) {
		printf("Key is pressed, snake ate the apple!\n");

		// 增加蛇的长度
		(*snakeLength)++;

		// 设置新蛇段的位置为前一蛇段的位置
		snake[*snakeLength - 1].x = snake[*snakeLength - 2].x;
		snake[*snakeLength - 1].y = snake[*snakeLength - 2].y;

		placeApple(apple, snake, *snakeLength);
	}

	printf("After move - Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	printf("After eat - Snake length: %d\n", *snakeLength);
}
void setDisplay(snakeStruct* snake, appleStruct* apple, coinStruct* coin, int snakeLength) {
	system("cls");
	for (int i = 0; i < snakeLength; i++) {
		printf("Snake[%d] position: (%d, %d)\n", i, snake[i].x, snake[i].y);
	}

	// 上
	for (int i = 0; i < WIDTH + 2; i++) {
		printf("▪");
	}
	printf("\n");


	for (int y = 0; y < HEIGHT; y++) {
		printf("▪"); // 左
		for (int x = 0; x < WIDTH; x++) {
			int isSnakeBody = 0;

			for (int i = 0; i < snakeLength; i++) {
				if (snake[i].x == x && snake[i].y == y) {
					isSnakeBody = 1;
					break;
				}
			}

			if (isSnakeBody) {
				colorPrint("O", 1, 255, 1); // 蛇
			}
			else if (x == apple->x && y == apple->y) {
				printf("A"); // apple
			}
			else if (x == coin->x && y == coin->y) {
				printf("C");// coin
			}
			else {
				printf(" ");
			}
		}
		printf("▪"); // 右
		printf("\n");
	}

	// 下
	for (int i = 0; i < WIDTH + 2; i++) {
		printf("▪");
	}
	printf("\n");
}
int checkEatApple(snakeStruct* snake, appleStruct* apple) {
	printf("Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	printf("Apple position: (%d, %d)\n", apple->x, apple->y);
	return(snake[0].x == apple->x && snake[0].y == apple->y);
}
int checkEatCoin(snakeStruct* snake, coinStruct* coin) {
	printf("Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	printf("Coin position: (%d, %d)\n", coin->x, coin->y);
	return(snake[0].x == coin->x && snake[0].y == coin->y);
}
int checkBoundary(snakeStruct* snake) {
	return(snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT);
}