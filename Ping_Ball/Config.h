#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	const unsigned char R, G, B;

	AColor(unsigned char r, unsigned char g, unsigned char b);

};
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Setup_Colors();
	static void Create_Pen_Brush(HPEN& pen, HBRUSH& brush, unsigned char r, unsigned char g, unsigned char b);
	static void Create_Pen_Brush(HPEN& pen, HBRUSH& brush, const AColor &color);
	static int Rand(int min, int max);

	static const int Global_Scale = 3;
	static int Current_Timer_Tick;

	static const AColor BG_Color;
	static const AColor Red_Brick_Color;
	static const AColor Blue_Brick_Color;

	static HWND Hwnd;

	static HPEN BG_Pen;
	static HBRUSH BG_Brush;

	static bool Has_Floor;
	static double Moving_Size_Step;

	//Border
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;


	//Platform
	static const int Platform_Y_Pos = 185;

	//Level
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;

	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	static const int Max_Active_Bricks_Count = 20;
};
//------------------------------------------------------------------------------------------------------------