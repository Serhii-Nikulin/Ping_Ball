#pragma once

#include "Config.h"

#include "Level.h"
#include "Platform.h"
#include "Border.h"

//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Redraw(HWND hwnd);
	void Draw(HDC hdc, RECT &paint_area, HPEN &bg_pen, HBRUSH &bg_brush);
	void Move(HWND hwnd, ALevel *level, AsPlatform *platform);

private:
	static const int Ball_Size = 4;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;

	int Ball_X_Pos;
	int Ball_Y_Pos;
	double Ball_Speed, Ball_Direction;
};
//------------------------------------------------------------------------------------------------------------