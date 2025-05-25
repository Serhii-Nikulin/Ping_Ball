#pragma once

#include "Config.h"
#include "Level.h"

enum EBall_State { EBS_Normal, EBS_Lost, EBS_On_Platform };

class ABall;
//------------------------------------------------------------------------------------------------------------
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double y_pos, ABall *ball) = 0;
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Redraw();
	void Draw(HDC hdc, RECT &paint_area) const;
	void Move(ALevel *level, int platform_x_pos, int platform_width, AHit_Checker *hit_checker);
	EBall_State Get_State() const;
	void Set_State(EBall_State new_state);

	static const double Radius;
	double Ball_Direction;

private:
	static const double Start_Ball_X_Pos;
	static const double Start_Ball_Y_Pos;

	EBall_State Ball_State;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;

	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Speed;
	double Rest_Distance;
};
//------------------------------------------------------------------------------------------------------------