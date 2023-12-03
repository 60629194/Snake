#define _CRT_SECURE_NO_WARNINGS
#include "GamePlay.h"
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
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
int moveWhichReallyMove = 0;

typedef struct{
	int x;
	int y;
}position;


static void colorPrint(const char* text, int red, int green, int blue);
static void placeApple(position* apple, position* snake, int snakeLength);
static void placeCoin(position* coin, position* snake, int snakeLength, position* apple);
static void move(position* snake, char* direction, int* snakeLength, int* ateApple, position* apple,int snakeSpeed,bool clearMode);
static void setDisplay(position* snake, position* apple, position* coin, int snakeLength,int stage,char *skin);
static int checkEatApple(position* snake, position* apple);
static int checkEatCoin(position* snake, position* coin);
static int checkBoundary(position* snake);
static int level(int stage);
static int checkBody(position* snake,int snakeLength);
static int absolute(int value);
static char* readObject(const char* filepath, int lineNumber);
static void colorPrintForChar(char text, int red, int green, int blue);

void GamePlay(const char* filepath) {
	system("chcp 950");
	srand((int)time(0));
	position snake[100];
	position apple;
	position coin;
	for (int i = 0; i < 100; i++) {
		snake[i].x = -1;
		snake[i].y = -1;
	}

	snake->x = WIDTH / 2;
	snake->y = HEIGHT / 2;

	int snakeLength = 1;
	int ateApple = 0;
	int appleCount = 0;
	int coinCount = 0;
	int stage = 1;
	int setMaxCoin = 0;
	int appleSum = 0;
	int coinSpawnTimer;
	int snakeCoinDistance;
	char *skinnow = readObject(filepath, 4);
	char skinNow = skinnow[0];
	bool coinExist=true;
	placeApple(&apple, snake, snakeLength);
	placeCoin(&coin, snake, snakeLength,&apple);
	snakeCoinDistance = absolute(snake[0].x - coin.x) + absolute(snake[0].y - coin.y);
	//printf("Apple position: (%d, %d)\n", apple.x, apple.y);
	printf("Game Loading...");
	while (1) {
		while (stage < 10) {
			appleCount = 0;
			moveWhichReallyMove = 0;
			snakeLength = stage + 1;
			setMaxCoin = stage;
			coinSpawnTimer = 0;
			snake->x = WIDTH / 2;
			snake->y = HEIGHT / 2;
			direction = 'n';

			while (appleCount < stage * 2) {
				//run or not
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
					move(snake, &direction, &snakeLength, &ateApple, &apple,level(stage)/2,false);
				}
				else
				{
					move(snake, &direction, &snakeLength, &ateApple, &apple, level(stage), false);
				}
				
				//coin disappear if not eat in shortest distance
				if (direction != 'n') {
					snakeCoinDistance--;
				}
				if (snakeCoinDistance < 0 && coinExist) {
					setMaxCoin--;
					coin.x = -1;
					coin.y = -1;
					coinExist = false;
					coinSpawnTimer = stage + (rand() % (stage + 5));
				}
				//check if eat apple
				if (checkEatApple(snake, &apple)) {
					PlaySound(TEXT("eatSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					apple.x = -1;
					apple.y = -1;
					placeApple(&apple, snake, snakeLength);
					appleSum++;
					//printf("Apple position: (%d, %d)\n", apple.x, apple.y);
					ateApple = 0;
					appleCount++;
				}
				//check if hit yourself
				if (checkBody(snake,snakeLength) == 1 && moveWhichReallyMove > snakeLength) {
					PlaySound(TEXT("gameOver.wav"), NULL, SND_FILENAME);
					printf("you hit yourself");
					exit(0);
				}
				//check if eat coin and spawn coin
				if (checkEatCoin(snake, &coin)) {
					PlaySound(TEXT("coinSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					setMaxCoin--;
					coin.x = -1;
					coin.y = -1;
					coinExist = false;
					//printf("Coin position: (%d, %d)\n", coin.x, coin.y);
					coinCount++;
					coinSpawnTimer = stage+(rand() % (stage+5));
				}
				else{
					coinSpawnTimer--;
				}
				if ((!coinExist) && setMaxCoin > 0 && coinSpawnTimer<=0) {
					placeCoin(&coin, snake, snakeLength,&apple);
					snakeCoinDistance = absolute(snake[0].x - coin.x) + absolute(snake[0].y - coin.y);
					coinExist = true;
				}
				scdata = stage * coinCount+appleSum;
				setDisplay(snake, &apple, &coin, snakeLength,stage,skinNow);
				//if hit wall
				if (checkBoundary(snake)) {
					PlaySound(TEXT("wallBreak.wav"), NULL, SND_FILENAME);
					printf("Gameover");
					exit(0);
				}
			}

			for(int i=3;i>0;i--){
				system("cls");
				printf("Next stage comes in %d second",i);
				Sleep(1000);
				move(snake, &direction, &snakeLength, &ateApple, &apple, 1,true);
			}

			for (int i = 0; i < WIDTH; i++) {
				move(snake, &direction, &snakeLength, &ateApple, &apple, 0,true);
			}

			
			stage++;
			WIDTH += 4;
			HEIGHT += 2;
		}
		printf("you win");
	}
}

void colorPrint(const char* text, int red, int green, int blue) {
	printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m", red, green, blue, text);
}
void colorPrintForChar(char text, int red, int green, int blue) {
	printf("\x1b[38;2;%d;%d;%dm%c\x1b[0m", red, green, blue, text);
}
void placeApple(position* apple, position* snake, int snakeLength) {
	bool overlap;
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
		if (apple->x % 2 == 1) {
			overlap = true;
		}
	} while (overlap);
}
void placeCoin(position* coin, position* snake, int snakeLength,position* apple) {
	bool overlap;
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
		if (coin->x % 2 == 1) {
			overlap = true;
		}
		if (apple->x == coin->x && apple->y == coin->y) {
			overlap = true;
		}
	} while (overlap);
}
void move(position* snake, char* direction, int* snakeLength, int* ateApple, position* apple,int snakeSpeed,bool clearMode) {

	// 保存蛇头的原始位置
	int prevX = snake[0].x;
	int prevY = snake[0].y;

	//printf("Before move - Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	//printf("Before eat - Snake length: %d\n", *snakeLength);
	DWORD startTime = GetTickCount();
	DWORD timeNow = GetTickCount();
	while (((int)timeNow-(int)startTime)<snakeSpeed) {
		timeNow = GetTickCount();
		if (_kbhit()) {
			int newDirection = _getch();
			if ((newDirection == UPKEY) && *direction != 's'&& snake[0].y!=snake[1].y+1) {
				*direction = 'w';
			}
			else if ((newDirection == DOWNKEY) && *direction != 'w'&&snake[0].y!=snake[1].y-1) {
				*direction = 's';
			}
			else if ((newDirection == LEFTKEY) && *direction != 'd'&&snake[0].x!=snake[1].x+2) {
				*direction = 'a';
			}
			else if ((newDirection == RIGHTKEY) && *direction != 'a'&&snake[0].x!=snake[1].x-2) {
				*direction = 'd';
			}
		}
	}
	if (clearMode) {
		*direction = 'n';
	}
	switch (*direction) {
	case 'w':
		snake[0].y--;
		moveWhichReallyMove++;
		break;
	case 's':
		snake[0].y++;
		moveWhichReallyMove++;
		break;
	case 'a':
		snake[0].x -= 2;
		moveWhichReallyMove++;
		break;
	case 'd':
		snake[0].x += 2;
		moveWhichReallyMove++;
		break;
	default:
		snake[0].x = WIDTH/2;
		snake[0].y = HEIGHT/2;
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

	// 如果蛇的长度大于1，且吃到了苹果，则增加蛇的长度
	if (*snakeLength > 0 && checkEatApple(&snake[0], apple)) {
		//printf("Key is pressed, snake ate the apple!\n");

		// 增加蛇的长度
		(*snakeLength)++;

		// 设置新蛇段的位置为前一蛇段的位置
		snake[*snakeLength - 1].x = snake[*snakeLength - 2].x;
		snake[*snakeLength - 1].y = snake[*snakeLength - 2].y;

		//placeApple(apple, snake, *snakeLength);
	}
	//printf("After move - Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	//printf("After eat - Snake length: %d\n", *snakeLength);
}
void setDisplay(position* snake, position* apple, position* coin, int snakeLength,int stage,char *skin) {
	system("cls");
	/*for (int i = 0; i < snakeLength; i++) {
		printf("Snake[%d] position: (%d, %d)\n", i, snake[i].x, snake[i].y);
	}
	printf("%d\n", snakeLength);*/
	// 上
	printf("level %d\n", stage);
	printf("score: %d\n", scdata);
	for (int i = 0; i < WIDTH/2 + 2; i++) {
		colorPrint("臣",77,57,1);
	}
	
	printf("\n");
	
	for (int y = 0; y < HEIGHT; y++) {
		colorPrint("臣",77,57,1); // 左
		for (int x = 0; x < WIDTH; x++) {
			if (snake[0].x == x && snake[0].y == y) {
				if (direction == 'w') {
					colorPrint("^", 1, 255, 1);
				}
				if (direction == 'a') {
					colorPrint("<", 1, 255, 1);
				}
				if (direction == 's') {
					colorPrint("v", 1, 255, 1);
				}
				if (direction == 'd') {
					colorPrint(">", 1, 255, 1);
				}
				if (direction == 'n') {
					if (snakeLength <= 1) {
						colorPrintForChar(skin, 1, 255, 1);
					}
				}
			}
			bool isSnakeBody = false;

			for (int i = 1; i < snakeLength; i++) {
				if (snake[i].x == x && snake[i].y == y) {
					isSnakeBody = true;
					break;
				}
			}
			
			if (isSnakeBody) {
				colorPrintForChar(skin, 1, 255, 1); // 蛇
			}
			else if (x == apple->x && y == apple->y) {
				colorPrint("Φ", 255, 1, 1); // apple
			}
			else if (x == coin->x && y == coin->y) {
				colorPrint("◎", 255, 255, 1);// coin
			}
			else {
				if (snake[0].x != x || snake[0].y != y) {
					printf(" ");
				}
			}
		}
		colorPrint("臣",77,57,1); // 右
		printf("\n");
	}

	// 下
	for (int i = 0; i < WIDTH/2 + 2; i++) {
		colorPrint("臣",77,57,1);
	}
	printf("\n");
}
int checkEatApple(position* snake, position* apple) {
	//printf("Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	//printf("Apple position: (%d, %d)\n", apple->x, apple->y);
	return(snake[0].x == apple->x && snake[0].y == apple->y);
}
int checkEatCoin(position* snake, position* coin) {
	//printf("Snake head position: (%d, %d)\n", snake[0].x, snake[0].y);
	//printf("Coin position: (%d, %d)\n", coin->x, coin->y);
	return(snake[0].x == coin->x && snake[0].y == coin->y);
}
int checkBoundary(position* snake) {
	return(snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT);
}
int checkBody(position* snake,int snakeLength) {
	for (int i = 3; i < snakeLength; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			printf("hit the snake[%d]\n", i);
			return 1;
		}
	}
	return 0;
}

int level(int stage) {
	return 1000 / stage;
}

int absolute(int value) {
	if (value < 0) {
		return -value;
	}
	else
	{
		return value;
	}
}

char* readObject(const char* filepath, int lineNumber) {
	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		return "File not found.";
	}

	char* line = NULL;
	size_t len = 0;
	int currentLine = 0;
	int maxLineLength = 1000; // Adjust as needed

	while (currentLine < lineNumber && !feof(file)) {
		line = malloc(maxLineLength * sizeof(char));
		if (fgets(line, maxLineLength, file) != NULL) {
			currentLine++;
		}
		else {
			free(line);
			line = NULL;
			break; // Break loop on EOF or error
		}
	}

	fclose(file);

	if (line == NULL) {
		return "Line number exceeds file length.";
	}

	return line;
}