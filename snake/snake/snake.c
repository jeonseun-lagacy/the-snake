#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>

//뱀 최대 길이
#define MAX_SNAKE_LENGTH 10
//뱀 모양
#define SNAKE_SHAPE "○"

//뱀 초기위치 (쉬운 좌표계산을 위해 맵 좌표와 동일한 짝수로 생성)
#define SNAKE_POS_X 6
#define SNAKE_POS_Y 4

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

//콘솔 화면에서 x, y좌표로 커서 이동
void gotoxy(int x, int y);

//콘솔 커서 숨기기
void hideCursor();

//방향키로 뱀 회전 하기
void turnSnake(int nKey, struct Snake* snake);

//진행방향으로 뱀 이동
void moveSnake(struct Snake* snake);

//뱀의 맵 이탈 확인
int isSnakeOut(const struct Snake snake, int mapWidth, int mapHeight);

//뱀의 길이 확장
int addSnakeLength(struct Snake* snake);

//뱀이 아이템을 먹는지 확인하고 먹인경우 아이템 구조체 값 변경
int isSnakeGetItem(const struct Snake snake, struct Item* item);

//뱀의 길이와 좌표를 이용하여 화면에 출력
void drawSnake(const struct Snake snake);

//이전에 그려진 뱀을 지움
void eraseSnake(const struct Snake snake);

//뱀의 먹이 생성
void spawnItem(struct Item* item, int mapWidth, int mapHeight);

//맵 그리기
void drawMap(int width, int height);

//게임 진행상황별 메시지 출력
void printGameState(char* gameState);

//뱀 방향, 길이, 좌표 저장 구조체
struct Snake {
	int direction;
	int length;
	POINT pos[MAX_SNAKE_LENGTH];
};

//아이템 모양 및 좌표 
//TODO 아이템 종류 추가 가능하도록 만들기
struct Item {
	char* itemShape;
	int state;//생성되었으면 1, 제거되었으면 0
	POINT pos;
};

int main(void) {
	//맵 최대 크기
	int mapWidth = 30;
	int mapHeight = 25;
	
	int gameSpeed = 132;
	int nKey;
	clock_t CurTime, OldTime;

	//게임 시작 전 뱀 상태 초기화
	struct Snake snake = { 77, 1, {SNAKE_POS_X, SNAKE_POS_Y} };
	struct Item item = { "ㅁ", 0, {0,0} };
	//커서 가리기
	hideCursor();

	//랜덤 시드 초기화
	srand(time(NULL));
	//한 스테이지에 대한 맵 생성 및 게임 수행
	drawMap(mapWidth, mapHeight);
	
	while (1) {
		//뱀의 맵 이탈 확인
		if (isSnakeOut(snake, mapWidth, mapHeight)) {
			printGameState("game over");
			break;
		}
		//뱀 최대길이 달성
		if (snake.length == MAX_SNAKE_LENGTH) {
			printGameState("clear");
			break;
		}
		OldTime = clock();
		if (_kbhit()) {
			nKey = _getch();
			//방향키 입력일 경우에만 회전
			if (nKey == 224) {
				nKey = _getch();
				//키 입력값으로 뱀 회전
				turnSnake(nKey, &snake);
				//TEST...
				//addSnakeLength(&snake);
			}
		}
		spawnItem(&item, mapWidth, mapHeight);
		drawSnake(snake);
		if (isSnakeGetItem(snake, &item)) {
			addSnakeLength(&snake);
		}
		//뱀 진행 속도 조절
		while (1) {
			CurTime = clock();
			if (CurTime - OldTime > gameSpeed) {
				break;
			}
		}
		eraseSnake(snake);
		moveSnake(&snake);
	}
	getchar();
	return 0;
}

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void turnSnake(int nKey, struct Snake* snake) {
	switch (nKey) {
	case UP:
		snake->direction = UP;
		break;
	case DOWN:
		snake->direction = DOWN;
		break;
	case LEFT:
		snake->direction = LEFT;
		break;
	case RIGHT:
		snake->direction = RIGHT;
		break;
	}
}

