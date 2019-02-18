#include <stdio.h>
#include <iostream>
#include <limits.h>

using namespace std;

#define BRDSIZE	19
#define PLAYER1	1
#define PLAYER2	2

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))  

int board[19][19] = { 0, };
int dir[3][2] = { {1,0},{1,1},{0,1} };

void printBoard();
bool checkFinish();
bool actionFlag(int y, int x);
void alphaBetaSearch();
int maxValue(int depth, int a, int b);
int minValue(int depth, int a, int b);
int utility(int turn);

int main()
{
	printBoard();

	while (!checkFinish()) {

		cout << "Your Turn" << endl;
		cout << "Press the coordinate ex)x y: " << endl;
		int x, y;
		cin >> x >> y;
		board[x][y] = PLAYER1;
		printBoard();
		cout << endl;

		cout << "Computer's Turn" << endl;
		alphaBetaSearch();
		printBoard();
		cout << endl;
	}

}

void printBoard()
{
	for (int i = 0; i < BRDSIZE; i++) {
		printf("%2d	 ", i);
		for (int j = 0; j < BRDSIZE; j++) {
			if (board[i][j] == PLAYER1)
				printf("[%s]", "●");
			else if (board[i][j] == PLAYER2)
				printf("[%s]", "○");
			else
				printf("[  ]");
		}
		printf("\n");
	}
}

bool checkFinish()
{
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[i][j] != 0) {
				for (int d = 0; d < 3; d++) {
					bool flag = true;
					for (int n = 1; n < 5; n++) {
						if (board[i][j] != board[i + dir[d][0] * n][j + dir[d][1] * n]) {
							flag = false;
							break;
						}
					}
					if (flag) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool actionFlag(int y, int x)
{
	if (board[y][x] != 0) { return false; }

	for (int i = y - 1; i < y + 2; i++) {
		for (int j = x - 1; j < x + 2; j++) {
			if (0 <= i && i < 19 && 0 <= j && j < 19) {
				if (board[i][j] != 0) { return true; }
			}
		}
	}
	return false;
}

void alphaBetaSearch()
{
	int x_max = 0;
	int y_max = 0;
	int vMax = 0;
	int tmp = 0;

	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {

			if (actionFlag(i, j)) {
				board[i][j] = PLAYER2;
				tmp = maxValue(0, INT_MIN, INT_MAX);
				board[i][j] = 0;

				if (tmp > vMax) {
					vMax = tmp;
					y_max = i;
					x_max = j;
				}
			}
		}
	}
	board[y_max][x_max] = PLAYER2;
}

int maxValue(int depth, int a, int b)
{
	if (checkFinish()) { return 10000000; }
	if (depth > 2) { return utility(PLAYER1); }	//인간 차례일 때 유틸리티 값 계산

	int v = INT_MIN;
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (actionFlag(i, j)) {
				board[i][j] = PLAYER1;
				v = MAX(v, minValue(depth + 1, a, b));
				board[i][j] = 0;
				if (v >= b) { return v; }
				a = MAX(a, v);
			}
		}
	}

	return v;
}

int minValue(int depth, int a, int b)
{
	if (checkFinish()) { return -10000000; }
	if (depth > 2) { return utility(PLAYER2); }

	int v = INT_MAX;
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (actionFlag(i, j)) {
				board[i][j] = PLAYER2;
				v = MIN(v, maxValue(depth + 1, a, b));
				board[i][j] = 0;
				if (v <= a) { return v; }
				b = MIN(b, v);
			}
		}
	}

	return v;
}

int utility(int turn)
{
	int visit[19][19] = { 0, };
	int sum = 0;
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {

			if (board[i][j] != 0 && visit[i][j] == 0) {
				for (int m = 0; m < 3; m++) {
					int cnt = 1;
					for (int n = 1; n < 4; n++) {
						if ((i + dir[m][0] * n) < 19 && (j + dir[m][1] * n) < 19 && board[i][j] == board[i + dir[m][0] * n][j + dir[m][1] * n]) {
							cnt++;
							visit[i + dir[m][0] * n][j + dir[m][1] * n] = 1;
						}
						else { break; }
					}

					if (cnt == 1) {
						if (turn == PLAYER2 && board[i][j] == PLAYER2) sum += 10;
						else sum += 1;
					}
					else if (cnt == 2) {
						if (turn == PLAYER2 && board[i][j] == PLAYER2) sum += 20;
						else if (turn == PLAYER1 && board[i][j] == PLAYER2) sum += 10;
						else if (turn == PLAYER2 && board[i][j] == PLAYER1) sum += 5;
						else sum += 1;
					}
					else if (cnt == 3) {
						if (turn == PLAYER2 && board[i][j] == PLAYER2) sum += 1000;
						else if (turn == PLAYER1 && board[i][j] == PLAYER2) sum += 500;
						else sum += 1;
					}
					else if (cnt == 4) {
						if (turn == PLAYER1 && board[i][j] == PLAYER1) sum += 1;
						else if (turn == PLAYER2 && board[i][j] == PLAYER2) sum += 100000;
						else if (turn == PLAYER1 && board[i][j] == PLAYER2) sum += 50000;
						else sum += 30000;

					}
				}
				visit[i][j] = 1;
			}
		}
	}

	return sum;
}
