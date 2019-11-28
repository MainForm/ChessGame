#ifndef __CHESS_HEADER__		// include 중복을 방지하기 위한 전처리기
#define __CHESS_HEADER__

#include <Windows.h>		//WindowAPI 헤더 선언 
#include <stdbool.h>		//bool 타입 쓰기 위해서 선언

#define MAP_SIZE 50			// 체스의 한 블럭을 윈도우 50 pixel에 맞춤
#define MAP_BLOCKCOUNT 8	//  각 판에 해당하는 x, y의 최대 사이즈를 8로 지정함

typedef struct _Point {		// 우리가 선택한 기물의 위치를 기억하는 구조체
	int x;					// 각각 x 좌표, y 좌표로 기억함
	int y;
}Point;

typedef int (*IdentifyFunc)(int, int, Point,int);

typedef struct _ChessPiece {		// 기물에 대한 정보를 기억하는 구조체
	int type;						// 0이면 king, 1이면 queen, 2이면 rook, 3이면 bishop, 4이면 knight, 5이면 pawn
	int team;						// 0이면 흑, 1이면 백
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
	int bCanMove[2];			// 기물이 움직일 수 있는 위치 [0]은 흑이 가능한 공간, [1]은 백이 가능한 공간
	ChessPiece* cp;				// NULL이면 기물이 없고, NULL이 아니면 기물이 있음!
} ChessBlock;

void InitiateChessGame();		// 체스 게임을 시작할 때 각각의 말에 해당하는 이미지 파일을 불러오고, 기물의 위치를 설정한다
void DeleteChessGame();			// 체스 게임이 끝냈을 때 이미지 파일을 안전하게 저장 및 종료하고, 힙 영역을 정리한다.

void AddChessPiece(int x,int y,int type, int team);		// x,y 좌표에 team과 type에 해당하는 기물을 생성한다
														// 이미 그 좌표에 기물이 있는 경우 그냥 종료한다
void DeleteChessPiece(int x, int y);					// x,y 좌표에 있는 기물을 없앤다

void PaintChessPiece(HDC hdc, int sx, int sy);		// 화면에 기물이 어디 있는지 그려준다.
void PaintChessBoard(HDC hdc, int sx, int sy);		// 화면에 체스판을 그려준다.(여기서 색깔을 번갈아주는 것과 선택하였을 때
													// 초록색 색깔)

//sx and sy are start point of chess board and x and y are clicked point
void ChessBoardMessage(int sx, int sy, int x, int y);// 복잡한 내용이므로 나중에 설명~~

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
int MoveChessPiece(int dx, int dy, int fx, int fy, ChessPiece* CPreturn);	// 기존의 fx,fy 좌표에서 선택한 dx,dy 좌표로 기물을 이동

void AllClearMovement();	// bCanMove[] 의 원소를 전부 0으로 초기화하는 함수


int IdentifyMovement(int x, int y, Point pt,int value);// 나중에 설명

void MovementOfChessPieces(Point pt, IdentifyFunc ptFunc);

/* 각 말에 대한 이동 범위 지정 */
void MovementOfKing(Point pt,IdentifyFunc ptFunc);
void MovementOfRook(Point pt, IdentifyFunc ptFunc);
void MovementOfBishop(Point pt, IdentifyFunc ptFunc);
void MovementOfKnight(Point pt, IdentifyFunc ptFunc);
void MovementOfPawn(Point pt, IdentifyFunc ptFunc);

bool IsCheck(int team);		// ChessPiece 구조체의 멤버 team으로부터 그 team의 king이 check인지 아닌지 확인
int IdentifyCheck(int x, int y, Point pt,int value);

bool IsCheckMate(int team);

int GetTurn();		// 현재 누구 턴인지룰 확인 (0 : 흑의 1 : 백의 턴)
#endif

/*
앙파상의 조건
폰을 두번 전진하면 앙파상 = 1을 한다

*/