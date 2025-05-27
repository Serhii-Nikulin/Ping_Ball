#pragma once

#include "Config.h"

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
	void Move();
	EBall_State Get_State() const;
	void Set_State(EBall_State new_state);
	static void Add_Hit_Checker(AHit_Checker* hit_checker);

	static const double Radius;
	void Set_Direction(double new_direction);
	double Get_Direction() const;
	bool Reflect(bool is_horizontal_hit);

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
	double Ball_Direction;
	double Rest_Distance;

	static int Hit_Checker_Count;
	static AHit_Checker* Hit_Checkers[3];
};
//------------------------------------------------------------------------------------------------------------