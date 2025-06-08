#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State { EBS_Normal, EBS_Lost, EBS_On_Platform };
//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double y_pos, ABall *ball) = 0;
	bool Hit_Circle_On_Line(double distance, double position, double min, double max, double radius, double &value_pos);
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Redraw();
	void Draw(HDC hdc, RECT &paint_area) const;
	void Move();
	EBall_State Get_State() const;
	void Set_State(EBall_State new_state);

	double Get_Direction() const;
	void Set_Direction(double new_direction);
	void Reflect(bool is_hit_from_horizontal);
	bool Is_Moving_Up() const;
	bool Is_Moving_Left() const;

	static void Add_Hit_Checker(AHit_Checker* hit_checker);

	double Ball_Speed;
	static const double Radius;
private:

	EBall_State Ball_State;

	RECT Ball_Rect, Prev_Ball_Rect;

	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Direction;
	double Rest_Distance;

	static const double Start_Ball_X_Pos;
	static const double Start_Ball_Y_Pos;
	static int Hit_Checker_Count;
	static AHit_Checker* Hit_Checkers[3];
};
//------------------------------------------------------------------------------------------------------------