#include <Windows.h>

#include "resource.h"
#include "Chess.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ChessGame");

HWND hMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

int bx, by;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	int bMap = 0;
	PAINTSTRUCT ps;
	LPMINMAXINFO pmmi;
	POINT WindowSizePoint = { 650, 460 };
	WCHAR strTurn[50];

	switch (iMessage) {
	case WM_GETMINMAXINFO:		// 화면의 크기를 고정해주는 메시지
		pmmi = (LPMINMAXINFO)lParam;
		pmmi->ptMaxTrackSize = WindowSizePoint;
		pmmi->ptMinTrackSize = WindowSizePoint;
		pmmi->ptMaxSize = WindowSizePoint;
		return 0;
	case WM_CREATE:			// 프로그램이 실행되었을 때 딱 한번만 실행됨
		hMain = hWnd;
		bx = 10; by = 10; //Setting the Board Position(체스판의 초기 위치)
		InitiateChessGame();// 처음 시작했을 때 기물의 위치와 기물의 이미지 파일을 가져오는 함수
		break;
	case WM_LBUTTONDOWN:	// 마우스 왼쪽 클릭하였을 때 발생하는 함수

		//ChessBoard Message handle function
		ChessBoardMessage(bx,by,LOWORD(lParam), HIWORD(lParam));	// LOWORD(lParam) : 마우스의 x좌표, HIWORD(lParam) : 마우스의 y좌표

		//redraw window
		InvalidateRect(hWnd, NULL, 0);	// 기물의 위치가 바뀌거나 기물이 사라지거나(기물이 먹혔거나), 기물의 이동가능한 위치 표현할 때 화면을 다시 그려줌
		break;
	case WM_PAINT:		// 화면을 그려줄 때(InvalidateRecrt() 함수가 실행될 때) 실행되는 부분 
		BeginPaint(hWnd, &ps);	// 그릴 화면(도화지)을 준비하는 함수

		PaintChessBoard(ps.hdc, bx, by);	// 체스 보드를 그려주는 함수
		PaintChessPiece(ps.hdc, bx, by);	// 체스 기물을 그려주는 함수

		/* 현재 누구 차례인지 나타내주는 코드 */
		
		
		if (GetTurn()) 
			wsprintf(strTurn, TEXT("백의 턴"));
		else 
			wsprintf(strTurn, TEXT("흑의 턴"));

		TextOut(ps.hdc, 430, 10, strTurn, wcslen(strTurn));

		EndPaint(hWnd, &ps);	//그림을 다 마치고 정리하는 함수
		break;
	case WM_CLOSE:		// 화면을 닫을 때 실행되는 부분
		DeleteChessGame();	// 이미지 파일을 반납하고 힙 영역을 정리해주는 부분
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}