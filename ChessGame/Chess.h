#ifndef __CHESS_HEADER__
#define __CHESS_HEADER__

#include <Windows.h>

#define MAP_SIZE 50
#define MAP_BLOCKCOUNT 8


typedef struct _ChessPiece {
	int type;
	int team;
	HBITMAP *hBitmap;
}ChessPiece;

typedef struct _ChessBlock {
	int bCanMove;
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

#endif