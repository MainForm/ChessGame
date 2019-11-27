#include <Windows.h>

#include "resource.h"
#include "Chess.h"
#include "LinkedList.h"

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
	case WM_GETMINMAXINFO:
		pmmi = (LPMINMAXINFO)lParam;
		pmmi->ptMaxTrackSize = WindowSizePoint;
		pmmi->ptMinTrackSize = WindowSizePoint;
		pmmi->ptMaxSize = WindowSizePoint;
		return 0;
	case WM_CREATE:
		hMain = hWnd;
		bx = 10; by = 10; //Setting the Board Position
		InitiateChessGame();
		break;
	case WM_LBUTTONDOWN:

		//ChessBoard Message handle function
		ChessBoardMessage(bx,by,LOWORD(lParam), HIWORD(lParam));

		//redraw window
		InvalidateRect(hWnd, NULL, 0);
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);

		PaintChessBoard(ps.hdc, bx, by);
		PaintChessPiece(ps.hdc, bx, by);

		if (GetTurn()) 
			wsprintf(strTurn, TEXT("백의 턴"));
		else 
			wsprintf(strTurn, TEXT("흑의 턴"));

		TextOut(ps.hdc, 430, 10, strTurn, wcslen(strTurn));
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		DeleteChessGame();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}