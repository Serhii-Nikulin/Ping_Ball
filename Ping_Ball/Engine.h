#pragma once

#include "Border.h"
//#include "Level.h"
#include "Platform.h"
#include "Ball.h"

const int Timer_ID = WM_USER + 1;
enum EKey_Type {EKT_None, EKT_Left, EKT_Right, EKT_Space};

//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

private:
	static const int FPS = 20;

	HWND Hwnd;

	ABall Ball;
	ALevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------