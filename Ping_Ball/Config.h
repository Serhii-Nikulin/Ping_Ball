#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	unsigned char R, G, B;

	AColor();
	AColor(unsigned char r, unsigned char g, unsigned char b);
	AColor(const AColor &color, int pen_size);
	AColor(const AColor &pen_color, const AColor &brush_color, int pen_size);
	int Get_RGB() const;
	void Select(HDC hdc) const;
	void Select_Pen(HDC hdc) const;

private:
	HPEN  Pen;
	HBRUSH Brush;
};
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Round_Rect(HDC hdc, RECT &rect, int corner_size = 2);
	static int Rand(int range);
	static void Throw();

	static const int Global_Scale = 3;
	static int Current_Timer_Tick;

	static const AColor BG_Color;
	static const AColor Red_Color;
	static const AColor Blue_Color;
	static const AColor White_Color;
	const static AColor Red_Highlight_Unbreakable;
	const static AColor Blue_Highlight_Unbreakable;
	const static AColor Parachute_Color;
	static const AColor Teleport_Color;
	static const AColor Ad_Ball_Highlight_Color;
	static const AColor Ad_Table_Blue_Color;
	static const AColor Ad_Table_Red_Color;

	static HWND Hwnd;

	static const int FPS = 20;
	static bool Has_Floor;
	static double Moving_Size_Step;
	static const int Hits_Per_Letter = 10;

	//Border
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;

	//Platform
	static const int Platform_Y_Pos = 185;

	//Level
	static HPEN Letter_Pen;

	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;

	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	static const int Max_Active_Bricks_Count = 20;
	static const int Max_Falling_Letter_Count = 20;
};
//------------------------------------------------------------------------------------------------------------