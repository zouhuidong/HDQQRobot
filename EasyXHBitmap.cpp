#include "EasyXHBitmap.h"

// �õ�IMAGE�������ɫ��Ϣ����
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

// �õ�IMAGE�����HBITMAP
HBITMAP GetImageHBitmap(IMAGE img)
{
	int w = img.getwidth();
	int h = img.getheight();

	// ����HBITMAP������ҪBGRһ�£����԰��������鷴��ɫ
	COLORREF* colors = GetColors(img);
	for (int i = 0; i < w * h; i++)
		colors[i] = BGR(colors[i]);

	HBITMAP hBitmap = CreateBitmap(w, h, 1, 32, (void*)colors);

	return hBitmap;
}