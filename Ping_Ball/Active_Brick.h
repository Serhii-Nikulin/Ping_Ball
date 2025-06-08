#pragma once

#include "Config.h"

enum EBrick_Type: unsigned char {EBT_None, EBT_Red, EBT_Blue};

//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual void Act() = 0;
	virtual void Draw(HDC hdc, RECT &paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
public:
	AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Setup_Colors();

private:
	EBrick_Type Brick_Type;

	RECT Brick_Rect;

	int Fade_Step;

	static unsigned char Get_Fading_Value(int step, int max_value, int min_value);
	static void Get_Fading_Color(const AColor &origin_color, int step, AColor &fading_color);

	static const int Max_Fade_Step = AsConfig::FPS;

	static AColor Fading_Red_Brick_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Brick_Colors[Max_Fade_Step];
};
//------------------------------------------------------------------------------------------------------------