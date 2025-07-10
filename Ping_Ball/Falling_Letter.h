#pragma once
#include "Active_Brick.h"

//Bricks
enum ELetter_Type 
{
	ELT_X, // cancel effects
	ELT_R, // reverse
	ELT_S, // slow ball
	ELT_M, // kill monster
	ELT_HP, // get one more life
	ELT_W, // wide platform
	ELT_C, // clay platform 7

	ELT_T, // triple ball 3
	ELT_L, // lazer platform 3
	ELT_F, // floor 3

	ELT_Plus, // next level 1

	ELT_Max
};
//------------------------------------------------------------------------------------------------------------
enum EFalling_Letter_State {EFLS_Normal, EFLS_Finalizing, EFLS_Finished};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter: public AGraphics_Object
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Get_Letter_Cell(RECT &letter_rect);
	void Finalize();
	void Test_Draw_All_Steps(HDC hdc);

	static void Init();
	static ELetter_Type Get_Random_Letter_Type();

	const ELetter_Type Letter_Type;
private:

	void Draw_Brick_Letter(HDC hdc);
	void Set_Brick_Letter_Color(bool is_switch_color, const AColor **front_color, const AColor **back_color) const;
	void Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2);

	int X, Y;
	bool Got_Hit;
	int Rotation_Step;
	int Next_Rotation_Tick;
	const EBrick_Type Brick_Type;
	RECT Letter_Cell, Prev_Letter_Cell;
	EFalling_Letter_State Falling_Letter_State;
	static const int Ticks_Per_Step = 4;
	static const int Max_Rotation_Step = 16;
	static const int Brick_Half_Height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
	static int Letters_Popularity[ELT_Max];
	static int All_Letters_Popularity;
};
//------------------------------------------------------------------------------------------------------------