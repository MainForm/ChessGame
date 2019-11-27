#include "Chess.h"
#include "resource.h"

//if team is opposite, pos go backward
#define FOWARD(pos,to,team) (team ? pos - to : pos + to)
#define IsRightPos(p) (0 <= p && p < MAP_BLOCKCOUNT)
#define GetTeam(x,y) (Board[y][x].cp != NULL ? Board[y][x].cp->team : -1)
#define GetType(x,y) (Board[y][x].cp != NULL ? Board[y][x].cp->type : -1)

extern HINSTANCE g_hInst;
extern HWND hMain;

ChessBlock Board[MAP_BLOCKCOUNT][MAP_BLOCKCOUNT] = { 0, };
HBITMAP ChessPieceBitmap[2][8];
Point SelectedPoint = { 0, };
bool bMoveMode = false;
bool TmpMovement[MAP_BLOCKCOUNT][MAP_BLOCKCOUNT] = { 0, };

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

	memset(Board[y][x].cp, 0, sizeof(ChessPiece));
	free(Board[y][x].cp);
	Board[y][x].cp = NULL;
}

int GetChessPiecePoint(ChessPiece* cp, Point* pt) {
	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (Board[iy][ix].cp == cp) {
				pt->x = ix;
				pt->y = iy;
				return 1;
			}
		}
	}
	return 0;
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

			if (bMoveMode && Board[ty][tx].bCanMove[GetTeam(SelectedPoint.x,SelectedPoint.y)])
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

	if ((bx < 0 || bx > 7) || (by < 0 || by > 7))	//exit functio when point get out off board area.
		return;

	if (bMoveMode == false) {
		AllClearMovement();

		if (Board[by][bx].cp == NULL)
			return;

		SelectedPoint.x = bx;
		SelectedPoint.y = by;

		switch (GetType(SelectedPoint.x,SelectedPoint.y)) {
		case 0:
			MovementOfKing(SelectedPoint,IdentifyMovement);
			break;
		case 1:
			MovementOfRook(SelectedPoint, IdentifyMovement);
			MovementOfBishop(SelectedPoint, IdentifyMovement);
			break;
		case 2:
			MovementOfRook(SelectedPoint, IdentifyMovement);
			break;
		case 3:
			MovementOfBishop(SelectedPoint, IdentifyMovement);
			break;
		case 4:
			MovementOfKnight(SelectedPoint, IdentifyMovement);
			break;
		case 5:
			MovementOfPawn(SelectedPoint, IdentifyMovement);
			break;
		}

		for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
			for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
				if (Board[iy][ix].bCanMove[GetTeam(bx,by)] == true) {
					bMoveMode = true;
					return;
				}
			}
		}
	}
	else {
		//Select same point.
		if (SelectedPoint.x == bx && SelectedPoint.y == by) {
			bMoveMode = false;
			return;
		}

		if (Board[by][bx].bCanMove[GetTeam(SelectedPoint.x,SelectedPoint.y)] == 0)
			return;
		
		MoveChessPiece(bx, by, SelectedPoint.x, SelectedPoint.y, NULL);

		if (IsCheck(!GetTeam(bx,by))) {
			MessageBox(hMain, TEXT("Test"), TEXT("Test"), MB_OK);
		}

		bMoveMode = false;
	}
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

void MovementOfKing(Point pt, IdentifyFunc ptFunc) {

	for (int iy = pt.y - 1; iy < pt.y + 2; iy++) {
		for (int ix = pt.x - 1; ix < pt.x + 2; ix++) {
			if (ix == pt.x && iy == pt.y)
				continue;
			ptFunc(ix, iy, pt);
		}
	}
}

void MovementOfRook(Point pt, IdentifyFunc ptFunc) {
	int ways[4][2] = {
		{1,0},{-1,0},{0,-1},{0,1}
	};

	for (int i = 0; i < 4; i++) {
		Point ptTmp = { pt.x,pt.y };

		ptTmp.x += ways[i][0];
		ptTmp.y += ways[i][1];

		while (ptFunc(ptTmp.x, ptTmp.y, pt)) {
			if (Board[ptTmp.y][ptTmp.x].cp != NULL)
				break;
			ptTmp.x += ways[i][0];
			ptTmp.y += ways[i][1];
		}
	}

}

