#pragma once
#include "Active_Brick.h"

//Bricks
enum ELetter_Type {ELT_None, ELT_S, ELT_L, ELT_HP, ELT_T, ELT_X, ELT_C, ELT_W, ELT_M, ELT_F, ELT_R, ELT_Plus};
enum EFalling_Letter_State {EFLS_Normal, EFLS_Finalizing, EFLS_Finished};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter: public AGraphics_Object
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Get_Letter_Cell(RECT &letter_rect);
	void Finalize();
	void Test_Draw_All_Steps(HDC hdc);

private:

	void Draw_Brick_Letter(HDC hdc);
	void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush) const;
	void Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2);

	int X, Y;
	bool Got_Hit;
	int Rotation_Step;
	int Next_Rotation_Tick;
	const EBrick_Type Brick_Type;
	const ELetter_Type Letter_Type;
	RECT Letter_Cell, Prev_Letter_Cell;
	static const int Ticks_Per_Step = 4;
	static const int Max_Rotation_Step = 16;
	static const int Brick_Half_Height = (int)(AsConfig::Brick_Height * AsConfig::Global_Scale / 2.0);
	EFalling_Letter_State Falling_Letter_State;
};
//------------------------------------------------------------------------------------------------------------