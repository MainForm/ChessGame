#ifndef __CHESS_HEADER__
#define __CHESS_HEADER__

#include <Windows.h>
#include <stdbool.h>

#define MAP_SIZE 50
#define MAP_BLOCKCOUNT 8

typedef struct _Point {
	int x;
	int y;
}Point;

typedef int (*IdentifyFunc)(int, int, Point);

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
	bool bCanMove[2];
	ChessPiece* cp;
} ChessBlock;

void InitiateChessGame();
void DeleteChessGame();

void AddChessPiece(int x,int y,int type, int team);
void DeleteChessPiece(int x, int y);

int GetChessPiecePoint(ChessPiece* cp, Point* pt);

void PaintChessPiece(HDC hdc, int sx, int sy);
void PaintChessBoard(HDC hdc, int sx, int sy);

//sx and sy are start point of chess board and x and y are clicked point
void ChessBoardMessage(int sx, int sy, int x, int y);
/*
Flow of ChessBoardMessage
1. Check whether move mode is true or false.
if move mode is false.
	2. Save the selected point and chess piece.
	3. Check that king of selected chess piece team is check.
	if that king is check
		4. exit this function
	3. Delete the selected chess piece.
	4. Identify the way that selected chess piece can go.
	when you Identify the way.
	you need to check the way that chess piece can go.
	for example,block is blank or other team chess piece is exist.
*/

//return the chess piece that was had by destination block.
int MoveChessPiece(int dx, int dy, int fx, int fy, ChessPiece* CPreturn);

void AllClearMovement();

int IdentifyMovement(int x, int y, Point pt);

void MovementOfKing(Point pt,IdentifyFunc ptFunc);
void MovementOfRook(Point pt, IdentifyFunc ptFunc);
void MovementOfBishop(Point pt, IdentifyFunc ptFunc);
void MovementOfKnight(Point pt, IdentifyFunc ptFunc);
void MovementOfPawn(Point pt, IdentifyFunc ptFunc);

bool IsCheck(int team);
int IdentifyCheck(int x, int y, Point pt);

#endif