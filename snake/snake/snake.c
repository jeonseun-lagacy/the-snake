#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

//뱀 최대 길이
#define MAX_SNAKE_LENGTH 10
//뱀 모양
#define SNAKE_SHAPE "○"

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

//뱀의 길이 확장
int addSnakeLength(struct Snake* snake);

//뱀의 길이와 좌표를 이용하여 화면에 출력
void drawSnake(const struct Snake snake);

//뱀 방향, 길이, 좌표 저장 구조체
struct Snake {
	int direction;
	int length;
	POINT pos[MAX_SNAKE_LENGTH];
};

int main(void) {
	hideCursor();
	const int game_speed = 66;
	int nKey;
	clock_t CurTime, OldTime;

	//게임 시작 전 뱀 상태 초기화
	struct Snake snake = { 77, 1, {5, 1} };

	while (1) {
		system("cls");
		//gotoxy(snake.pos->x, snake.pos->y);
		OldTime = clock();
		if (_kbhit()) {
			nKey = _getch();
			//방향키 입력일 경우에만 회전
			if (nKey == 224) {
				nKey = _getch();
				//키 입력값으로 뱀 회전
				turnSnake(nKey, &snake);
			}
		}
		//뱀 방향에 맞게 이동
		moveSnake(&snake);
		drawSnake(snake);
		
		//뱀 진행 속도 조절
		while (1) {
			CurTime = clock();
			if (CurTime - OldTime > game_speed) {
				break;
			}
		}
	}
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
	
	//머리 이외의 부분이 있는 경우
	if (snake->length > 1) {
		for (int i = 1; i < snake->length; i++) {
			
		}
	}
	snake->pos[0].x += deltaX;
	snake->pos[0].y += deltaY;
}

int addSnakeLength(struct Snake* snake){
	//길이 증가 가능한 경우 증가시키고 1 반환
	if (snake->length < MAX_SNAKE_LENGTH) {
		//TODO 머리 부분에 새로운 몸통을 추가하고 나머지를 한칸씩 밀어냄
	}
	//최대 길이인 경우 0반환
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
