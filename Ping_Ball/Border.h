#pragma once

#include "Config.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker
{
public:
	AsBorder();
	virtual bool Check_Hit(double next_x_pos, double y_pos, ABall *ball);

	void Init();
	void Draw(HDC hdc);

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN &bg_pen, HBRUSH &bg_brush) const;

	HPEN Border_Blue_Pen, Border_Red_Pen;
	HBRUSH Border_Blue_Brush, Border_Red_Brush;
};
//------------------------------------------------------------------------------------------------------------