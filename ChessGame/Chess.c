#include "Chess.h"
#include "resource.h"

//if team is opposite, pos go backward
#define FOWARD(pos,to,team) (team ? pos - to : pos + to)
#define IsRightPos(p) (0 <= p && p < MAP_BLOCKCOUNT)
#define GetTeam(x,y) (Board[y][x].cp != NULL ? Board[y][x].cp->team : -1)
#define GetType(x,y) (Board[y][x].cp != NULL ? Board[y][x].cp->type : -1)

extern HINSTANCE g_hInst;
extern HWND hMain;
ChessBlock Board[8][8] = { 0, };
HBITMAP ChessPieceBitmap[2][8];
int bMoveMode,bCheck = -1, prevX = -1, prevY = -1,prevTeam = -1;

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
			DeleteChessPiece(iy,ix);
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
	if (Board[y][x].cp != NULL) //if this board has anoter chess piece.
		return;

	Board[y][x].cp = (ChessPiece*)malloc(sizeof(ChessPiece));
	
	Board[y][x].cp->type = type;
	Board[y][x].cp->team = team;
}

void DeleteChessPiece(int x,int y) {
	if (Board[y][x].cp == NULL) //if this board already dosen't have chess piece.
		return;

	memset(Board[y][x].cp, NULL, sizeof(ChessPiece));
	free(Board[y][x].cp);
	Board[y][x].cp = NULL;
}

void PaintChessPiece(HDC hdc,int sx,int sy) {
	HDC MemDC;
	HBITMAP hOldBitmap,hTmpBitmap;

	MemDC = CreateCompatibleDC(hdc);

	hTmpBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1));
	hOldBitmap = (HBITMAP)SelectObject(MemDC, hTmpBitmap);

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (Board[iy][ix].cp != NULL) {
				SelectObject(MemDC, ChessPieceBitmap[GetTeam(ix,iy)][GetType(ix,iy)]);
				TransparentBlt(hdc,sx + (MAP_SIZE * ix) + 1, sy + (MAP_SIZE * iy), 49, 49, MemDC, 0, 0, 100, 100, RGB(255, 255, 255));
			}
		}
	}

	SelectObject(MemDC, hOldBitmap);
	DeleteObject(hTmpBitmap);
	DeleteDC(MemDC);
}

void PaintChessBoard(HDC hdc, int sx, int sy) {
	HBRUSH hMapBs[3], hOldBrush;
	int bMap = 0;

	hMapBs[0] = CreateSolidBrush(RGB(200, 200, 200));
	hMapBs[1] = CreateSolidBrush(RGB(255, 255, 255));
	hMapBs[2] = CreateSolidBrush(RGB(0, 255, 0));

	hOldBrush = (HBRUSH)SelectObject(hdc, hMapBs[0]);	//Get original brush

	for (int iy = sy; iy < sy + (MAP_SIZE * 8); iy += MAP_SIZE) {
		for (int ix = sx; ix < sx + (MAP_SIZE * 8); ix += MAP_SIZE) {
			int tx = (ix - sx) / MAP_SIZE, ty = (iy - sy) / MAP_SIZE;

			bMap = !bMap;

			if (bMoveMode == TRUE && Board[ty][tx].bCanMove[prevTeam])
				SelectObject(hdc, hMapBs[2]);
			else
				SelectObject(hdc, hMapBs[bMap]);

			Rectangle(hdc, ix, iy, ix + MAP_SIZE, iy + MAP_SIZE);
		}
		bMap = !bMap;
	}

	SelectObject(hdc, hOldBrush);//return brush

	DeleteObject(hMapBs[0]);
	DeleteObject(hMapBs[1]);
	DeleteObject(hMapBs[2]);
}

void ChessBoardMessage(int sx, int sy, int x, int y) {
	int px = x - sx, py = y - sy;// setting offset point at 0
	int bx = px / MAP_SIZE, by = py / MAP_SIZE;
	WCHAR msg[100];

	if ((bx < 0 || bx > 7) || (by < 0 || by > 7))	//exit functio when point get out off board area.
		return;

	wsprintf(msg, TEXT("%d,%d,%d, %d, %d"), bMoveMode,prevX,prevY,bx,by);
	//MessageBox(hMain, msg, TEXT("Confirm"), MB_OK);
	
	if (bMoveMode == FALSE) {
		if (Board[by][bx].cp != NULL) {

			SetChessPieceMovement(bx, by, Board[by][bx].cp->type, Board[by][bx].cp->team);
			

			prevX = bx; prevY = by;
			prevTeam = GetTeam(bx, by);
			bMoveMode = TRUE;
		}
	}
	else {
		int bFinish = 0;

		if (prevX == bx && prevY == by) {
			CancelMoveMode();
			return;
		}

		if (Board[by][bx].bCanMove[prevTeam] == FALSE)
			return;

		if (GetType(bx, by) == 0)
			bFinish = TRUE;

		MoveChessPiece(bx, by, prevX, prevY,NULL);

		if (bCheck = IsCheck(!GetTeam(bx, by)), bCheck == 1) {
			MessageBox(hMain, TEXT("Df"), TEXT("Df"), MB_OK);
			bCheck = 0;
		}

		CancelMoveMode();

		if (bFinish) {
			MessageBox(hMain, TEXT("GameOver"), TEXT("Confirm"), MB_OK);
			if (MessageBox(hMain, TEXT("Do you want one more game?"),TEXT("Confirm"), MB_OKCANCEL) == IDOK) {
				DeleteChessGame();
				InitiateChessGame();
			}
		}
	}
}

