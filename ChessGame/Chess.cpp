#include "Chess.h"
#include "resource.h"

extern HINSTANCE g_hInst;
extern HWND hMain;
ChessPiece * Board[8][8];
HBITMAP ChessPieceBitmap[2][8];

void InitiateChessGame() {
	int procedure[8] = { 2, 4, 3, 1, 0, 3, 4, 2 };

	//delete all chess piece at board
	DeleteChessGame();

	//Create Bitmaps of each chesspiece
	for (int i = 0; i < 2; i++) {
		for (int i1 = 0; i1 < 6; i1++){
			ChessPieceBitmap[i][i1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1 + i1 + (i * 10)));
		}
	}

	//setting board to start game
	for (int i = 0; i < 8; i++) {
		AddChessPiece(i, 0, procedure[i], 0);
		AddChessPiece(i,1,5, 0);
		AddChessPiece(i, 7, procedure[i], 1);
		AddChessPiece(i,6,5, 1);
	}

}

void DeleteChessGame() {
	//free all chess piece at board
	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (Board[iy][ix] != NULL) {
				DeleteChessPiece(iy,ix);
			}
		}
	}

	//Delete unnecessary piece bitmap.
	for (int i = 0; i < 2; i++) {
		for (int i1 = 0; i1 < 6; i1++) {
			DeleteObject(ChessPieceBitmap[i][i1]);
		}
	}
}

void AddChessPiece(int x,int y,int type,int team) {
	if (Board[y][x] != NULL) //if this board has anoter chess piece.
		return;

	Board[y][x] = (ChessPiece*)malloc(sizeof(ChessPiece));
	
	Board[y][x]->type = type;
	Board[y][x]->team = team;

	Board[y][x]->hBitmap = &ChessPieceBitmap[team][type];
}

void DeleteChessPiece(int x,int y) {
	if (Board[y][x] == NULL) //if this board already dosen't have chess piece.
		return;

	free(Board[y][x]);
	Board[y][x] = NULL;
}

void PaintChessPiece(HDC hdc,int sx,int sy) {
	HDC MemDC;
	HBITMAP hOldBitmap,hTmpBitmap;

	MemDC = CreateCompatibleDC(hdc);

	hTmpBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1));
	hOldBitmap = (HBITMAP)SelectObject(MemDC, hTmpBitmap);

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (Board[iy][ix] != NULL) {
				SelectObject(MemDC, *Board[iy][ix]->hBitmap);
				TransparentBlt(hdc,sx + (MAP_SIZE * ix) + 1, sy + (MAP_SIZE * iy), 49, 49, MemDC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
		}
	}

	SelectObject(MemDC, hOldBitmap);
	DeleteObject(hTmpBitmap);
	DeleteDC(MemDC);
}

void PaintChessBoard(HDC hdc, int sx, int sy) {
	HBRUSH hMapBs[2], hOldBrush;
	int bMap = 0;

	hMapBs[0] = CreateSolidBrush(RGB(200, 200, 200));
	hMapBs[1] = CreateSolidBrush(RGB(255, 255, 255));

	hOldBrush = (HBRUSH)SelectObject(hdc, hMapBs[0]);	//Get original brush

	for (int iy = sy; iy < sy + (MAP_SIZE * 8); iy += MAP_SIZE) {
		for (int ix = sx; ix < sx + (MAP_SIZE * 8); ix += MAP_SIZE) {
			bMap = !bMap;

			SelectObject(hdc, hMapBs[bMap]);

			Rectangle(hdc, ix, iy, ix + MAP_SIZE, iy + MAP_SIZE);
		}
		bMap = !bMap;
	}

	SelectObject(hdc, hOldBrush);//return brush

	DeleteObject(hMapBs[0]);
	DeleteObject(hMapBs[1]);
}

int bMoveMode, prevX, prevY;

void ChessBoardMessage(int sx, int sy, int x, int y) {
	int px = x - sx, py = y - sy;// setting offset point at 0
	int bx = px / MAP_SIZE, by = py / MAP_SIZE;


	if ((bx < 0 || bx > 7) || (by < 0 || by > 7))	//exit functio when point get out off board area.
		return;
	
	if (bMoveMode == FALSE) {
		if (Board[by][bx] != NULL) {
			int tteam = Board[by][bx]->team, ttype = Board[by][bx]->type;
			WCHAR tMsg[100];

			wsprintf(tMsg, TEXT("%d, %d"), ttype, tteam);

			SetWindowText(hMain, tMsg);
			prevX = bx; prevY = by;
			bMoveMode = TRUE;
		}
	}
	else {
		if (bx == prevX && by == prevY)
			return;

		DeleteChessPiece(bx, by);
		AddChessPiece(bx, by, Board[prevY][prevX]->type, Board[prevY][prevX]->team);
		DeleteChessPiece(prevX, prevY);
		bMoveMode = FALSE;
	}
}