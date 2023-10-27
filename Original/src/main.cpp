// ChessAI by Nicholas Wulf ////////////////////////////////////////////////////////////////////////////////////
// Uses the max/min algorithm to simulate chess AI                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdarg.h>
#include <cstdlib.>
#include <iostream>
#include <sstream>
#include <cmath>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

// Definitions
#define CPU1 false  // true if player 1 is cpu
#define CPU2 true  // true if player 2 is cpu

// Global Variables
int board[8][8];  // 0=empty, 1=pawn, 2=horse, 3=bishop, 4=castle, 5=queen, 6=king, (+)=white, (-)=black

// Functions
void doTurn(bool white, bool human, int evalType);
void decideWhite(int level, int evalType, int &startX, int &startY, int &endX, int &endY, int &score);
void decideBlack(int level, int evalType, int &startX, int &startY, int &endX, int &endY, int &score);
void decidePlayer(bool white, int &startX, int &startY, int &endX, int &endY);
void findPawnMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum);
void findHorseMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum);
void findBishopMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum);
void findCastleMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum);
void findQueenMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum);
void findKingMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum);
int evaluateBoard1();
void printBoard();
void initializeBoard();

int main(int argc, char **argv) {
	srand(time(NULL));
	initializeBoard();
	string temp;
	while (true) {
		doTurn(true,!CPU1,1);
		if (CPU1 && CPU2) cin >> temp;
		doTurn(false,!CPU2,1);
		if (CPU1 && CPU2) cin >> temp;
	}
}

void doTurn(bool white, bool human, int evalType) {
	int startX, startY, endX, endY, score=0;
	int piece;
	if (human) decidePlayer(white,startX,startY,endX,endY);
	else {
		if (white) decideWhite(4,evalType,startX,startY,endX,endY,score);
		else decideBlack(4,evalType,startX,startY,endX,endY,score);
	}
	piece = board[startX][startY];
	board[startX][startY] = 0;
	board[endX][endY] = piece;
	printBoard();
	if (white) cout << "\n White moves ";
	else cout << "\n Black moves ";
	switch (abs(piece)) {
		case 1: cout << "pawn"; break;
		case 2: cout << "horse"; break;
		case 3: cout << "bishop"; break;
		case 4: cout << "castle"; break;
		case 5: cout << "queen"; break;
		case 6: cout << "king"; break;
		default: cout << "ERROR";
	}
	printf(" from (%i,%i) to (%i,%i) for score of %i\n",startX,startY,endX,endY,score);
}

void decideWhite(int level, int evalType, int &startX, int &startY, int &endX, int &endY, int &score) {
	int piece, victim, newX, newY;
	int dummy1, dummy2, dummy3, dummy4;
	int moveSet[30][2];
	int moveNum = 0;
	float battles = 1;  // Used to remember how many other scores have been equal to the current
	int newScore;
	score = -10000;
	bool young = true;  // While true, no piece has been found yet
	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {
			piece = board[x][y];
			if (piece > 0) {  // If the piece is white
				switch (piece) {
					case 1: findPawnMoves(true,x,y,moveSet,moveNum); break;
					case 2: findHorseMoves(true,x,y,moveSet,moveNum); break;
					case 3: findBishopMoves(true,x,y,moveSet,moveNum); break;
					case 4: findCastleMoves(true,x,y,moveSet,moveNum); break;
					case 5: findQueenMoves(true,x,y,moveSet,moveNum); break;
					case 6: findKingMoves(true,x,y,moveSet,moveNum); break;
					default:;
				}
				for (int i=0; i<moveNum; i++) {
					newX = moveSet[i][0];
					newY = moveSet[i][1];
					victim = board[newX][newY];  // Remember what used to be in new location
					board[x][y] = 0;  // Remove the piece from its current location
					board[newX][newY] = piece;  // Place the piece in the new location
					if (level <= 1 || victim == -6) {
						switch (evalType) {
							case 1: newScore = evaluateBoard1(); break;
							default: newScore = 0;
						}
					}
					else {
						decideBlack(level-1,evalType,dummy1,dummy2,dummy3,dummy4,newScore);
					}
					board[x][y] = piece;  // Replace the piece to its old location
					board[newX][newY] = victim;  // Restore the victim to its location
					bool tieWinner = false;
					if (newScore == score) {
						if (rand()%1000 > (battles*1000)/(battles+1)) {  // Makes sure that each equal score 
							tieWinner = true;                               //   has equal chance to be chosen
						}
						battles++;
					}
					if (newScore > score || tieWinner || young == true) {  // If the new move was better then
						young = false;                                     //   record its score and moves
						score = newScore;
						startX = x;
						startY = y;
						endX = newX;
						endY = newY;
						if (!tieWinner) battles = 1;
					}
				}
			}
		}
	}
}

