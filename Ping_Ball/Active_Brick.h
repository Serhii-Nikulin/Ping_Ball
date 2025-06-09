#pragma once

#include "Config.h"

enum EBrick_Type: unsigned char {EBT_None, EBT_Red, EBT_Blue, EBT_Unbreakable};
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Draw(HDC hdc, RECT &paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
protected:
	virtual ~AActive_Brick();

	AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y);

	EBrick_Type Brick_Type;
	RECT Brick_Rect;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Red_Blue: public AActive_Brick
{
public:
	virtual ~AActive_Brick_Red_Blue();
	AActive_Brick_Red_Blue(EBrick_Type brick_type, int brick_x, int brick_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Setup_Colors();

private:
	int Fade_Step;

	static unsigned char Get_Fading_Value(int step, int max_value, int min_value);
	static void Get_Fading_Color(const AColor &origin_color, int step, AColor &fading_color);

	static const int Max_Fade_Step = AsConfig::FPS;

	static AColor Fading_Red_Brick_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Brick_Colors[Max_Fade_Step];
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Unbreakable: public AActive_Brick
{
public:
	virtual ~AActive_Brick_Unbreakable();
	AActive_Brick_Unbreakable(EBrick_Type brick_type, int brick_x, int brick_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

private:
	int Unbreakable_Animation_Step;
	static const int Max_Unbreakable_Animation_Step = 5;
};
//------------------------------------------------------------------------------------------------------------