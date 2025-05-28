#pragma once

#include "Active_Brick.h"
#include "Ball.h"

//Bricks
enum ELetter_Type {ELT_None, ELT_O};

//------------------------------------------------------------------------------------------------------------
class ALevel: public AHit_Checker
{
public:
	ALevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	bool Hit_Circle_On_Line(double distance, double position, double min, double max, double radius, double &value_pos);

	void Init();
	void Draw(HDC hdc, RECT &paint_area);

	static const int Level_Height = 14;
	static const int Level_Width = 12;
	static unsigned char Level_01[ALevel::Level_Height][ALevel::Level_Width];

	void Set_Current_Level(unsigned char level[ALevel::Level_Height][ALevel::Level_Width]);

	AActive_Brick Active_Brick;

private:
	void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush) const;
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type) const;

	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distnace);


	RECT Level_Rect{};
	HPEN Letter_Pen;
	HPEN Brick_Red_Pen, Brick_Blue_Pen;
	HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	unsigned char Current_Level[ALevel::Level_Height][ALevel::Level_Width];
 
	int Current_Brick_Left_Pos;
	int Current_Brick_Right_Pos;
	int Current_Brick_Top_Pos;
	int Current_Brick_Bottom_Pos;
};
//------------------------------------------------------------------------------------------------------------