#pragma once

#include "Config.h"
#include "Ball.h"
#include "Falling_Letter.h"

enum EPlatform_State { EPS_Is_Ready, EPS_Normal, EPS_Meltdown, EPS_Missing, EPS_Rolling, EPS_Expanding_Rolling };
//------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker
{
public:
	~AsPlatform();
	AsPlatform();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	void Move(bool to_left);
	void Act();

	EPlatform_State Get_State() const;
	void Set_State(EPlatform_State new_state);

	void Redraw();
	void Draw(HDC hdc, RECT &paint_area);
	bool Hit_By(AFalling_Letter *falling_letter);

	int X_Pos;
	int X_Step;
	int Width;

private:
	void Clear_BG(HDC hdc) const;
	void Draw_Circle(HDC hdc, int x, int y);
	void Draw_Circle_Highlight(HDC hdc, int x, int y) const;
	void Draw_Normal_State(HDC hdc);
	void Draw_Meltdown_State(HDC hdc);
	void Draw_Rolling_State(HDC hdc);
	void Draw_Expanding_Rolling_State(HDC hdc);

	bool Reflect_From_Center(double next_x_pos, double next_y_pos, ABall *ball);
	bool Reflect_From_Circle(double next_x_pos, double next_y_pos, ABall *ball, double circle_x_pos);

	bool Get_Platform_Image_Stroke_Color(int x, int y, int &len, const AColor **pen_color);

	EPlatform_State Platform_State;
	RECT Platform_Rect, Prev_Platform_Rect;
	AColor Highlight_Color, Platform_Side_Color, Platform_Inner_Color;

	static const int Height = 7;
	static const int Circle_Size = 7;
	static const int Normal_Width = 28;
	static const int Meltdown_Speed = 4;
	static const int Rolling_Speed = 3;
	static const int Max_Rotation_Step = 16;
	static const int Finish_Rolling_X_Pos = (AsConfig::Max_X_Pos + 1 + AsConfig::Border_X_Offset + 1 - Circle_Size) / 2;

	int Inner_Width;
	int Rotation_Step;
	int Meltdown_Y_Pos[Normal_Width * AsConfig::Global_Scale];

	int *Normal_Platform_Image;
	int Normal_Platform_Image_Width;
	int Normal_Platform_Image_Height;
};
//------------------------------------------------------------------------------------------------------------