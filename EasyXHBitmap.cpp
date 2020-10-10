#include "EasyXHBitmap.h"

// 得到IMAGE对象的颜色信息数组
COLORREF* GetColors(IMAGE img)
{
	int w = img.getwidth();
	int h = img.getheight();
	COLORREF* color = new COLORREF[w * h];

	IMAGE* old = GetWorkingImage();
	SetWorkingImage(&img);
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			color[w * i + j] = getpixel(j, i);
	SetWorkingImage(old);

	return color;
}

// 得到IMAGE对象的HBITMAP
HBITMAP GetImageHBitmap(IMAGE img)
{
	int w = img.getwidth();
	int h = img.getheight();

	// 由于HBITMAP那里需要BGR一下，所以把整个数组反个色
	COLORREF* colors = GetColors(img);
	for (int i = 0; i < w * h; i++)
		colors[i] = BGR(colors[i]);

	HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, (void*)colors);

	return hBitmap;
}