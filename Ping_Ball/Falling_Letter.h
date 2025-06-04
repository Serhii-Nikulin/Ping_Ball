#pragma once
#include "Active_Brick.h"

//Bricks
enum ELetter_Type {ELT_None, ELT_O};

//------------------------------------------------------------------------------------------------------------
class AFalling_Letter: public AGraphics_Object
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

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