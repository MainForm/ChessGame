#ifndef __CHESS_HEADER__
#define __CHESS_HEADER__

#include <Windows.h>

#define MAP_SIZE 50
#define MAP_BLOCKCOUNT 8


typedef struct _ChessPiece {
	int type;
	int team;
	HBITMAP hBitmap;
}ChessPiece;

void InitiateChessGame(ChessPiece Board[][MAP_BLOCKCOUNT]);
void DeleteChessGame(ChessPiece Board[][MAP_BLOCKCOUNT]);

void AddChessPiece(ChessPiece* cp, int type, int team);
void PaintChessPiece(HDC hdc, ChessPiece Board[][MAP_BLOCKCOUNT], int sx, int sy);

void PaintChessBoard(HDC hdc, int sx, int sy);

//sx and sy are start point of chess board and x and y are clicked point
void ChessBoardMessage(int sx, int sy, int x, int y);

#endif