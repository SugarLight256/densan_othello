/* http://www.geocities.co.jp/SiliconValley-Bay/4543/Osero/index.html */
#include "stdio.h"
#include "stdafx.h"

#define scanf scanf_s
#define WALL -1
#define NONE 0
#define BLACK 1
#define WHITE 2

#define PBLACK false
#define PWHITE true

void whitePlace(void);
void blackPlace(void);

void resetBoard(void);

bool place(int x, int y, int knd);

bool canPlace(int x, int y, int knd);

void turn(int x, int y);

void printBoard(void);

void InitUndo(void);

static short _board[10][10] = { 0 };// 0:�Ȃ� 1:�� 2:�� -1:

static short pointBord[10][10]={
{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
{ 0, 30,-12,  0, -1, -1,  0,-12, 30,  0 },
{ 0,-12,-15, -3, -3, -3, -3,-15,-12,  0 },
{ 0,  0, -3,  0, -1, -1,  0, -3,  0,  0 },
{ 0, -1, -3, -1, -1, -1, -1, -3, -1,  0 },
{ 0, -1, -3, -1, -1, -1, -1, -3, -1,  0 },
{ 0,  0, -3,  0, -1, -1,  0, -3,  0,  0 },
{ 0,-12,-15, -3, -3, -3, -3,-15,-12,  0 },
{ 0, 30,-12,  0, -1, -1,  0,-12, 30,  0 },
{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
};

static short place_val;//����������ɂ�ďI��

static short phase;//����������ɂ�ďI��

static bool _Player = false;

typedef struct{
	int posx;
	int posy;
	int num;
	int position[10][10];
}Undo;

struct position{
	int x;
	int y;
};

int cpuX,cpuY;

int main(){

	int nowBlack, nowWhite;
	int x, y;
	resetBoard();
	while (1){//�������[�v
		printBoard();//�{�[�h�`��

		if(_Player == PBLACK){       //�e�v���C���[���Ƃɐ΂�u��
			blackPlace();
		}else if(_Player == PWHITE){
			whitePlace();
		}

		_Player = !_Player;//�v���C���[���
		place_val++;			//�萔�J�E���g
		if(place_val >= 60){    //�萔60��ŏI��
			break;
		}
	}
	return 0;
}

void whitePlace(){
	int x, y;
	do{
		printf("���̔Ԃł�\n");
		scanf("%d", &x);
		scanf("%d", &y);

		if(x>8 || y>8 || x<=0 || y<=0){
			printf("1~8�̐��l����͂��Ă�������\n");
		}
	}while(place(x, y, WHITE) != true || x>8 || y>8 || x<=0 || y<=0);
}

void blackPlace(){
	int x, y;
	do{
		printf("���̔Ԃł�\n");
		scanf("%d", &x);
		scanf("%d", &y);
		if(x>8 || y>8 || x<=0 || y<=0){
			printf("1~8�̐��l����͂��Ă�������\n");
		}
	}while(place(x, y, BLACK) != true || x>8 || y>8 || x<=0 || y<=0);
}

void turn(int x, int y){
	_board[x][y] = _board[x][y] % 2 + 1;
}

bool place(int x, int y, int knd){  //x,y, knd...�ł���̂��܂̐F
	unsigned short dir;				//�ォ��E����8����
	unsigned short cur_x, cur_y, turn_x[8], turn_y[8];	//current x,y
	int i, j, enemy, turn=0;
	if(_board[x][y] != NONE)return false;
	enemy = knd%2+1;

	for (dir = 1; dir <= 8; dir++){	//8�����ɒT��
		cur_x = x;
		cur_y = y;
		i = 0;
		do{
			switch (dir){
			case 1:
				cur_y++;
				break;
			case 2:
				cur_x++;
				cur_y++;
				break;
			case 3:
				cur_x++;
				break;
			case 4:
				cur_x++;
				cur_y--;
				break;
			case 5:
				cur_y--;
				break;
			case 6:
				cur_x--;
				cur_y--;
				break;
			case 7:
				cur_x--;
				break;
			case 8:
				cur_x--;
				cur_y++;
				break;
			}
			turn_x[i] = cur_x;
			turn_y[i] = cur_y;
			if (_board[cur_x][cur_y] == knd){
				for (j = 0; i > j; j++){
					_board[turn_x[j]][turn_y[j]] = knd;
					turn++;
				}
			}
			i++;
		}while (_board[cur_x][cur_y] == enemy);
	}
	if(turn<=0)return false;
	_board[x][y] = knd;
	return true;
}

bool canPlace(int x, int y, int knd){  //x,y, knd...�ł���̂��܂̐F
	unsigned short dir;				//�ォ��E����8����
	unsigned short cur_x, cur_y, turn_x[8], turn_y[8];	//current x,y
	int i, j, enemy, turn=0;
	if(_board[x][y] != NONE)return false;
	enemy = knd%2+1;

	for (dir = 1; dir <= 8; dir++){	//8�����ɒT��
		cur_x = x;
		cur_y = y;
		i = 0;
		do{
			switch (dir){
			case 1:
				cur_y++;
				break;
			case 2:
				cur_x++;
				cur_y++;
				break;
			case 3:
				cur_x++;
				break;
			case 4:
				cur_x++;
				cur_y--;
				break;
			case 5:
				cur_y--;
				break;
			case 6:
				cur_x--;
				cur_y--;
				break;
			case 7:
				cur_x--;
				break;
			case 8:
				cur_x--;
				cur_y++;
				break;
			}
			turn_x[i] = cur_x;
			turn_y[i] = cur_y;
			if (_board[cur_x][cur_y] == knd){
				for (j = 0; i > j; j++){
					turn++;
				}
			}
			i++;
		}while (_board[cur_x][cur_y] == enemy);
	}
	if(turn<=0)return false;
	return true;
}

void resetBoard(){
	int i,j;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			_board[i][j] = 0;
			if (i == 0 || j == 0 || i == 9 || j == 9){
				_board[i][j] = -1;
			}
		}
	}
	_board[4][4] = WHITE;
	_board[4][5] = BLACK;
	_board[5][4] = BLACK;
	_board[5][5] = WHITE;
}

void printBoard(){
	int i, j;
	printf(" 0 1 2 3 4 5 6 7 8 9\n");
	for (i = 0; i < 10; i++){
		printf("%d", i);
		for (j = 0; j < 10; j++){
			switch (_board[j][i]){
			case WALL:
				printf("��");
				break;
			case NONE:
				printf("��");
				break;
			case BLACK:
				printf("��");
				break;
			case WHITE:
				printf("��");
				break;
			}
		}
		putchar('\n');
	}
}

void cpu(){
}

/*int Minimax(){

}*/

void InitUndo(Undo* p_undo, int x, int y){
	p_undo->posx = x;
	p_undo->posy = y;
	p_undo->num  = 0;
	for(int i=0; i<10; i++){
		for(int j=0; j<10; j++){
			p_undo->position[i][j] = -1;
		}
	}
}

void Reverse(Undo undo){
	int i=0, j=0;
	for(i=0; i<10; i++){
		while(undo.position[i][j] != -1){
			turn(i,j);
			j++;
		}
	}
	_board[undo.posx][undo.posy] = NONE;
	_Player = !_Player;
}

int valuation(int x,int y){
	int point=0;

	switch(phase){
	case 1:
		point += pointBord[x][y]*1;
		break;
	case 2:
		break;
	case 3:
		break;
	}

	return point;
}