#ifndef __CHESS_HEADER__		// include �ߺ��� �����ϱ� ���� ��ó����
#define __CHESS_HEADER__

#include <Windows.h>		//WindowAPI ��� ���� 
#include <stdbool.h>		//bool Ÿ�� ���� ���ؼ� ����

#define MAP_SIZE 50			// ü���� �� ���� ������ 50 pixel�� ����
#define MAP_BLOCKCOUNT 8	//  �� �ǿ� �ش��ϴ� x, y�� �ִ� ����� 8�� ������

typedef struct _Point {		// �츮�� ������ �⹰�� ��ġ�� ����ϴ� ����ü
	int x;					// ���� x ��ǥ, y ��ǥ�� �����
	int y;
}Point;

typedef int (*IdentifyFunc)(int, int, Point,int);

typedef struct _ChessPiece {		// �⹰�� ���� ������ ����ϴ� ����ü
	int type;						// 0�̸� king, 1�̸� queen, 2�̸� rook, 3�̸� bishop, 4�̸� knight, 5�̸� pawn
	int team;						// 0�̸� ��, 1�̸� ��
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
	int bCanMove[2];			// �⹰�� ������ �� �ִ� ��ġ [0]�� ���� ������ ����, [1]�� ���� ������ ����
	ChessPiece* cp;				// NULL�̸� �⹰�� ����, NULL�� �ƴϸ� �⹰�� ����!
} ChessBlock;

void InitiateChessGame();		// ü�� ������ ������ �� ������ ���� �ش��ϴ� �̹��� ������ �ҷ�����, �⹰�� ��ġ�� �����Ѵ�
void DeleteChessGame();			// ü�� ������ ������ �� �̹��� ������ �����ϰ� ���� �� �����ϰ�, �� ������ �����Ѵ�.

void AddChessPiece(int x,int y,int type, int team);		// x,y ��ǥ�� team�� type�� �ش��ϴ� �⹰�� �����Ѵ�
														// �̹� �� ��ǥ�� �⹰�� �ִ� ��� �׳� �����Ѵ�
void DeleteChessPiece(int x, int y);					// x,y ��ǥ�� �ִ� �⹰�� ���ش�

void PaintChessPiece(HDC hdc, int sx, int sy);		// ȭ�鿡 �⹰�� ��� �ִ��� �׷��ش�.
void PaintChessBoard(HDC hdc, int sx, int sy);		// ȭ�鿡 ü������ �׷��ش�.(���⼭ ������ �������ִ� �Ͱ� �����Ͽ��� ��
													// �ʷϻ� ����)

//sx and sy are start point of chess board and x and y are clicked point
void ChessBoardMessage(int sx, int sy, int x, int y);// ������ �����̹Ƿ� ���߿� ����~~

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
int MoveChessPiece(int dx, int dy, int fx, int fy, ChessPiece* CPreturn);	// ������ fx,fy ��ǥ���� ������ dx,dy ��ǥ�� �⹰�� �̵�

void AllClearMovement();	// bCanMove[] �� ���Ҹ� ���� 0���� �ʱ�ȭ�ϴ� �Լ�


int IdentifyMovement(int x, int y, Point pt,int value);// ���߿� ����

void MovementOfChessPieces(Point pt, IdentifyFunc ptFunc);

/* �� ���� ���� �̵� ���� ���� */
void MovementOfKing(Point pt,IdentifyFunc ptFunc);
void MovementOfRook(Point pt, IdentifyFunc ptFunc);
void MovementOfBishop(Point pt, IdentifyFunc ptFunc);
void MovementOfKnight(Point pt, IdentifyFunc ptFunc);
void MovementOfPawn(Point pt, IdentifyFunc ptFunc);

bool IsCheck(int team);		// ChessPiece ����ü�� ��� team���κ��� �� team�� king�� check���� �ƴ��� Ȯ��
int IdentifyCheck(int x, int y, Point pt,int value);

bool IsCheckMate(int team);

int GetTurn();		// ���� ���� �������� Ȯ�� (0 : ���� 1 : ���� ��)
#endif

/*
���Ļ��� ����
���� �ι� �����ϸ� ���Ļ� = 1�� �Ѵ�

*/