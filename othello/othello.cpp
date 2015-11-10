#include "stdio.h"
#include "stdafx.h"

#define scanf scanf_s
#define WALL -1
#define NONE 0
#define BLACK 1
#define WHITE 2

void resetBoard(void);

bool place(int x, int y, int knd);

void turn(int x, int y);

void printBoard(void);

static short board[10][10] = { 0 };// 0:なし 1:黒 2:白 -1:壁

int main(){

	int nowBlack, nowWhite;
	int x, y;
	int nowPlayer=BLACK;
	resetBoard();
	while (1){
		printBoard();
		do{
			if(nowPlayer == BLACK){
				printf("黒の番です\n");
			}else{
				printf("白の番です\n");
			}
			scanf("%d", &x);
			scanf("%d", &y);
			if(x>8 || y>8 || x<=0 || y<=0){
				printf("1~8の数値を入力してください\n");
			}
		}while(x>8 || y>8 || x<=0 || y<=0);
		if (place(x, y, nowPlayer) == true){
			if (nowPlayer == BLACK){
				nowPlayer = WHITE;
			}
			else{
				nowPlayer = BLACK;
			}
		}
		else{
			printf("そこには置けません\n");
		}
	}
	return 0;
}

bool place(int x, int y, int knd){	//x,y, knd...打ち手のこまの色
	unsigned short dir;				//上から右回りに8方向
	unsigned short cur_x, cur_y, turn_x[8], turn_y[8];	//current x,y
	int i, j, enemy, turn=0;
	if (knd == WHITE){
		enemy = BLACK;
	}
	else{
		enemy = WHITE;
	}

	for (dir = 1; dir <= 8; dir++){	//8方向に探索
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

			if (board[cur_x][cur_y] == knd){
				for (j = 0; i > j; j++){
					board[turn_x[j]][turn_y[j]] = knd;
					turn++;
				}
			}
			
			i++;
		}while (board[cur_x][cur_y] == enemy);
	}
	if(turn==0)return false;
	board[x][y] = knd;
	return true;
}

void resetBoard(){
	int i,j;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			board[i][j] = 0;
			if (i == 0 || j == 0 || i == 9 || j == 9){
				board[i][j] = -1;
			}
		}
	}
	board[4][4] = WHITE;
	board[4][5] = BLACK;
	board[5][4] = BLACK;
	board[5][5] = WHITE;
}

void printBoard(){
	int i, j;
	printf(" 0 1 2 3 4 5 6 7 8 9\n");
	for (i = 0; i < 10; i++){
		printf("%d", i);
		for (j = 0; j < 10; j++){
			switch (board[j][i]){
			case WALL:
				printf("■");
				break;
			case NONE:
				printf("□");
				break;
			case BLACK:
				printf("○");
				break;
			case WHITE:
				printf("●");
				break;
			}
		}
		putchar('\n');
	}
}