void decideBlack(int level, int evalType, int &startX, int &startY, int &endX, int &endY, int &score) {
	int piece, victim, newX, newY;
	int dummy1, dummy2, dummy3, dummy4;
	int moveSet[30][2];
	int moveNum = 0;
	float battles = 1;  // Used to remember how many other scores have been equal to the current
	int newScore;
	score = 10000;
	bool young = true;  // While true, no piece has been found yet
	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {
			piece = board[x][y];
			if (piece < 0) {  // If the piece is black
				switch (piece) {
					case -1: findPawnMoves(false,x,y,moveSet,moveNum); break;
					case -2: findHorseMoves(false,x,y,moveSet,moveNum); break;
					case -3: findBishopMoves(false,x,y,moveSet,moveNum); break;
					case -4: findCastleMoves(false,x,y,moveSet,moveNum); break;
					case -5: findQueenMoves(false,x,y,moveSet,moveNum); break;
					case -6: findKingMoves(false,x,y,moveSet,moveNum); break;
					default:;
				}
				for (int i=0; i<moveNum; i++) {
					newX = moveSet[i][0];
					newY = moveSet[i][1];
					victim = board[newX][newY];  // Remember what used to be in new location
					board[x][y] = 0;  // Remove the piece from its current location
					board[newX][newY] = piece;  // Place the piece in the new location
					if (level <= 1 || victim == 6) {
						switch (evalType) {
							case 1: newScore = evaluateBoard1(); break;
							default: newScore = 0;
						}
					}
					else {
						decideWhite(level-1,evalType,dummy1,dummy2,dummy3,dummy4,newScore);
					}
					board[x][y] = piece;  // Replace the piece to its old location
					board[newX][newY] = victim;  // Restore the victim to its location
					bool tieWinner = false;
					if (newScore == score) {
						if ((rand()%100) > (battles*100)/(battles+1)) {  // Makes sure that each equal score 
							tieWinner = true;                               //   has equal chance to be chosen
						}
						battles++;
					}
					if (newScore < score || tieWinner || young == true) {  // If the new move was better then
						young = false;                                     //   record its score and moves
						score = newScore;
						startX = x;
						startY = y;
						endX = newX;
						endY = newY;
						if (!tieWinner) battles = 1;
					}
				}
			}
		}
	}
}

void decidePlayer(bool white, int &startX, int &startY, int &endX, int &endY) {
	int piece;
	int moveSet[30][2];
	int moveNum;
	string sTemp;
	bool cancel = true;
	while (cancel) {
		bool go = false;
		while (!go) {
			cout << "\n Type the x,y coordinate of the position you would like to move from: ";
			cin >> sTemp;
			if (isdigit(sTemp[0]) && sTemp[1] == ',' && isdigit(sTemp[2]) && int(sTemp[3]) == 0) {
				startX = int(sTemp[0]-48);
				startY = int(sTemp[2]-48);
				if (startX < 8 && startY < 8) {
					piece = board[startX][startY];
					if ((piece > 0 && white) || (piece < 0 && !white)) go = true;
					else {
						if (piece == 0) cout << "  Invalid entry: Selected postion is empty!\n";
						else cout << "  Invalid entry: Selected position contains enemy piece!\n";
					}
				}
				else cout << "  Invalid entry: Coordinates are out of bounds!\n";
			}
			else cout << "  Invalid entry: Type \"#,#\" with no spaces!\n";
		}
		switch (abs(piece)) {
			case 1: findPawnMoves(white,startX,startY,moveSet,moveNum); break;
			case 2: findHorseMoves(white,startX,startY,moveSet,moveNum); break;
			case 3: findBishopMoves(white,startX,startY,moveSet,moveNum); break;
			case 4: findCastleMoves(white,startX,startY,moveSet,moveNum); break;
			case 5: findQueenMoves(white,startX,startY,moveSet,moveNum); break;
			case 6: findKingMoves(white,startX,startY,moveSet,moveNum); break;
			default:;
		}
		go = false;
		while (!go) {
			cout << "\n Type the x,y coordinate of the position you would like to move to: ";
			cin >> sTemp;
			if (sTemp[0] == 'c' && int(sTemp[1] == 0)) {
				go = true;
				cout << "  Canceled\n";
			}
			else if (isdigit(sTemp[0]) && sTemp[1] == ',' && isdigit(sTemp[2]) && int(sTemp[3]) == 0) {
				endX = int(sTemp[0]-48);
				endY = int(sTemp[2]-48);
				if (endX < 8 && endY < 8) {
					for (int i=0; i < moveNum; i++) {
						if (moveSet[i][0] == endX && moveSet[i][1] == endY) {
							go = true;
							cancel = false;
						}
					}
					if (!go) cout << "  Invalid entry: Cannot move there!\n";
				}
				else cout << "  Invalid entry: Coordinates are out of bounds!\n";
			}
			else cout << "  Invalid entry: Type \"#,#\" with no spaces!\n";
		}
	}
}

void findPawnMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum) {
	moveNum = 0;
	if (white) {
		if (posY < 7) {  // Make sure the pawn can still go further
			if (board[posX][posY+1] == 0) {  // Is it empty in front
				moveSet[moveNum][0] = posX;
				moveSet[moveNum][1] = posY+1;
				moveNum++;
			}
			if (posX > 0) {  // Make sure the pawn has room on left
				if (board[posX-1][posY+1] < 0) {  // Is there an enemy to the upper left
					moveSet[moveNum][0] = posX-1;
					moveSet[moveNum][1] = posY+1;
					moveNum++;
				}
			}
			if (posX < 7) {  // Make sure the pawn has room on right
				if (board[posX+1][posY+1] < 0) {  // Is there an enemy to the upper right
					moveSet[moveNum][0] = posX+1;
					moveSet[moveNum][1] = posY+1;
					moveNum++;
				}
			}
			if (posY == 1) {
				if (board[posX][2] == 0 && board[posX][3] == 0) {  // Can the pawn jump two spaces
					moveSet[moveNum][0] = posX;
					moveSet[moveNum][1] = 3;
					moveNum++;
				}
			}
		}
	}
	else {
		if (posY > 0) {  // Make sure the pawn can still go further
			if (board[posX][posY-1] == 0) {  // Is it empty in front
				moveSet[moveNum][0] = posX;
				moveSet[moveNum][1] = posY-1;
				moveNum++;
			}
			if (posX > 0) {  // Make sure the pawn has room on left
				if (board[posX-1][posY-1] > 0) {  // Is there an enemy to the lower left
					moveSet[moveNum][0] = posX-1;
					moveSet[moveNum][1] = posY-1;
					moveNum++;
				}
			}
			if (posX < 7) {  // Make sure the pawn has room on right
				if (board[posX+1][posY-1] > 0) {  // Is there an enemy to the lower right
					moveSet[moveNum][0] = posX+1;
					moveSet[moveNum][1] = posY-1;
					moveNum++;
				}
			}
			if (posY == 6) {
				if (board[posX][5] == 0 && board[posX][4] == 0) {  // Can the pawn jump two spaces
					moveSet[moveNum][0] = posX;
					moveSet[moveNum][1] = 4;
					moveNum++;
				}
			}
		}
	}
}

void findHorseMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum) {
	moveNum = 0;
	if (posX+1 <= 7 && posY+2 <= 7) {  // Make sure 1 o'clock is on board
		if ((board[posX+1][posY+2] <= 0 && white) || (board[posX+1][posY+2] >= 0 && !white)) {  // Is non-ally at 1 o'clock
			moveSet[moveNum][0] = posX+1;
			moveSet[moveNum][1] = posY+2;
			moveNum++;
		}
	}
	if (posX+2 <= 7 && posY+1 <= 7) {  // Make sure 2 o'clock is on board
		if ((board[posX+2][posY+1] <= 0 && white) || (board[posX+2][posY+1] >= 0 && !white)) {  // Is non-ally at 2 o'clock
			moveSet[moveNum][0] = posX+2;
			moveSet[moveNum][1] = posY+1;
			moveNum++;
		}
	}
	if (posX+2 <= 7 && posY-1 >= 0) {  // Make sure 4 o'clock is on board
		if ((board[posX+2][posY-1] <= 0 && white) || (board[posX+2][posY-1] >= 0 && !white)) {  // Is non-ally at 4 o'clock
			moveSet[moveNum][0] = posX+2;
			moveSet[moveNum][1] = posY-1;
			moveNum++;
		}
	}
	if (posX+1 <= 7 && posY-2 >= 0) {  // Make sure 5 o'clock is on board
		if ((board[posX+1][posY-2] <= 0 && white) || (board[posX+1][posY-2] >= 0 && !white)) {  // Is non-ally at 5 o'clock
			moveSet[moveNum][0] = posX+1;
			moveSet[moveNum][1] = posY-2;
			moveNum++;
		}
	}
	if (posX-1 >= 0 && posY-2 >= 0) {  // Make sure 7 o'clock is on board
		if ((board[posX-1][posY-2] <= 0 && white) || (board[posX-1][posY-2] >= 0 && !white)) {  // Is non-ally at 7 o'clock
			moveSet[moveNum][0] = posX-1;
			moveSet[moveNum][1] = posY-2;
			moveNum++;
		}
	}
	if (posX-2 >= 0 && posY-1 >= 0) {  // Make sure 8 o'clock is on board
		if ((board[posX-2][posY-1] <= 0 && white) || (board[posX-2][posY-1] >= 0 && !white)) {  // Is non-ally at 8 o'clock
			moveSet[moveNum][0] = posX-2;
			moveSet[moveNum][1] = posY-1;
			moveNum++;
		}
	}
	if (posX-2 >= 0 && posY+1 <= 7) {  // Make sure 10 o'clock is on board
		if ((board[posX-2][posY+1] <= 0 && white) || (board[posX-2][posY+1] >= 0 && !white)) {  // Is non-ally at 10 o'clock
			moveSet[moveNum][0] = posX-2;
			moveSet[moveNum][1] = posY+1;
			moveNum++;
		}
	}
	if (posX-1 >= 0 && posY+2 <= 7) {  // Make sure 11 o'clock is on board
		if ((board[posX-1][posY+2] <= 0 && white) || (board[posX-1][posY+2] >= 0 && !white)) {  // Is non-ally at 11 o'clock
			moveSet[moveNum][0] = posX-1;
			moveSet[moveNum][1] = posY+2;
			moveNum++;
		}
	}
}

void findBishopMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum) {
	int testX;
	int testY;
	int piece;
	moveNum = 0;
	testX = posX+1;
	testY = posY+1;  // Move to the upper right diagonal
	while (testX <= 7 && testY <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX++;    //   move to the next upper right square
			testY++;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX+1;
	testY = posY-1;  // Move to the lower right diagonal
	while (testX <= 7 && testY >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX++;    //   move to the next lower right square
			testY--;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX-1;
	testY = posY-1;  // Move to the lower left diagonal
	while (testX >= 0 && testY >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX--;    //   move to the next lower left square
			testY--;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX-1;
	testY = posY+1;  // Move to the upper left diagonal
	while (testX >= 0 && testY <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX--;    //   move to the next lower left square
			testY++;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
}

void findCastleMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum) {
	int testX;
	int testY;
	int piece;
	moveNum = 0;
	testX = posX;
	testY = posY+1;  // Move up
	while (testY <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testY++;    //   move to the next upper square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX+1;
	testY = posY;  // Move right
	while (testX <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX++;    //   move to the next right square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX;
	testY = posY-1;  // Move down
	while (testY >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testY--;    //   move to the next lower square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX-1;
	testY = posY;  // Move left
	while (testX >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX--;    //   move to the next left square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
}

void findQueenMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum) {
		int testX;
	int testY;
	int piece;
	moveNum = 0;
	testX = posX;
	testY = posY+1;  // Move up
	while (testY <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testY++;    //   move to the next upper square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX+1;
	testY = posY+1;  // Move to the upper right diagonal
	while (testX <= 7 && testY <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX++;    //   move to the next upper right square
			testY++;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX+1;
	testY = posY;  // Move right
	while (testX <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX++;    //   move to the next right square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX+1;
	testY = posY-1;  // Move to the lower right diagonal
	while (testX <= 7 && testY >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX++;    //   move to the next lower right square
			testY--;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX;
	testY = posY-1;  // Move down
	while (testY >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testY--;    //   move to the next lower square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX-1;
	testY = posY-1;  // Move to the lower left diagonal
	while (testX >= 0 && testY >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX--;    //   move to the next lower left square
			testY--;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX-1;
	testY = posY;  // Move left
	while (testX >= 0) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX--;    //   move to the next left square
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
	testX = posX-1;
	testY = posY+1;  // Move to the upper left diagonal
	while (testX >= 0 && testY <= 7) {  // Make sure the test spot is still on the board
		piece = board[testX][testY];
		if (piece == 0) {  // Is the test spot empty
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there and
			testX--;    //   move to the next lower left square
			testY++;
		}
		else if ((piece < 0 && white) || (piece > 0 && !white)) {  // Is there an enemy
			moveSet[moveNum][0] = testX;
			moveSet[moveNum][1] = testY;
			moveNum++;  // Then it is ok to move there but
			break;      //   can't move on
		}
		else {  // Is there an ally
			break;  // Then not ok to move there or move on
		}
	}
}

void findKingMoves(bool white, int posX, int posY, int moveSet[][2], int &moveNum) {
	moveNum = 0;
	if (posY+1 <= 7) {  // Test upper spot for non-ally
		if ((board[posX][posY+1] <= 0 && white) || (board[posX][posY+1] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX;
			moveSet[moveNum][1] = posY+1;
			moveNum++;
		}
	}
	if (posX+1 <=7 && posY+1 <= 7) {  // Test upper right spot for non-ally
		if ((board[posX+1][posY+1] <= 0 && white) || (board[posX+1][posY+1] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX+1;
			moveSet[moveNum][1] = posY+1;
			moveNum++;
		}
	}
	if (posX+1 <= 7) {  // Test right spot for non-ally
		if ((board[posX+1][posY] <= 0 && white) || (board[posX+1][posY] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX+1;
			moveSet[moveNum][1] = posY;
			moveNum++;
		}
	}
	if (posX+1 <=7 && posY-1 >= 0) {  // Test lower right spot for non-ally
		if ((board[posX+1][posY-1] <= 0 && white) || (board[posX+1][posY-1] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX+1;
			moveSet[moveNum][1] = posY-1;
			moveNum++;
		}
	}
	if (posY-1 >= 0) {  // Test lower spot for non-ally
		if ((board[posX][posY-1] <= 0 && white) || (board[posX][posY-1] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX;
			moveSet[moveNum][1] = posY-1;
			moveNum++;
		}
	}
	if (posX-1 >=0 && posY-1 >= 0) {  // Test lower left spot for non-ally
		if ((board[posX-1][posY-1] <= 0 && white) || (board[posX-1][posY-1] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX-1;
			moveSet[moveNum][1] = posY-1;
			moveNum++;
		}
	}
	if (posX-1 >= 0) {  // Test left spot for non-ally
		if ((board[posX-1][posY] <= 0 && white) || (board[posX-1][posY] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX-1;
			moveSet[moveNum][1] = posY;
			moveNum++;
		}
	}
	if (posX-1 >=0 && posY+1 <= 7) {  // Test upper left spot for non-ally
		if ((board[posX-1][posY+1] <= 0 && white) || (board[posX-1][posY+1] >= 0 && !white)) {  
			moveSet[moveNum][0] = posX-1;
			moveSet[moveNum][1] = posY+1;
			moveNum++;
		}
	}
}

int evaluateBoard1() {
	int score = 0;
	int change = 0;
	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {
			switch (abs(board[x][y])) {
				case 1: change = 10; break;
				case 2: change = 20; break;
				case 3: change = 25; break;
				case 4: change = 30; break;
				case 5: change = 50; break;
				case 6: change = 10000; break;
				default: change = 0;
			}
			if (board[x][y] < 0) {
				score -= change;
			}
			else {
				score += change;
			}
		}
	}
	return score;
}

void printBoard() {
	char c;
	cout << "\n";
	for (int y=7; y>=0; y--) {
		printf("  %i>", y);
		for (int x=0; x<8; x++) {
			switch (board[x][y]) {
				case -6: c = 'k'; break;
				case -5: c = 'q'; break;
				case -4: c = 'c'; break;
				case -3: c = 'b'; break;
				case -2: c = 'h'; break;
				case -1: c = 'p'; break;
				case 1: c = 'P'; break;
				case 2: c = 'H'; break;
				case 3: c = 'B'; break;
				case 4: c = 'C'; break;
				case 5: c = 'Q'; break;
				case 6: c = 'K'; break;
				default: c = '-';
			}
			cout << "  " << c;
		}
		cout << "\n\n";
	}
	cout << "      ^  ^  ^  ^  ^  ^  ^  ^\n";
	cout << "      0  1  2  3  4  5  6  7\n";
}

void initializeBoard() {
	int temp[8][8] = {
        
		{-4,-2,-3,-5,-6,-3,-2,-4},
		{-1,-1,-1,-1,-1,-1,-1,-1},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 1, 1, 1, 1, 1, 1, 1, 1},
		{ 4, 2, 3, 5, 6, 3, 2, 4}};
		/*
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0,-1, 0, 0, 0, 0, 0},
		{ 0, 0, 0,-1, 0,-1, 0, 0},
		{ 0, 0, 0, 0, 1, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0}};
		*/
	for (int y=0; y<8; y++) {
		for (int x=0; x<8; x++) {
			board[x][y] = temp[7-y][x];
		}
	}
}
