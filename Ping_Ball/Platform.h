#pragma once

#include "Config.h"
enum EPlatform_State { EPS_Missing, EPS_Normal, EPS_Meltdown };
//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();
	void Init();
	void Act(HWND hwnd);

	void Redraw(HWND hwnd);
	void Draw(HDC hdc, RECT &paint_area);

	int Width;
	int X_Pos;
	int X_Step;

private:
	void Draw_Meltdown(HDC hdc);
	void Draw_Circle(HDC hdc, int x, int y);
	void Draw_Normal(HDC hdc);

	EPlatform_State Platform_State;
	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Highlight_Pen;
	HPEN Platform_Side_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Side_Brush, Platform_Inner_Brush;

	static const int Meltdown_Speed = 4;
	static const int Normal_Width = 28;

	int Meltdown_Y_Pos[Normal_Width * AsConfig::Global_Scale];
	int Inner_Width;
};
//------------------------------------------------------------------------------------------------------------