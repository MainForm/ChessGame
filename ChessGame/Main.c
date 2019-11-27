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
	case WM_GETMINMAXINFO:		// ȭ���� ũ�⸦ �������ִ� �޽���
		pmmi = (LPMINMAXINFO)lParam;
		pmmi->ptMaxTrackSize = WindowSizePoint;
		pmmi->ptMinTrackSize = WindowSizePoint;
		pmmi->ptMaxSize = WindowSizePoint;
		return 0;
	case WM_CREATE:			// ���α׷��� ����Ǿ��� �� �� �ѹ��� �����
		hMain = hWnd;
		bx = 10; by = 10; //Setting the Board Position(ü������ �ʱ� ��ġ)
		InitiateChessGame();// ó�� �������� �� �⹰�� ��ġ�� �⹰�� �̹��� ������ �������� �Լ�
		break;
	case WM_LBUTTONDOWN:	// ���콺 ���� Ŭ���Ͽ��� �� �߻��ϴ� �Լ�

		//ChessBoard Message handle function
		ChessBoardMessage(bx,by,LOWORD(lParam), HIWORD(lParam));	// LOWORD(lParam) : ���콺�� x��ǥ, HIWORD(lParam) : ���콺�� y��ǥ

		//redraw window
		InvalidateRect(hWnd, NULL, 0);	// �⹰�� ��ġ�� �ٲ�ų� �⹰�� ������ų�(�⹰�� �����ų�), �⹰�� �̵������� ��ġ ǥ���� �� ȭ���� �ٽ� �׷���
		break;
	case WM_PAINT:		// ȭ���� �׷��� ��(InvalidateRecrt() �Լ��� ����� ��) ����Ǵ� �κ� 
		BeginPaint(hWnd, &ps);	// �׸� ȭ��(��ȭ��)�� �غ��ϴ� �Լ�

		PaintChessBoard(ps.hdc, bx, by);	// ü�� ���带 �׷��ִ� �Լ�
		PaintChessPiece(ps.hdc, bx, by);	// ü�� �⹰�� �׷��ִ� �Լ�

		/* ���� ���� �������� ��Ÿ���ִ� �ڵ� */
		
		
		if (GetTurn()) 
			wsprintf(strTurn, TEXT("���� ��"));
		else 
			wsprintf(strTurn, TEXT("���� ��"));

		TextOut(ps.hdc, 430, 10, strTurn, wcslen(strTurn));

		EndPaint(hWnd, &ps);	//�׸��� �� ��ġ�� �����ϴ� �Լ�
		break;
	case WM_CLOSE:		// ȭ���� ���� �� ����Ǵ� �κ�
		DeleteChessGame();	// �̹��� ������ �ݳ��ϰ� �� ������ �������ִ� �κ�
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}