#ifndef __CHESS_HEADER__
#define __CHESS_HEADER__

#include <Windows.h>

#define MAP_SIZE 50
#define MAP_BLOCKCOUNT 8


typedef struct _ChessPiece {
	int type;
	int team;
}ChessPiece;

/*
if cp is NULL,That Block dosen't have any chess piece.
if(bCanMove == TRUE){
	if(cp == NULL){
		this block can have any chess piece.
	}
	else if(cp != NULL){
		that chess piece that is had by block is can be taken by other chess piece.
	}
}
else{
	this block is blocked.
}
*/
typedef struct _ChessBlock {
	int bCanMove[2];
	ChessPiece* cp;
} ChessBlock;

void InitiateChessGame();
void DeleteChessGame();

void AddChessPiece(int x,int y,int type, int team);
void DeleteChessPiece(int x, int y);
void PaintChessPiece(HDC hdc, int sx, int sy);
int SetChessPieceMovement(int x, int y, int type, int team);
int SetBoardMovement(int x, int y, int team);

void PaintChessBoard(HDC hdc, int sx, int sy);
void CancelMoveMode();

//sx and sy are start point of chess board and x and y are clicked point
void ChessBoardMessage(int sx, int sy, int x, int y);
//return the chess piece that was had by destination block.
int MoveChessPiece(int dx, int dy, int fx, int fy, ChessPiece* CPreturn);

int IsCheck();

#endif