void moveSnake(struct Snake* snake){
	int deltaX = 0;
	int deltaY = 0;
	switch (snake->direction) {
	case UP:
		deltaY = -1;
		break;
	case DOWN:
		deltaY = 1;
		break;
	case LEFT:
		deltaX -= strlen(SNAKE_SHAPE);
		break;
	case RIGHT:
		deltaX = strlen(SNAKE_SHAPE);
		break;
	}
	
	//길이가 2이상인 경우
	if (snake->length > 1) {
		for (int i = snake->length - 1; i > 0; i--) {
			snake->pos[i].x = snake->pos[i-1].x;
			snake->pos[i].y = snake->pos[i-1].y;
		}
	}
	snake->pos[0].x += deltaX;
	snake->pos[0].y += deltaY;
}

int isSnakeOut(const struct Snake snake, int mapWidth, int mapHeight){
	if (!snake.pos->x || !snake.pos->y) {
		return 1;
	}
	else if (snake.pos->x == (mapWidth*2 -2) || snake.pos->y == mapHeight - 1) {
		return 1;
	}
	else {
		return 0;
	}
}

int addSnakeLength(struct Snake* snake){
	//길이 증가 가능한 경우 증가시키고 1 반환
	if (snake->length < MAX_SNAKE_LENGTH) {
		//TODO 머리 부분에 새로운 몸통을 추가하고 나머지를 한칸씩 밀어냄
		snake->length++;
		for (int i = snake->length - 1; i > 0; i--) {
			snake->pos[i].x = snake->pos[i - 1].x;
			snake->pos[i].y = snake->pos[i - 1].y;
		}
		return 1;
	}
	//최대 길이인 경우 0반환
	else {
		return 0;
	}
}

int isSnakeGetItem(const struct Snake snake, struct Item* item)
{
	if (snake.pos[0].x == item->pos.x && snake.pos[0].y == item->pos.y) {
		item->state = 0;
		return 1;
	}
	else {
		return 0;
	}
	
}

void drawSnake(const struct Snake snake){
	for (int i = 0; i < snake.length; i++) {
		gotoxy(snake.pos[i].x, snake.pos[i].y);
		printf("%s", SNAKE_SHAPE);
	}
}

void eraseSnake(const struct Snake snake){
	for (int i = 0; i < snake.length; i++) {
		gotoxy(snake.pos[i].x, snake.pos[i].y);
		for (int j = 0; j < strlen(SNAKE_SHAPE); j++) {
			printf(" ");
		}
	}
}

//TODO 맵 테두리 내부에 생성되도록 만들것
void spawnItem(struct Item* item, int mapWidth, int mapHeight){
	if (!item->state) {
		int randX, randY;//생성될 아이템 좌표
		do {
			randX = rand() % ((mapWidth * 2) - 4) + 2;
		} while (randX % 2 != 0);
		randY = rand() % (mapHeight - 2) + 1;
		item->pos.x = randX;
		item->pos.y = randY;
		item->state = 1;
		gotoxy(randX, randY);
		printf("♥");
	}
}

void drawMap(int width, int height){
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i == 0 || j == width - 1 || i == height - 1 || j == 0) {
				printf("▒");
			}
			else {
				printf("  ");
			}
		}printf("\n");
	}
}

void printGameState(char* gameState){
	gotoxy(12, 7);
	if (!strcmp(gameState, "game over")) {
		printf("┌───────────┐");
		gotoxy(12, 8);
		printf("│ GAME OVER!│");
		gotoxy(12, 9);
		printf("└───────────┘");
	}

	if (!strcmp(gameState, "clear")) {
		printf("┌─────────────┐");
		gotoxy(12, 8);
		printf("│ STAGE CLEAR!│");
		gotoxy(12, 9);
		printf("└─────────────┘");
	}
}
