#include "Chess.h"
#include "resource.h"

//if team is opposite, pos go backward
#define FOWARD(pos,to,team) (team ? pos - to : pos + to)
#define IsRightPos(p) (0 <= p && p < MAP_BLOCKCOUNT)
#define GetTeam(x,y) (Board[y][x].cp != NULL ? Board[y][x].cp->team : -1)
#define GetType(x,y) (Board[y][x].cp != NULL ? Board[y][x].cp->type : -1)

extern BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

extern HINSTANCE g_hInst;
extern HWND hMain;

ChessBlock Board[MAP_BLOCKCOUNT][MAP_BLOCKCOUNT] = { 0, };
HBITMAP ChessPieceBitmap[2][6];
Point SelectedPoint = { 0, };
bool bMoveMode = false;
bool TmpMovement[MAP_BLOCKCOUNT][MAP_BLOCKCOUNT] = { 0, };
int turn = 1;
bool bEnPassant = false;
bool bCastling[2][2] = { {true, true},{true,true} };

void InitiateChessGame() {
	int procedure[8] = { CHESS_ROOK, CHESS_KNIGHT, CHESS_BISHOP, CHESS_QUEEN, CHESS_KING, CHESS_BISHOP, CHESS_KNIGHT, CHESS_ROOK };

	//delete all chess piece at board
	DeleteChessGame();

	//Create Bitmaps of each chesspiece
	for (int i = 0; i < 2; i++) {
		for (int i1 = 0; i1 < 6; i1++){
			ChessPieceBitmap[i][i1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KING1 + i1 + (i * 10)));
		}
	}
	
	turn = 1;

	//setting board to start game
	for (int i = 0; i < 8; i++) {
		AddChessPiece(i, 0, procedure[i], 0);
		AddChessPiece(i,1,CHESS_PAWN, 0);
		AddChessPiece(i, 7, procedure[i], 1);
		AddChessPiece(i,6,CHESS_PAWN, 1);
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
	HBRUSH hMapBs[5], hOldBrush;
	int bMap = 0;

	hMapBs[0] = CreateSolidBrush(RGB(200, 200, 200));
	hMapBs[1] = CreateSolidBrush(RGB(255, 255, 255));
	hMapBs[2] = CreateSolidBrush(RGB(0, 255, 0));
	hMapBs[3] = CreateSolidBrush(RGB(100, 190, 240));
	hMapBs[4] = CreateSolidBrush(RGB(150, 100, 255));

	hOldBrush = (HBRUSH)SelectObject(hdc, hMapBs[0]);	//Get original brush

	for (int iy = sy; iy < sy + (MAP_SIZE * 8); iy += MAP_SIZE) {
		for (int ix = sx; ix < sx + (MAP_SIZE * 8); ix += MAP_SIZE) {
			int tx = (ix - sx) / MAP_SIZE, ty = (iy - sy) / MAP_SIZE;

			bMap = !bMap;

			if (bMoveMode && Board[ty][tx].bCanMove[GetTeam(SelectedPoint.x, SelectedPoint.y)] == 1)
				SelectObject(hdc, hMapBs[2]);
			else if (bMoveMode && Board[ty][tx].bCanMove[GetTeam(SelectedPoint.x, SelectedPoint.y)] == 2)
				SelectObject(hdc, hMapBs[4]);
			else if (bMoveMode && tx == SelectedPoint.x && ty == SelectedPoint.y)
				SelectObject(hdc, hMapBs[3]);
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
	DeleteObject(hMapBs[3]);
	DeleteObject(hMapBs[4]);
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


		if (turn != GetTeam(bx, by))
			return;

		SelectedPoint.x = bx;
		SelectedPoint.y = by;

		IsCheck(GetTeam(bx, by));

		MovementOfChessPieces(SelectedPoint, IdentifyMovement);

		for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
			for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
				if (Board[iy][ix].bCanMove[GetTeam(bx,by)]) {
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

		//castling
		if (GetType(bx, by) == CHESS_KING && Board[by][bx].bCanMove[GetTeam(bx, by)] == 2) {
			int tx = (bx == 6) ? 7 : 0, ty = (by == 0) ? 0 : 7;
			
			MoveChessPiece((tx == 7 ? 5 : 3), ty, tx, ty, NULL);
		}

		if (GetType(bx, by) == CHESS_KING) {
			bCastling[GetTeam(bx, by)][0] = false;
			bCastling[GetTeam(bx, by)][1] = false;
		}
		
		if (GetType(bx, by) == CHESS_ROOK) {
			if(SelectedPoint.x == 0)
				bCastling[GetTeam(bx, by)][0] = false;
			else if(SelectedPoint.x == 7)
				bCastling[GetTeam(bx, by)][1] = false;
		}


		//event of enpassant
		if (bEnPassant && Board[by][bx].bCanMove[GetTeam(bx, by)] == 2) {
			DeleteChessPiece(bx, FOWARD(by, -1, GetTeam(bx, by)));
			bEnPassant = false;
		}

		// quit event of enpassant
		if (Board[by][bx].bCanMove[GetTeam(bx, by)] != 2) {
			bEnPassant = false;
		}

		if (GetType(bx, by) == CHESS_PAWN && FOWARD(SelectedPoint.y, 2, GetTeam(bx, by)) == by) {
			for (int i = -1; i <= 1; i += 2) {
				int ttype = GetType(bx + i, by), tteam = GetTeam(bx + i, by);
				if (ttype == CHESS_PAWN && tteam != GetTeam(bx, by)) {
					bEnPassant = true;
				}
			}
		}


		//Promotion event of Pawn
		if (GetType(bx, by) == CHESS_PAWN && (by == 0 || by == 7)) {
			int ttype = -1,tteam  = GetTeam(bx,by);

			ttype = DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hMain, DlgProc);
			DeleteChessPiece(bx, by);
			AddChessPiece(bx, by, ttype, tteam);
		}

		if (IsCheckMate(!GetTeam(bx, by))) {
			InvalidateRect(hMain, NULL, 0);
			MessageBox(hMain, TEXT("CheckMate!!"), TEXT("GameOver"), MB_OK);
			if (MessageBox(hMain, TEXT("Do you want to replay game?"), TEXT("Choice"), MB_YESNO) == IDYES) {
				InitiateChessGame();
			}
			else {
				DestroyWindow(hMain);
			}
			turn = !turn;
			bMoveMode = false;
			return;
		}

		if (IsCheck(!GetTeam(bx,by))) {
			MessageBox(hMain, TEXT("Check!!"), TEXT("emergency"), MB_OK);
		}

		turn = !turn;
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


void MovementOfChessPieces(Point pt, IdentifyFunc ptFunc) {
	switch (GetType(pt.x, pt.y)) {
	case CHESS_KING:
		MovementOfKing(pt, ptFunc);
		break;
	case CHESS_QUEEN:
		MovementOfRook(pt, ptFunc);
		MovementOfBishop(pt, ptFunc);
		break;
	case CHESS_ROOK:
		MovementOfRook(pt, ptFunc);
		break;
	case CHESS_BISHOP:
		MovementOfBishop(pt, ptFunc);
		break;
	case CHESS_KNIGHT:
		MovementOfKnight(pt, ptFunc);
		break;
	case CHESS_PAWN:
		MovementOfPawn(pt, ptFunc);
		break;
	}
}

void MovementOfKing(Point pt, IdentifyFunc ptFunc) {

	for (int iy = pt.y - 1; iy < pt.y + 2; iy++) {
		for (int ix = pt.x - 1; ix < pt.x + 2; ix++) {
			if (ix == pt.x && iy == pt.y)
				continue;
			ptFunc(ix, iy, pt,1);
		}
	}

	//0 : queen side, 1 : king side
	for (int i = 0; i < 2; i++) {
		int cntBetween[2] = { 3,2 };
		int rs = 0;
		if (bCastling[GetTeam(pt.x, pt.y)][i] == 0)
			continue;

		for (int ix = 1; ix <= cntBetween[i]; ix++) {
			if (Board[pt.y][FOWARD(pt.x, ix, !i)].cp != NULL || TmpMovement[pt.y][ix] == true)
				rs = 1;
		}

		if (rs)
			continue;

		if (i == 0) {
			ptFunc(pt.x - 2, pt.y, pt, 2);
		}
		else {
			ptFunc(pt.x + 2, pt.y, pt, 2);
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

		while (ptFunc(ptTmp.x, ptTmp.y, pt,1)) {
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

		while (ptFunc(ptTmp.x, ptTmp.y, pt,1)) {
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
		ptFunc(pt.x + ways[i][0], pt.y + ways[i][1], pt,1);
	}
}

void MovementOfPawn(Point pt, IdentifyFunc ptFunc) {
	int team = GetTeam(pt.x, pt.y);

	if (bEnPassant && ((team == 0 && pt.y == 4) || (team == 1 && pt.y == 3))) {
		for (int i = -1; i <= 1; i += 2) {
			if (GetTeam(pt.x + i, pt.y) != team && GetType(pt.x + i, pt.y) == CHESS_PAWN) {
				ptFunc(pt.x + i, FOWARD(pt.y , 1,team), pt,2);
			}
		}
	}
	
	if (Board[FOWARD(pt.y, 1, team)][pt.x].cp == NULL) {
		ptFunc(pt.x, FOWARD(pt.y, 1, team), pt,1);


		if ((pt.y == 1 || pt.y == 6) && Board[FOWARD(pt.y, 2, team)][pt.x].cp == NULL) 
			ptFunc(pt.x, FOWARD(pt.y, 2, team), pt,1);
		
	}

	for (int i = 0; i < 2; i++) {
		if (Board[FOWARD(pt.y,1,team)][FOWARD(pt.x, 1, i)].cp != NULL) {
			ptFunc(FOWARD(pt.x, 1, i), FOWARD(pt.y, 1, team), pt,1);
		}
	}
}

int IdentifyMovement(int x, int y, Point pt,int value) {
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
		return 1;
	}

	MoveChessPiece(pt.x, pt.y, x, y, &CPtmp);
	int a = GetTeam(pt.x, pt.y);
	Board[y][x].bCanMove[GetTeam(pt.x, pt.y)] = value;
	
	if (CPtmp.team != -1 || CPtmp.type != -1)
		AddChessPiece(x, y, CPtmp.type, CPtmp.team);

	return 1;
}

int IdentifyCheck(int x, int y, Point pt,int value) {
	if (!IsRightPos(x) || !IsRightPos(y))
		return 0;

	if (Board[y][x].cp != NULL && Board[y][x].cp->team == GetTeam(pt.x, pt.y))
		return 0;


	TmpMovement[y][x] = value;

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
				MovementOfChessPieces(pt, IdentifyCheck);
			}
		}
	}

	for (int iy = 0; iy < MAP_BLOCKCOUNT; iy++) {
		for (int ix = 0; ix < MAP_BLOCKCOUNT; ix++) {
			if (TmpMovement[iy][ix] && Board[iy][ix].cp != NULL && GetType(ix, iy) == CHESS_KING && GetTeam(ix, iy) == team)
				return 1;
		}
	}

	return 0;
}

bool IsCheckMate(int team) {
	Point pt = { 0,0 };
	AllClearMovement();

	for (pt.y = 0; pt.y < MAP_BLOCKCOUNT; pt.y++) {
		for (pt.x = 0; pt.x < MAP_BLOCKCOUNT; pt.x++) {
			if(GetTeam(pt.x,pt.y) == team)
				MovementOfChessPieces(pt, IdentifyMovement);
		}
	}

	for (pt.y = 0; pt.y < MAP_BLOCKCOUNT; pt.y++) {
		for (pt.x = 0; pt.x < MAP_BLOCKCOUNT; pt.x++) {
			if (Board[pt.y][pt.x].bCanMove[team])
				return 0;
		}
	}

	return 1;
}

int GetTurn() {
	return turn;
}