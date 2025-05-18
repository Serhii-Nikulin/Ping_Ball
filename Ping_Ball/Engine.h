#pragma once
#include "Windows.h"

#define _USE_MATH_DEFINES
#include <math.h>

const int Global_Scale = 3;
const int Timer_ID = WM_USER + 1;
enum EKey_Type {EKT_None, EKT_Left, EKT_Right, EKT_Space};

//Bricks
enum ELetter_Type {ELT_None, ELT_O};
enum EBrick_Type: unsigned char {EBT_None, EBT_Red, EBT_Blue};

class AsEngine;
class ALevel;
class AsPlatform;
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Redraw(AsEngine *engine);
	void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
	void Move(AsEngine *engine, ALevel *level, AsPlatform *platform);

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
class ALevel
{
public:
	ALevel();

	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Check_Level_Brick_Hit(int next_x_pos, int &next_y_pos, double &ball_direction);

	static const int Level_Height = 14;
	static const int Level_Width = 12;
	
private:
	void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);

	RECT Level_Rect{};
	HPEN Letter_Pen;
	HPEN Brick_Red_Pen, Brick_Blue_Pen;
	HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();
	void Init();
	void Redraw(AsEngine *engine);
	void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);

	int Width;
	int X_Pos;
	int X_Step;

	static const int Height = 7;
	static const int Y_Pos = 185;

private:
	void Draw_Circle(HDC hdc, int x, int y);

	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Highlight_Pen;
	HPEN Platform_Side_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Side_Brush, Platform_Inner_Brush;
	
	int Inner_Width;
};
//------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
	AsBorder();

	void Init();
	void Bounds(HDC hdc, AsEngine *engine);

	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, AsEngine *engine);

	HPEN Border_Blue_Pen, Border_Red_Pen;
	HBRUSH Border_Blue_Brush, Border_Red_Brush;
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	static void Create_Pen_Brush(HPEN& pen, HBRUSH& brush, unsigned char r, unsigned char g, unsigned char b);
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;

private:
	ABall Ball;
	ALevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------