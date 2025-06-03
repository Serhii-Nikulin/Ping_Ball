#pragma once

#include "Active_Brick.h"
#include "Ball.h"

//Bricks
enum ELetter_Type {ELT_None, ELT_O};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	void Act();
	bool Is_Finished();
	void Draw(HDC hdc, RECT &paint_area);

private:

	void Draw_Brick_Letter(HDC hdc);
	void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush) const;

	int X, Y;
	bool Got_Hit;
	int Rotation_Step;
	int Next_Rotation_Tick;
	const EBrick_Type Brick_Type;
	const ELetter_Type Letter_Type;
	RECT Letter_Cell, Prev_Letter_Cell;
	static const int Ticks_Per_Step = 4;

};
//------------------------------------------------------------------------------------------------------------
class ALevel: public AHit_Checker
{
public:
	ALevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Act();
	static const int Level_Height = 14;
	static const int Level_Width = 12;
	static unsigned char Level_01[ALevel::Level_Height][ALevel::Level_Width];

	void Set_Current_Level(unsigned char level[ALevel::Level_Height][ALevel::Level_Width]);

private:
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type) const;

	bool Check_Hit_From_Vertical(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance);
	bool Check_Hit_From_Horizontal(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distnace);

	void Add_Active_Brick(EBrick_Type brick_type, int brick_x, int brick_y);
	bool Add_Falling_Letter(EBrick_Type brick_type, int brick_x, int brick_y);
	void On_Hit(int brick_x, int brick_y);

	RECT Level_Rect{};

	unsigned char Current_Level[ALevel::Level_Height][ALevel::Level_Width];
 
	int Current_Brick_Left_Pos;
	int Current_Brick_Right_Pos;
	int Current_Brick_Top_Pos;
	int Current_Brick_Bottom_Pos;

	int Active_Bricks_Count;
	AActive_Brick *Active_Bricks[AsConfig::Max_Active_Bricks_Count];

	int Falling_Letter_Count;
	AFalling_Letter *Falling_Letters[AsConfig::Max_Falling_Letter_Count];
};
//------------------------------------------------------------------------------------------------------------