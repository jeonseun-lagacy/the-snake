#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>

//�� �ִ� ����
#define MAX_SNAKE_LENGTH 10
//�� ���
#define SNAKE_SHAPE "��"

//�� �ʱ���ġ (���� ��ǥ����� ���� �� ��ǥ�� ������ ¦���� ����)
#define SNAKE_POS_X 6
#define SNAKE_POS_Y 4

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

//���� �� ��Ż Ȯ��
int isSnakeOut(const struct Snake snake, int mapWidth, int mapHeight);

//���� ���� Ȯ��
int addSnakeLength(struct Snake* snake);

//���� �������� �Դ��� Ȯ���ϰ� ���ΰ�� ������ ����ü �� ����
int isSnakeGetItem(const struct Snake snake, struct Item* item);

//���� ���̿� ��ǥ�� �̿��Ͽ� ȭ�鿡 ���
void drawSnake(const struct Snake snake);

//������ �׷��� ���� ����
void eraseSnake(const struct Snake snake);

//���� ���� ����
void spawnItem(struct Item* item, int mapWidth, int mapHeight);

//�� �׸���
void drawMap(int width, int height);

//���� �����Ȳ�� �޽��� ���
void printGameState(char* gameState);

//�� ����, ����, ��ǥ ���� ����ü
struct Snake {
	int direction;
	int length;
	POINT pos[MAX_SNAKE_LENGTH];
};

//������ ��� �� ��ǥ 
//TODO ������ ���� �߰� �����ϵ��� �����
struct Item {
	char* itemShape;
	int state;//�����Ǿ����� 1, ���ŵǾ����� 0
	POINT pos;
};

int main(void) {
	//�� �ִ� ũ��
	int mapWidth = 30;
	int mapHeight = 25;
	
	int gameSpeed = 132;
	int nKey;
	clock_t CurTime, OldTime;

	//���� ���� �� �� ���� �ʱ�ȭ
	struct Snake snake = { 77, 1, {SNAKE_POS_X, SNAKE_POS_Y} };
	struct Item item = { "��", 0, {0,0} };
	//Ŀ�� ������
	hideCursor();

	//���� �õ� �ʱ�ȭ
	srand(time(NULL));
	//�� ���������� ���� �� ���� �� ���� ����
	drawMap(mapWidth, mapHeight);
	
	while (1) {
		//���� �� ��Ż Ȯ��
		if (isSnakeOut(snake, mapWidth, mapHeight)) {
			printGameState("game over");
			break;
		}
		//�� �ִ���� �޼�
		if (snake.length == MAX_SNAKE_LENGTH) {
			printGameState("clear");
			break;
		}
		OldTime = clock();
		if (_kbhit()) {
			nKey = _getch();
			//����Ű �Է��� ��쿡�� ȸ��
			if (nKey == 224) {
				nKey = _getch();
				//Ű �Է°����� �� ȸ��
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
		//�� ���� �ӵ� ����
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
	
	//���̰� 2�̻��� ���
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
	//���� ���� ������ ��� ������Ű�� 1 ��ȯ
	if (snake->length < MAX_SNAKE_LENGTH) {
		//TODO �Ӹ� �κп� ���ο� ������ �߰��ϰ� �������� ��ĭ�� �о
		snake->length++;
		for (int i = snake->length - 1; i > 0; i--) {
			snake->pos[i].x = snake->pos[i - 1].x;
			snake->pos[i].y = snake->pos[i - 1].y;
		}
		return 1;
	}
	//�ִ� ������ ��� 0��ȯ
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

//TODO �� �׵θ� ���ο� �����ǵ��� �����
void spawnItem(struct Item* item, int mapWidth, int mapHeight){
	if (!item->state) {
		int randX, randY;//������ ������ ��ǥ
		do {
			randX = rand() % ((mapWidth * 2) - 4) + 2;
		} while (randX % 2 != 0);
		randY = rand() % (mapHeight - 2) + 1;
		item->pos.x = randX;
		item->pos.y = randY;
		item->state = 1;
		gotoxy(randX, randY);
		printf("��");
	}
}

void drawMap(int width, int height){
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i == 0 || j == width - 1 || i == height - 1 || j == 0) {
				printf("��");
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
		printf("��������������������������");
		gotoxy(12, 8);
		printf("�� GAME OVER!��");
		gotoxy(12, 9);
		printf("��������������������������");
	}

	if (!strcmp(gameState, "clear")) {
		printf("������������������������������");
		gotoxy(12, 8);
		printf("�� STAGE CLEAR!��");
		gotoxy(12, 9);
		printf("������������������������������");
	}
}
