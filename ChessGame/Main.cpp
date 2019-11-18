#include <Windows.h>

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	int bMap = 0;

	switch (iMessage) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		HBRUSH hMapBs[2];
		HBRUSH hOldBs;

		BeginPaint(hWnd, &ps);
		
		hMapBs[0] = CreateSolidBrush(RGB(200, 200, 200));
		hMapBs[1] = CreateSolidBrush(RGB(255,255,255));

		hOldBs = (HBRUSH)SelectObject(ps.hdc, hMapBs[0]);

		for (int iy = 10; iy < 10 + (50 * 8); iy += 50) {
			for (int ix = 10; ix < 10 + (50 * 8); ix += 50) {
				bMap = !bMap;

				SelectObject(ps.hdc, hMapBs[bMap]);
				
				Rectangle(ps.hdc, ix, iy, ix + 50, iy + 50);
			}
			bMap = !bMap;
		}

		//Rectangle(ps.hdc, 10, 10, 100, 100);

		SelectObject(ps.hdc, hOldBs);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}