/* http://www.geocities.co.jp/SiliconValley-Bay/4543/Osero/index.html */
#include "stdafx.h"

#define scanf scanf_s
#define WALL -1
#define NONE 0
#define BLACK 1
#define WHITE 2

#define PBLACK false
#define PWHITE true   
#define SEARCH_DEEP 6

#define _DEBUG false

#define V_PLACE_HATE 2*3
#define V_CAN_HATE 1*2*10
#define V_NUM_HATE 1
#define V_KAKUTE_HATE 5*3*11

typedef struct{
	int x;
	int y;
	int num;
	int position[100];
}Undo;

void whitePlace(void);
void blackPlace(void);
void resetBoard(void);
bool place(Undo* undo);
bool canPlace(int x, int y);
void turn(int x, int y);
void turn(int y);
bool printBoard(void);

void cpu(void);
int Minmax(bool myTurn, int deep, bool put);
void RePlace(Undo undo);
int valuation(void);
int value_Place(void);
int value_CanPlace(void);
int value_Num(void);
int value_Kakute(void);

static short _board[10][10] = { 0 };

static short _StoneBoard[10][10] = {
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1,  0,  0,  0,  0,  0,  0,  0,  0, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
};


static short pointBoard[10][10]={
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0, 45,-11,  4, -1, -1,  4,-11, 45,  0 },
	{ 0,-11,-16, -1, -3, -3, -1,-16,-11,  0 },
	{ 0,  4, -1,  1, -1, -1,  1, -1,  4,  0 },
	{ 0, -1, -3, -1,  0,  0, -1, -3, -1,  0 },
	{ 0, -1, -3, -1,  0,  0, -1, -3, -1,  0 },
	{ 0,  4, -1,  1, -1, -1,  1, -1,  4,  0 },
	{ 0,-11,-16, -1, -3, -3, -1,-16,-11,  0 },
	{ 0, 45,-11,  4, -1, -1,  4,-11, 45,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
};

static short place_val;//����������ɂ�ďI��

static short phase=0;//����������ɂ�ďI��

static bool _Player = false;

static short int phaseDeep=0;

void InitUndo(Undo* undo, int x, int y);

int cpuX,cpuY;

int main(){

	int i;
	resetBoard();
	printBoard();
	while (1){//�������[�v
		if(_Player == PBLACK){       //�e�v���C���[���Ƃɐ΂�u��
			blackPlace();
		}else if(_Player == PWHITE){
			whitePlace();
		}

		place_val++;			//�萔�J�E���g
		if(place_val>=10){
			phase=1;
		}
		if(place_val >= 45){
			phase=2;
			phaseDeep=8-SEARCH_DEEP;
		}
		if(place_val > 60){    //�萔60��ŏI��
			break;
		}
		if (printBoard())break;//�{�[�h�`��
	}
	scanf("%d", &i);
	return 0;
}

void whitePlace(){
	/*do{
		printf("���̔Ԃł�\n");
		scanf("%d", &x);
		scanf("%d", &y);

		if(x>8 || y>8 || x<=0 || y<=0){
			printf("1~8�̐��l����͂��Ă�������\n");
		}else{
			InitUndo(&undo, x, y);
		}
	}while(place(&undo) != true || x>8 || y>8 || x<=0 || y<=0);  */
	printf("CPU �l����...\n");
	cpu();
}

void blackPlace(){
	/*int x, y;
	Undo undo;
	do{
		printf("���̔Ԃł�\n");
		scanf("%d", &x);
		scanf("%d", &y);
		if(x>8 || y>8 || x<=0 || y<=0){
			printf("1~8�̐��l����͂��Ă�������\n");
			continue;
		}else{
			InitUndo(&undo, x, y);
		}
	}while(place(&undo) != true || x>8 || y>8 || x<=0 || y<=0);*/
	cpu();
}

void turn(int x, int y){
	_board[x][y] = _board[x][y] % 2 + 1;
}

