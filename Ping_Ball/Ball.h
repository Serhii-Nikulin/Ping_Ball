#pragma once

#include "Config.h"
#include "Level.h"

enum EBall_State { EBS_Normal, EBS_Lost, EBS_On_Platform };
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Redraw();
	void Draw(HDC hdc, RECT &paint_area) const;
	void Move(ALevel *level, int platform_x_pos, int platform_width);
	EBall_State Get_State() const;
	void Set_State(EBall_State new_state);

private:
	static const int Ball_Size = 4;
	static const double Start_Ball_X_Pos;
	static const double Start_Ball_Y_Pos;

	EBall_State Ball_State;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;


	double Ball_X_Pos;
	double Ball_Y_Pos;
	double Ball_Speed, Ball_Direction;
};
//------------------------------------------------------------------------------------------------------------