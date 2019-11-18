#include <Windows.h>

#include "Chess.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("First");

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

	switch (iMessage) {
	case WM_CREATE:
		bx = 10; by = 10; //Setting the Board Position
		break;
	case WM_LBUTTONDOWN:
		//ChessBoard Message
		
		ChessBoardMessage(bx,by,LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HBRUSH hOldBs, hTmpBrush;

		BeginPaint(hWnd, &ps);
		
		hTmpBrush = CreateSolidBrush(WHITE_BRUSH);	//temporary brush to extract old brush
		hOldBs = (HBRUSH)SelectObject(ps.hdc,hTmpBrush);	//extract old brush
		
		PaintChessBoard(ps.hdc, 10, 10);

		SelectObject(ps.hdc, hOldBs);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}