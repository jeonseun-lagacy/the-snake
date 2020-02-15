#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

//�� �ִ� ����
#define MAX_SNAKE_LENGTH 10
//�� ���
#define SNAKE_SHAPE "��"

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

//�ܼ� ȭ�鿡�� x, y��ǥ�� Ŀ�� �̵�
void gotoxy(int x, int y);

//�ܼ� Ŀ�� �����
void hideCursor();

//����Ű�� �� ȸ�� �ϱ�
void turnSnake(int nKey, struct Snake* snake);

//����������� �� �̵�
void moveSnake(struct Snake* snake);

//���� ���� Ȯ��
int addSnakeLength(struct Snake* snake);

//���� ���̿� ��ǥ�� �̿��Ͽ� ȭ�鿡 ���
void drawSnake(const struct Snake snake);

//�� ����, ����, ��ǥ ���� ����ü
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

	//���� ���� �� �� ���� �ʱ�ȭ
	struct Snake snake = { 77, 1, {5, 1} };

	while (1) {
		system("cls");
		//gotoxy(snake.pos->x, snake.pos->y);
		OldTime = clock();
		if (_kbhit()) {
			nKey = _getch();
			//����Ű �Է��� ��쿡�� ȸ��
			if (nKey == 224) {
				nKey = _getch();
				//Ű �Է°����� �� ȸ��
				turnSnake(nKey, &snake);
			}
		}
		//�� ���⿡ �°� �̵�
		moveSnake(&snake);
		drawSnake(snake);
		
		//�� ���� �ӵ� ����
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
	
	//�Ӹ� �̿��� �κ��� �ִ� ���
	if (snake->length > 1) {
		for (int i = 1; i < snake->length; i++) {
			
		}
	}
	snake->pos[0].x += deltaX;
	snake->pos[0].y += deltaY;
}

int addSnakeLength(struct Snake* snake){
	//���� ���� ������ ��� ������Ű�� 1 ��ȯ
	if (snake->length < MAX_SNAKE_LENGTH) {
		//TODO �Ӹ� �κп� ���ο� ������ �߰��ϰ� �������� ��ĭ�� �о
	}
	//�ִ� ������ ��� 0��ȯ
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
