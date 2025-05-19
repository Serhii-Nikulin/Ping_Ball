#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();
	void Init();
	void Redraw(HWND hwnd);
	void Draw(HDC hdc, RECT &paint_area, HPEN &bg_pen, HBRUSH &bg_brush);

	int Width;
	int X_Pos;
	int X_Step;

private:
	void Draw_Circle(HDC hdc, int x, int y);

	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Highlight_Pen;
	HPEN Platform_Side_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Side_Brush, Platform_Inner_Brush;

	int Inner_Width;
};
//------------------------------------------------------------------------------------------------------------