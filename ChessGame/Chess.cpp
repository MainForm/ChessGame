#include "Chess.h"

#define MAP_SIZE 50

void PaintChessBoard(HDC hdc, int sx, int sy) {
	HBRUSH hMapBs[2], hOldBrush;
	int bMap = 0;

	hMapBs[0] = CreateSolidBrush(RGB(200, 200, 200));
	hMapBs[1] = CreateSolidBrush(RGB(255, 255, 255));

	hOldBrush = (HBRUSH)SelectObject(hdc, hMapBs[0]);	//Get original brush

	for (int iy = sy; iy < sy + (MAP_SIZE * 8); iy += MAP_SIZE) {
		for (int ix = sx; ix < sx + (MAP_SIZE * 8); ix += MAP_SIZE) {
			bMap = !bMap;

			SelectObject(hdc, hMapBs[bMap]);

			Rectangle(hdc, ix, iy, ix + MAP_SIZE, iy + MAP_SIZE);
		}
		bMap = !bMap;
	}

	SelectObject(hdc, hOldBrush);//return brush
}

void ChessBoardMessage(int sx, int sy, int x, int y) {
	int px = x - sx, py = y - sy;// setting offset point at 0
	WCHAR cx = 'A' + (px / MAP_SIZE), cy = '8' - (py / MAP_SIZE);// each block address

	if ((cx < 'A' || cx > 'H') || (cy < '1' || cy > '8'))	//exit functio when point get out off board area.
		return;


}