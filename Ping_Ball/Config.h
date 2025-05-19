#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(HPEN& pen, HBRUSH& brush, unsigned char r, unsigned char g, unsigned char b);

	static const int Global_Scale = 3;

	//Border
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;


	//Platform
	static const int Platform_Height = 7;
	static const int Platform_Y_Pos = 185;
};
//------------------------------------------------------------------------------------------------------------