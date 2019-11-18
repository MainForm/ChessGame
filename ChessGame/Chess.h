#ifndef __CHESS_HEADER__
#define __CHESS_HEADER__

#include <Windows.h>

void PaintChessBoard(HDC hdc, int sx, int sy);

//sx and sy are start point of chess board and x and y are clicked point
void ChessBoardMessage(int sx, int sy, int x, int y);

#endif