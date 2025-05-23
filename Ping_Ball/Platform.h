#pragma once

#include "Config.h"

enum EPlatform_State { EPS_Missing, EPS_Normal, EPS_Meltdown, EPS_Rolling, EPS_Expanding_Rolling };
//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();
	void Init();
	void Act();
	void Set_State(EPlatform_State new_state);

	void Redraw();
	void Draw(HDC hdc, RECT &paint_area);


	int Width;
	int X_Pos;
	int X_Step;

private:
	void Clear_BG(HDC hdc);
	void Draw_Circle(HDC hdc, int x, int y);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Normal_State(HDC hdc);
	void Draw_Meltdown_State(HDC hdc);
	void Draw_Rolling_State(HDC hdc);
	void Draw_Expanding_Rolling_State(HDC hdc);

	EPlatform_State Platform_State;
	RECT Platform_Rect, Prev_Platform_Rect;
	HPEN Highlight_Pen;
	HPEN Platform_Side_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Side_Brush, Platform_Inner_Brush;

	static const int Meltdown_Speed = 4;
	static const int Normal_Width = 28;
	static const int Circle_Size = 7;
	static const int Max_Rotation_Step = 16;
	static const int Finish_Rolling_X_Pos = (AsConfig::Max_X_Pos + 1 + AsConfig::Border_X_Offset + 1 - Circle_Size) / 2;
	static const int Rolling_Speed = 3;

	int Meltdown_Y_Pos[Normal_Width * AsConfig::Global_Scale];
	int Inner_Width;
	int Rotation_Step;
};
//------------------------------------------------------------------------------------------------------------