int SetBoardMovement(int x, int y, int team) {
	if (IsRightPos(x) && IsRightPos(y)) {
		if (Board[y][x].cp != NULL && Board[y][x].cp->team == team)
			return 0;

		Board[y][x].bCanMove[team] = TRUE;

		if(GetType(x,y) == 0 && GetTeam(x,y) != team)
			return 1;
	}

	return 0;
}

int SetChessPieceMovement(int x, int y, int type, int team) {
	int KnightMove[8][2] = { {2,1},{2,-1},{-2,1},{-2,-1}, {1,2},{1,-2},{-1,2},{-1,-2 } };
	int BishopMove[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
	int RookMove[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
	int rs = 0;

	switch (type) {
	case 0:
		for (int i = 0; i < 4; i++) {
			if (SetBoardMovement(x + BishopMove[i][0], y + BishopMove[i][1], team))
				rs = 1;
		}
		for (int i = 0; i < 4; i++) {
			if (SetBoardMovement(x + RookMove[i][0], y + RookMove[i][1], team))
				rs = 1;
		}
		break;
	case 1://Movement of Queen
		for (int i = 0; i < 4; i++) {
			int tx = x, ty = y;

			while (1) {
				tx += BishopMove[i][0];
				ty += BishopMove[i][1];
				if (!IsRightPos(tx) || !IsRightPos(ty))
					break;
				if (SetBoardMovement(tx, ty, team))
					rs = 1;
				if (GetTeam(tx, ty) != -1)
					break;
			}
		}
		for (int i = 0; i < 4; i++) {
			int tx = x, ty = y;

			while (1) {
				tx += RookMove[i][0];
				ty += RookMove[i][1];
				if (!IsRightPos(tx) || !IsRightPos(ty))
					break;
				if (SetBoardMovement(tx, ty, team))
					rs = 1;
				if (GetTeam(tx, ty) != -1)
					break;
			}
		}
		break;
	case 2://Movement of Rook

		for (int i = 0; i < 4; i++) {
			int tx = x, ty = y;

			while(1){
				tx += RookMove[i][0];
				ty += RookMove[i][1];
				if (!IsRightPos(tx) || !IsRightPos(ty))
					break;
				if (SetBoardMovement(tx, ty, team))
					rs = 1;
				if (GetTeam(tx, ty) != -1)
					break;
			} 
		}

		break;
	case 3://Movement of Bishop
		for (int i = 0; i < 4; i++) {
			int tx = x, ty = y;

			while (1) {
				tx += BishopMove[i][0];
				ty += BishopMove[i][1];
				if (!IsRightPos(tx) || !IsRightPos(ty))
					break;
				if (SetBoardMovement(tx, ty, team))
					rs = 1;
				if (GetTeam(tx, ty) != -1)
					break;
			}
		}

		break;
	case 4://Movement of Knight

		for (int i = 0; i < 8; i++) {
			if (SetBoardMovement(x + KnightMove[i][0], y + KnightMove[i][1], team))
				rs = 1;
		}
		break;
	case 5://Movement of Pawn
		
		if (Board[FOWARD(y, 1, team)][x].cp == NULL) {
			if (SetBoardMovement(x, FOWARD(y, 1, team), team))
				rs = 1;

			if ((y == 1 || y == 6) && Board[FOWARD(y, 2, team)][x].cp == NULL)
				if (SetBoardMovement(x, FOWARD(y, 2, team), team))
					rs = 1;
		}
		
		for (int i = 0; i < 2; i++) {
			int ty = FOWARD(y, 1, team), tx = FOWARD(x, 1, i);
			if (Board[ty][tx].cp != NULL && IsRightPos(tx)) {
				if (SetBoardMovement(tx, ty, team))
					rs = 1;
			}
		}

		break;
	}

	return rs;
}

void CancelMoveMode() {
	AllClearMovement();

	prevX = -1; prevY = -1;
	bMoveMode = FALSE;
}

int IsCheck(int team) {
	int rs = 0;

	AllClearMovement();

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (GetTeam(ix, iy) != team)
				if (SetChessPieceMovement(ix, iy, GetType(ix, iy), GetTeam(ix, iy)))
					rs = 1;
		}
	}

	AllClearMovement();

	return rs;
}

int MoveChessPiece(int dx, int dy, int fx, int fy,ChessPiece * CPreturn) {

	if (Board[fy][fx].cp == NULL)
		return 0;

	if (Board[dy][dx].cp != NULL && CPreturn != NULL)
		*CPreturn = *Board[dy][dx].cp;

	DeleteChessPiece(dx, dy);
	AddChessPiece(dx, dy, GetType(fx, fy), GetTeam(fx, fy));
	DeleteChessPiece(fx, fy);

	return 1;
}

void AllClearMovement() {
	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			Board[iy][ix].bCanMove[0] = FALSE;
			Board[iy][ix].bCanMove[1] = FALSE;
		}
	}
}