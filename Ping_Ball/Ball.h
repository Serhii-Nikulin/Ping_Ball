#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State { EBS_Normal, EBS_Lost, EBS_On_Platform, EBS_Off_Parachute, EBS_On_Parachute, EBS_Teleporting };
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

	void Move();
	void Draw(HDC hdc, RECT &paint_area);
	void Draw_Teleporting(HDC hdc, int step) const;
	EBall_State Get_State() const;
	void Set_On_Parachute(int brick_x, int brick_y);
	void Set_State(EBall_State new_state, double ball_x_pos = Start_Ball_X_Pos, double ball_y_pos = Start_Ball_Y_Pos);

	bool Is_Moving_Up() const;
	bool Is_Moving_Left() const;
	double Get_Direction();
	void Set_Direction(double new_direction);
	void Reflect(bool is_hit_from_horizontal);

	void Get_Center(double &center_x_pos, double &center_y_pos);

	static void Add_Hit_Checker(AHit_Checker* hit_checker);

	static const double Radius;

private:
	void Redraw();
	void Draw_Parachute(HDC hdc, RECT &paint_area);
	void Redraw_Parachute();
	void Clear_Parachute(HDC hdc);

	RECT Ball_Rect, Prev_Ball_Rect;
	RECT Parachute_Rect, Prev_Parachute_Rect, Before_Teleport_Rect;
	EBall_State Ball_State, Prev_Ball_State;

	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Speed;
	double Ball_Direction;
	double Rest_Distance;

	static const double Start_Ball_X_Pos;
	static const double Start_Ball_Y_Pos;
	static const int Parachute_Size = 15;

	static int Hit_Checker_Count;
	static AHit_Checker* Hit_Checkers[3];
};
//------------------------------------------------------------------------------------------------------------