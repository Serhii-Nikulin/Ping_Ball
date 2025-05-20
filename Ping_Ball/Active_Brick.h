#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AActive_Brick
{
public:
	AActive_Brick();
	void Draw(HDC hdc);
	void Act(HWND hwnd);

private:
	RECT Brick_Rect;
	HPEN Pen;
	HBRUSH Brush;

	const int Max_Fade_Step = 80;
	int Fade_Step;
};
//------------------------------------------------------------------------------------------------------------