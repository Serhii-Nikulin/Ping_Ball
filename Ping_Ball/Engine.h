#pragma once

#include "Border.h"
#include "Level.h"
#include "Platform.h"

const int Timer_ID = WM_USER + 1;
enum EKey_Type {EKT_None, EKT_Left, EKT_Right, EKT_Space};
enum EGame_State { EGS_Play_Level, EGS_Lost_Ball, EGS_Restart_Level };

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
	void Restart_Level();
	void Play_Level();
	void Act();
	void On_Falling_Letter(AFalling_Letter *falling_letter);

	EGame_State Game_State;

	ABall Balls[AsConfig::Max_Balls_Count];

	AsLevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------