void turn(int y){
	int x=0;
	
	x = y%10;
	y = y/10;  
	turn (x,y);
} 

bool place(Undo* undo){  //x,y, knd...�ł���̂��܂̐F
	unsigned short dir;				//�ォ��E����8����
	unsigned short cur_x, cur_y, turn_x[8], turn_y[8], x, y;	//current x,y
	int i, j, enemy, turn=0, knd;
	x = undo->x;
	y = undo->y;
	if(_board[x][y] != NONE)return false;
	if(_Player == PWHITE){
		knd = WHITE;
		enemy = BLACK;
	}else{
		knd = BLACK;
		enemy = WHITE;
	}
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
					undo->position[turn]=turn_x[j]+turn_y[j]*10;
					turn++;
				}
			}
			i++;
		}while (_board[cur_x][cur_y] == enemy);
	}
	if(turn<=0){
		printf("�����ɂ͒u���܂���\n");
		return false;
	}
	_board[x][y] = knd;
	_Player = !_Player;
	return true;
}

bool canPlace(int x, int y){  //x,y, knd...�ł���̂��܂̐F
	unsigned short dir;				//�ォ��E����8����
	unsigned short cur_x, cur_y, turn_x[8], turn_y[8];	//current x,y
	int i, j, enemy, turn=0, knd;
	if(_board[x][y] != NONE)return false;
	if(_Player == PWHITE){
		knd = WHITE;
		enemy = BLACK;
	}else{
		knd = BLACK;
		enemy = WHITE;
	}

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

bool printBoard(){
	int i, j;
	int whiteCount=0, blackCount=0;
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
				blackCount++;
				break;
			case WHITE:
				printf("��");
				whiteCount++;
				break;
			}
		}
		putchar('\n');
	}
	printf("��:%d ��:%d\n\n", blackCount, whiteCount);
	if(blackCount+whiteCount >= 64){
		if(blackCount > whiteCount){
			printf("���̏���\n");
			return true;
		}else if(whiteCount > blackCount){
			printf("���̏���\n");
			return true;
		}else{
			printf("��������\n");
			return true;
		}
	}
	if(blackCount == 0){
		printf("���̏���\n");
		return true;
	}
	if(whiteCount == 0){
		printf("���̏���\n");
		return true;
	}
	return false;
}

void cpu(){
	int x=0, y=0;
	Undo undo;
	
	y = Minmax(true, SEARCH_DEEP+phaseDeep, true);

	if(y<0 || y>10*10){
		_Player = !_Player;
		printf("CPU �p�X(���E�ցE�M)\n");
		return;
	}
	
	x = y%10;
	y = y/10;
	
	if(_DEBUG){
		printBoard();
	}
	InitUndo(&undo, x, y);
	place(&undo);
	printf("CPU:(x:%d,y:%d)�ɒu����\n", x, y);
	
	for(x=0; x<10*10; x++){
		if(place_val >= 60)break;
		if(canPlace(x%10, x/10))break;
		if(x == 10*10-1){
			_Player = !_Player;
			cpu();
		}
	}
}

int Minmax(bool myTurn, int deep, bool put){
	int value, tmp, bestX=-1, bestY=-1;
	Undo undo;
	
	if(deep <= 0){
		if(_DEBUG){
			printf("value:%d\n",tmp=valuation());
		}
		return valuation();
	}
	
	if(myTurn){
		value = -99999;
	}else{
		value = 99999;
	}
	
	for(int x=0; x<10; x++){
		for(int y=0; y<10; y++){
			if(canPlace(x,y)){
				if(_DEBUG){
					printf("%d place (%d,%d)\n",_Player,x,y);
				}
				InitUndo(&undo, x, y);
				place(&undo);
				tmp = Minmax(!myTurn, deep-1, true);
				RePlace(undo);
				if(_DEBUG){
					printf("back branch\n\n");
				}
				if((myTurn && tmp>value) || (myTurn && tmp<value)){
					value = tmp;
					bestX = x;
					bestY = y;
				}
			}
		}
	}
	if(deep == SEARCH_DEEP+phaseDeep){
		return bestX+bestY*10;
	}else if(value != -99999 && value != 99999){
		return value;
	}else if(!put){
		return valuation();
	}else{
		_Player = !_Player;
		tmp = Minmax(!myTurn, deep-1, false);
		_Player = !_Player;
		return tmp;
	}                     
}