void MovementOfBishop(Point pt, IdentifyFunc ptFunc) {
	int ways[4][2] = {
		{1,1},{-1,1},{1,-1},{-1,-1}
	};

	for (int i = 0; i < 4; i++) {
		Point ptTmp = { pt.x,pt.y };

		ptTmp.x += ways[i][0];
		ptTmp.y += ways[i][1];

		while (ptFunc(ptTmp.x, ptTmp.y, pt)) {
			if (Board[ptTmp.y][ptTmp.x].cp != NULL)
				break;
			ptTmp.x += ways[i][0];
			ptTmp.y += ways[i][1];
		}
	}

}

void MovementOfKnight(Point pt, IdentifyFunc ptFunc) {
	int ways[8][2] = {
	{2,1},{-2,1},{2,-1},{-2,-1},
	{1,2},{-1,2},{1,-2},{-1,-2}
	};

	for (int i = 0; i < 8; i++) {
		ptFunc(pt.x + ways[i][0], pt.y + ways[i][1], pt);
	}
}

void MovementOfPawn(Point pt, IdentifyFunc ptFunc) {
	int team = GetTeam(pt.x, pt.y);

	ptFunc(pt.x, FOWARD(pt.y, 1, team), pt);
	if(pt.y == 1 || pt.y == 6)
		ptFunc(pt.x, FOWARD(pt.y, 2, team), pt);

	for (int i = 0; i < 2; i++) {
		if (Board[FOWARD(pt.y,1,team)][FOWARD(pt.x, 1, i)].cp != NULL) {
			ptFunc(FOWARD(pt.x, 1, i), FOWARD(pt.y, 1, team), pt);
		}
	}
}

int IdentifyMovement(int x, int y, Point pt) {
	ChessPiece CPtmp = { -1,-1 };

	if (!IsRightPos(x) || !IsRightPos(y))
		return 0;

	if (Board[y][x].cp != NULL && Board[y][x].cp->team == GetTeam(pt.x,pt.y))
		return 0;

	
	MoveChessPiece(x, y, pt.x, pt.y, &CPtmp);

	if (IsCheck(GetTeam(x, y))) {
		MoveChessPiece(pt.x, pt.y, x, y, &CPtmp);
		if (CPtmp.team != -1 || CPtmp.type != -1)
			AddChessPiece(x, y, CPtmp.type, CPtmp.team);
		return 0;
	}

	MoveChessPiece(pt.x, pt.y, x, y, &CPtmp);
	Board[y][x].bCanMove[GetTeam(pt.x, pt.y)] = true;
	
	if (CPtmp.team != -1 || CPtmp.type != -1)
		AddChessPiece(x, y, CPtmp.type, CPtmp.team);

	
	return 1;
}

int IdentifyCheck(int x, int y, Point pt) {
	if (!IsRightPos(x) || !IsRightPos(y))
		return 0;

	if (Board[y][x].cp != NULL && Board[y][x].cp->team == GetTeam(pt.x, pt.y))
		return 0;


	TmpMovement[y][x] = true;

	return 1;
}

bool IsCheck(int team) {
	Point pt = { 0, };

	if (team == -1)
		return false;

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			TmpMovement[iy][ix] = false;
		}
	}

	for (pt.y = 0; pt.y < MAP_BLOCKCOUNT; pt.y++) {
		for (pt.x = 0; pt.x < MAP_BLOCKCOUNT; pt.x++) {
			if (Board[pt.y][pt.x].cp != NULL && GetTeam(pt.x, pt.y) != team) {
				switch (GetType(pt.x, pt.y)) {
				case 0:
					MovementOfKing(pt, IdentifyCheck);
					break;
				case 1:
					MovementOfRook(pt, IdentifyCheck);
					MovementOfBishop(pt, IdentifyCheck);
					break;
				case 2:
					MovementOfRook(pt, IdentifyCheck);
					break;
				case 3:
					MovementOfBishop(pt, IdentifyCheck);
					break;
				case 4:
					MovementOfKnight(pt, IdentifyCheck);
					break;
				case 5:
					MovementOfPawn(pt, IdentifyCheck);
					break;
				}
			}
		}
	}

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (TmpMovement[iy][ix] && Board[iy][ix].cp != NULL && GetType(ix, iy) == 0 && GetTeam(ix, iy) == team)
				return 1;
		}
	}

	return 0;
}