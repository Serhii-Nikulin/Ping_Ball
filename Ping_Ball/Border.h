#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
	AsBorder();

	void Init();
	void Draw(HDC hdc);

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN &bg_pen, HBRUSH &bg_brush);

	HPEN Border_Blue_Pen, Border_Red_Pen;
	HBRUSH Border_Blue_Brush, Border_Red_Brush;
};
//------------------------------------------------------------------------------------------------------------