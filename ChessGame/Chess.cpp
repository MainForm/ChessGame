#include "Chess.h"
#include "resource.h"

extern HINSTANCE g_hInst;

void InitiateChessGame(ChessPiece  Board[][MAP_BLOCKCOUNT]) {
	DeleteChessGame(Board);

	for (int i = 0; i < 6; i++) {
		AddChessPiece(&Board[0][i], i, 0);
	}
	for (int i = 0; i < 6; i++) {
		AddChessPiece(&Board[1][i], i, 1);
	}
}

void DeleteChessGame(ChessPiece Board[][MAP_BLOCKCOUNT]) {
	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			DeleteObject(Board[iy][ix].hBitmap);

			memset(&Board[iy][ix], NULL, sizeof(ChessPiece));
		}
	}
}

void AddChessPiece(ChessPiece* cp, int type,int team) {
	cp->type = type;
	cp->team = team;

	cp->hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1 + type + (10 * team)));
}

void PaintChessPiece(HDC hdc, ChessPiece Board[][MAP_BLOCKCOUNT],int sx,int sy) {
	HDC MemDC;
	HBITMAP hOldBitmap,hTmpBitmap;

	MemDC = CreateCompatibleDC(hdc);

	hTmpBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1));
	hOldBitmap = (HBITMAP)SelectObject(MemDC, hTmpBitmap);

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (Board[iy][ix].hBitmap != NULL) {
				SelectObject(MemDC, Board[iy][ix].hBitmap);
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

void ChessBoardMessage(int sx, int sy, int x, int y) {
	int px = x - sx, py = y - sy;// setting offset point at 0
	WCHAR cx = 'A' + (px / MAP_SIZE), cy = '8' - (py / MAP_SIZE);// each block address

	if ((cx < 'A' || cx > 'H') || (cy < '1' || cy > '8'))	//exit functio when point get out off board area.
		return;


}