void InitUndo(Undo* p_undo, int x, int y){
	p_undo->x = x;
	p_undo->y = y;
	p_undo->num  = 0;
	for(int i=0; i<10*10; i++){
		p_undo->position[i] = -1;
	}
}

void RePlace(Undo undo){
	int i=0;
	while(undo.position[i] != -1){
		turn(undo.position[i]);
		i++;
	}
	_board[undo.x][undo.y] = NONE;
	_Player = !_Player;
}

int valuation(){
	int point=0;

	switch(phase){
	case 0:
		point += value_Place()*V_PLACE_HATE;
		point += value_CanPlace()*V_CAN_HATE;
		point += value_Num()*V_NUM_HATE;
		point += value_Kakute()*V_KAKUTE_HATE;
		break;
	case 1:
		point += value_Place()*V_PLACE_HATE;
		point += value_CanPlace()*V_CAN_HATE;
		point += value_Num()*V_NUM_HATE;
		point += value_Kakute()*V_KAKUTE_HATE;
		break;
	case 2:
		point += value_Place()*V_PLACE_HATE;
		point += value_CanPlace()*V_CAN_HATE;
		point += value_Num()*V_NUM_HATE;
		point += value_Kakute()*V_KAKUTE_HATE;
		break;
	}

	return point;
}

int value_Place(){//�΂̈ʒu���|�C���g�Ղ���擾���ĕԂ�
	int point=0;
	for(int x=1; x<9; x++){
		for(int y=1; y<9; y++){
			if(_board[x][y]==WHITE){
				point += pointBoard[x][y];
			}else if(_board[x][y]==BLACK){
				point -= pointBoard[x][y];
			}
		}
	}
	return point;
}

int value_CanPlace(){//�����u���邩
	int point = 0;
	for(int x=1; x<9; x++){
		for(int y=1; y<9; y++){
		    if(canPlace(x,y)){
				point++;
			}
		}
	}
	if(_Player == PWHITE){
		return point;
	}else if(_Player == PBLACK && point == 0){
		return 9999;
	}
	else {
		return -point;
	}
}

int value_Num(){//�������Ԃ��邩
	int point = 0;
	for(int x=1; x<9; x++){
		for(int y=1; y<9; y++){
			if(_board[x][y]==WHITE){
				point++;
			}else if(_board[x][y]==BLACK){
				point--;
			}
		}
	}
	return point;
}

int value_Kakute() {//�m��΂̐����|�C���g�ɂ��ĕԂ�
	int point = 0;
	int vecX, vecY, moveX, moveY, tmp;
	int vecList[4] = { 1,0,-1,0 };
	for (int x = 1; x <= 8; x += 7) {
		for (int y = 1; y <= 8; y += 7) {
			if (_board[x][y] == WHITE || _board[x][y] == BLACK) {
				tmp = _board[x][y];
				for (int i = 0; i < 4; i++) {//vecX -> 1,0,-1,-2��-2�𖳎�
					vecX = vecList[i];
					if (vecX == 0) {
						vecY = vecList[i - 1];
					}
					else {
						vecY = 0;
					}
					moveX = 0;
					moveY = 0;
					do{
						if (_board[x][y] == BLACK) {
							point--;
						}
						else {
							point++;
						}
						moveX += vecX;
						moveY += vecY;
					} while (_board[x + moveX][y + moveY] == tmp);
					if (_board[x][y] == BLACK) {
						point++;
					}
					else {
						point--;
					}
				}
			}
		}
	}
	return point;
}