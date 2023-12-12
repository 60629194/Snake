#define _CRT_SECURE_NO_WARNINGS
#include"global.h"
#include "GamePlay.h"
#include "LeaderBoard.h"
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <process.h>


#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77

int scdata;
int coinCount = 0;

short int WIDTH = 20;
short int HEIGHT = 10;
char direction = 'n';
int moveWhichReallyMove = 0;

typedef struct{
	int x;
	int y;
}position;

typedef struct {
	int r, g, b;
}color;

color wall = { 77,57,1 };
color snakeColor = { 1,255,1 };
color appleColor = { 255,1,1 };
color coinColor = { 255,255,1 };

static void placeApple(position* apple, position* snake, int snakeLength);
static void placeCoin(position* coin, position* snake, int snakeLength, position* apple);
static void move(position* snake, char* direction, int* snakeLength, position* apple,int snakeSpeed,bool clearMode);
static void setDisplay(position* snake, position* apple, position* coin, int snakeLength,int stage,char *skin);
static int checkEatApple(position* snake, position* apple);
static int checkEatCoin(position* snake, position* coin);
static int checkBoundary(position* snake);
static int level(int stage);
static int checkBody(position* snake,int snakeLength);
static void colorPrintForChar(char text, int red, int green, int blue);

void GamePlay(const char* filepath) {


	WIDTH = 20;
	HEIGHT = 10;
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
	int appleCount = 0;
	int stage = 1;
	int setMaxCoin = 0;
	coinCount = 0;
	int appleSum = 0;
	int coinSpawnTimer;
	int snakeCoinDistance;
	char *skinnow = readObject(filepath, 4);
	char skinNow = skinnow[0];
	bool coinExist=true;
	placeApple(&apple, snake, snakeLength);
	placeCoin(&coin, snake, snakeLength,&apple);
	snakeCoinDistance = abs(snake[0].x - coin.x) + abs(snake[0].y - coin.y);

	printf("Game Loading...");
	system("BGM.bat");

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
					move(snake, &direction, &snakeLength, &apple,level(stage)/2,false);
				}
				else
				{
					move(snake, &direction, &snakeLength, &apple, level(stage), false);
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
					if(SFX)
					{
						PlaySound(TEXT("eatSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
					apple.x = -1;
					apple.y = -1;
					placeApple(&apple, snake, snakeLength);
					appleSum++;
					appleCount++;
				}
				//check if hit yourself
				if (checkBody(snake,snakeLength) == 1 && moveWhichReallyMove > snakeLength) {
					if(SFX)
					{
						PlaySound(TEXT("gameOver.wav"), NULL, SND_FILENAME);
					}
					printf("you hit yourself");
					Sleep(3000);
					break;
				}
				//check if eat coin and spawn coin
				if (checkEatCoin(snake, &coin)) {
					if(SFX)
					{
						PlaySound(TEXT("coinSFX.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
					setMaxCoin--;
					coin.x = -1;
					coin.y = -1;
					coinExist = false;
					coinCount++;
					coinSpawnTimer = stage+(rand() % (stage+5));
				}
				else{
					coinSpawnTimer--;
				}
				if ((!coinExist) && setMaxCoin > 0 && coinSpawnTimer<=0) {
					placeCoin(&coin, snake, snakeLength,&apple);
					snakeCoinDistance = abs(snake[0].x - coin.x) + abs(snake[0].y - coin.y);
					coinExist = true;
				}
				scdata = stage * coinCount+appleSum;
				setDisplay(snake, &apple, &coin, snakeLength,stage,skinNow);
				//if hit wall
				if (checkBoundary(snake)) {
					if(SFX)
					{
						PlaySound(TEXT("wallBreak.wav"), NULL, SND_FILENAME);
					}
					printf("Gameover");
					Sleep(3000);
					break;
				}
			}
			if (checkBoundary(snake)) {
				break;
			}
			if (checkBody(snake, snakeLength) == 1 && moveWhichReallyMove > snakeLength) {
				break;
			}

			for(int i=3;i>0;i--){
				system("cls");
				printf("Next stage comes in %d second",i);
				Sleep(1000);
				move(snake, &direction, &snakeLength, &apple, 1,true);
			}

			for (int i = 0; i < WIDTH; i++) {
				move(snake, &direction, &snakeLength, &apple, 0,true);
			}

			stage++;
			WIDTH += 4;
			HEIGHT += 2;
		}
		if (checkBoundary(snake)) {
			break;
		}
		if (checkBody(snake, snakeLength) == 1 && moveWhichReallyMove > snakeLength) {
			break;
		}
		printf("you win");
	}
	system("stopBGM.bat");
}
void colorPrintForChar(char text, int red, int green, int blue) {
	printf("\x1b[38;2;%d;%d;%dm%c\x1b[0m", red, green, blue, text);
}
void placeApple(position* apple, position* snake, int snakeLength) {
	bool overlap;
	do {
		apple->x = rand() % WIDTH;
		apple->y = rand() % HEIGHT;
		// check apple/snake overlap
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
		// check coin/snake overlap
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
void move(position* snake, char* direction, int* snakeLength, position* apple,int snakeSpeed,bool clearMode) {

	// initial place of snake head
	int prevX = snake[0].x;
	int prevY = snake[0].y;

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

	// move snake body
	for (int i = *snakeLength - 1; i > 0; i--) {
		if (snake[i].x != -1 && snake[i].y != -1) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
	}

	// connect snake body and head
	snake[1].x = prevX;
	snake[1].y = prevY;

	// ate apple add snakelength
	if (*snakeLength > 0 && checkEatApple(&snake[0], apple)) {
		
		(*snakeLength)++;

		// set new snake body
		snake[*snakeLength - 1].x = snake[*snakeLength - 2].x;
		snake[*snakeLength - 1].y = snake[*snakeLength - 2].y;

	}
	
}
void setDisplay(position* snake, position* apple, position* coin, int snakeLength,int stage,char *skin) {
	system("cls");

	// up
	printf("level %d\n", stage);
	printf("score: %d\n", scdata);
	for (int i = 0; i < WIDTH/2 + 2; i++) {
		colorPrint("臣", wall.r, wall.g, wall.b);
	}
	
	printf("\n");
	
	for (int y = 0; y < HEIGHT; y++) {
		colorPrint("臣", wall.r, wall.g, wall.b); // left
		for (int x = 0; x < WIDTH; x++) {
			if (snake[0].x == x && snake[0].y == y) {//snake head
				if (direction == 'w') {
					colorPrint("^", snakeColor.r, snakeColor.g, snakeColor.b);
				}
				if (direction == 'a') {
					colorPrint("<", snakeColor.r, snakeColor.g, snakeColor.b);
				}
				if (direction == 's') {
					colorPrint("v", snakeColor.r, snakeColor.g, snakeColor.b);
				}
				if (direction == 'd') {
					colorPrint(">", snakeColor.r, snakeColor.g, snakeColor.b);
				}
				if (direction == 'n') {
					if (snakeLength <= 1) {
						colorPrintForChar(skin, snakeColor.r, snakeColor.g, snakeColor.b);
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
				colorPrintForChar(skin, snakeColor.r, snakeColor.g, snakeColor.b); // snake
			}
			else if (x == apple->x && y == apple->y) {
				colorPrint("Φ", appleColor.r, appleColor.g, appleColor.b); // apple
			}
			else if (x == coin->x && y == coin->y) {
				colorPrint("◎", coinColor.r, coinColor.g, coinColor.b);// coin
			}
			else {
				if (snake[0].x != x || snake[0].y != y) {
					if (coin->x == x - 1 && coin->y == y) {
						printf("");
					}
					else if (apple->x == x - 1 && apple->y == y) {
						printf("");
					}
					else
					{
						printf(" ");
					}
				}
			}
		}
		colorPrint("臣", wall.r, wall.g, wall.b); // right
		printf("\n");
	}

	// down
	for (int i = 0; i < WIDTH/2 + 2; i++) {
		colorPrint("臣", wall.r, wall.g, wall.b);
	}
	printf("\n");
}
int checkEatApple(position* snake, position* apple) {
	return(snake[0].x == apple->x && snake[0].y == apple->y);
}
int checkEatCoin(position* snake, position* coin) {
	return(snake[0].x == coin->x && snake[0].y == coin->y);
}
int checkBoundary(position* snake) {
	return(snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT);
}
int checkBody(position* snake,int snakeLength) {
	for (int i = 3; i < snakeLength; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			return 1;
		}
	}
	return 0;
}

int level(int stage) {
	return 1000 / stage;
}


