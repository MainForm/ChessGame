#include "Chess.h"
#include "resource.h"

extern HINSTANCE g_hInst;
ChessPiece * Board[8][8];
HBITMAP ChessPieceBitmap[2][8];

void InitiateChessGame() {
	DeleteChessGame();

	//Create Bitmaps of each chesspiece
	for (int i = 0; i < 2; i++) {
		for (int i1 = 0; i1 < 6; i1++){
			ChessPieceBitmap[i][i1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1 + i1 + (i * 10)));
		}
	}
	
	for (int i = 0; i < 6; i++) {
		AddChessPiece(&Board[0][i],i, 0);
	}
	for (int i = 0; i < 6; i++) {
		AddChessPiece(&Board[1][i],i, 1);
	}
}

void DeleteChessGame() {
	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (Board[iy][ix] != NULL) {
				DeleteChessPiece(&Board[iy][ix]);
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int i1 = 0; i1 < 6; i1++) {
			DeleteObject(ChessPieceBitmap[i][i1]);
		}
	}
}

void AddChessPiece(ChessPiece ** cp,int type,int team) {
	if (*cp != NULL)
		return;

	*cp = (ChessPiece*)malloc(sizeof(ChessPiece));
	
	(*cp)->type = type;
	(*cp)->team = team;

	(*cp)->hBitmap = &ChessPieceBitmap[team][type];
}

void DeleteChessPiece(ChessPiece** cp) {
	free(*cp);
	*cp = NULL;
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

void ChessBoardMessage(int sx, int sy, int x, int y) {
	int px = x - sx, py = y - sy;// setting offset point at 0
	int bx = px / MAP_SIZE, by = py / MAP_SIZE;
	WCHAR chMessage[100]; 


	if ((bx < 0 || bx > 7) || (by < 0 || by > 7))	//exit functio when point get out off board area.
		return;
	
	if (Board[by][bx] != NULL) {
		wsprintf(chMessage, TEXT("%d,%d"), Board[by][bx]->team, Board[by][bx]->type);

		MessageBox(NULL, chMessage, TEXT("Ȯ��"), MB_OK);
	}
}