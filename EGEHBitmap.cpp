#include "EGEHBitmap.h"

// �õ�IMAGE�������ɫ��Ϣ����
COLORREF* GetColors(PIMAGE img)
{
	int w = getwidth(img);
	int h = getheight(img);
	COLORREF* color = new COLORREF[w * h];

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			color[w * i + j] = getpixel(j, i, img);

	return color;
}

// �õ�IMAGE�����HBITMAP
HBITMAP GetImageHBitmap(PIMAGE img)
{
	int w = getwidth(img);
	int h = getheight(img);

	COLORREF* colors = GetColors(img);

	HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, (void*)colors);

	return hBitmap;
}