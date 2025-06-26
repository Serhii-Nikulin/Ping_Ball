#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State { EBS_Normal, EBS_Lost, EBS_On_Platform, EBS_Off_Parachute, EBS_On_Parachute };
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
	void Draw(HDC hdc, RECT &paint_area);
	void Move();
	EBall_State Get_State() const;
	void Set_State(EBall_State new_state, double ball_x_pos = Start_Ball_X_Pos, double ball_y_pos = Start_Ball_Y_Pos);

	double Get_Direction() const;
	void Set_Direction(double new_direction);
	void Reflect(bool is_hit_from_horizontal);
	bool Is_Moving_Up() const;
	bool Is_Moving_Left() const;

	static void Add_Hit_Checker(AHit_Checker* hit_checker);

	void Set_On_Parachute(int brick_x, int brick_y);

	double Ball_Speed;
	static const double Radius;
private:

	void Draw_Parachute(HDC hdc, RECT &paint_area);
	void Redraw_Parachute();
	void Clear_Parachute(HDC hdc);

	EBall_State Ball_State, Prev_Ball_State;

	RECT Ball_Rect, Prev_Ball_Rect;
	RECT Parachute_Rect, Prev_Parachute_Rect;

	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Direction;
	double Rest_Distance;

	static const double Start_Ball_X_Pos;
	static const double Start_Ball_Y_Pos;
	static int Hit_Checker_Count;
	static AHit_Checker* Hit_Checkers[3];

	static const int Parachute_Size = 15;
};
//------------------------------------------------------------------------------------------------------------