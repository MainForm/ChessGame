#include "Chess.h"

void PaintChessBoard(HDC hdc, int sx, int sy) {
	HBRUSH hMapBs[2];
	int bMap = 0;

	hMapBs[0] = CreateSolidBrush(RGB(200, 200, 200));
	hMapBs[1] = CreateSolidBrush(RGB(255, 255, 255));

	for (int iy = sy; iy < sy + (50 * 8); iy += 50) {
		for (int ix = sx; ix < sx + (50 * 8); ix += 50) {
			bMap = !bMap;

			SelectObject(hdc, hMapBs[bMap]);

			Rectangle(hdc, ix, iy, ix + 50, iy + 50);
		}
		bMap = !bMap;
	}
}