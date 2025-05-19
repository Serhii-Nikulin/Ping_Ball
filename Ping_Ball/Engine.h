#pragma once

#include "Border.h"
#include "Level.h"

const int Timer_ID = WM_USER + 1;
enum EKey_Type {EKT_None, EKT_Left, EKT_Right, EKT_Space};

class AsEngine;
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
class AsEngine
{
public:
	AsEngine();

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