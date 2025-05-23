#pragma once

#include "Config.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Redraw();
	void Draw(HDC hdc, RECT &paint_area);
	void Move(ALevel *level, int platform_x_pos, int platform_width);

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