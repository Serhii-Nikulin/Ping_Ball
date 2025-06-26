#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double distance, double position, double min, double max, double radius, double& value_pos)
{
	double min_pos, max_pos;

	if (distance > radius)
		return false;

	value_pos = sqrt(radius * radius - distance * distance);
	min_pos = position - value_pos;
	max_pos = position + value_pos;

	if ((min_pos >= min and min_pos <= max) or (max_pos > min and max_pos < max))
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------




//ABall
const double ABall::Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
const double ABall::Start_Ball_X_Pos = (AsConfig::Border_X_Offset + AsConfig::Max_X_Pos) / 2 + 1;
const double ABall::Radius = 2.0 - 0.5 / AsConfig::Global_Scale;
int ABall::Hit_Checker_Count = 0;
AHit_Checker* ABall::Hit_Checkers[];
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_State(EBS_Normal), Prev_Ball_State(EBS_Lost), Ball_Rect{}, Prev_Ball_Rect{}, Center_X_Pos(Start_Ball_X_Pos), Center_Y_Pos(Start_Ball_Y_Pos), Ball_Speed(6.0), Ball_Direction(M_PI_4), Rest_Distance(0.0), Parachute_Rect{}, Prev_Parachute_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw()
{
	Prev_Ball_Rect = Ball_Rect;

	Ball_Rect.left = (int)((Center_X_Pos - Radius) * AsConfig::Global_Scale);
	Ball_Rect.top = (int)((Center_Y_Pos - Radius) * AsConfig::Global_Scale);
	Ball_Rect.right = (int)((Center_X_Pos + Radius) * AsConfig::Global_Scale);
	Ball_Rect.bottom = (int)((Center_Y_Pos + Radius) * AsConfig::Global_Scale);

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect{};

	if (Ball_State == EBS_Teleporting)
		return;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
	{
		AsConfig::BG_Color.Select(hdc);
		Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	}

	switch (Ball_State)
	{
	case EBS_Lost:
		if (Prev_Ball_State == EBS_On_Parachute)
		{
			Clear_Parachute(hdc);
			Prev_Ball_State = EBS_Lost;
		}
		return;

	case EBS_On_Parachute:
		Draw_Parachute(hdc, paint_area);
		break;

	case EBS_Off_Parachute:
		Clear_Parachute(hdc);
		Set_State(EBS_Normal, Center_X_Pos, Center_Y_Pos);
		break;

	}

	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
	{
		AsConfig::White_Color.Select(hdc);
		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw_Teleporting(HDC hdc, int step) const
{
	int top_y, low_y;

	top_y = Ball_Rect.top + step / 2;
	low_y = Ball_Rect.bottom - 1 - step / 2;

	if (top_y > low_y)
		return;

	AsConfig::White_Color.Select(hdc);
	Ellipse(hdc, Ball_Rect.left, top_y, Ball_Rect.right - 1, low_y);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move()
{
	int i;
	bool got_hit;
	double next_x_pos, next_y_pos;

	if (Ball_State == EBS_Lost or Ball_State == EBS_On_Platform or Ball_State == EBS_Teleporting)
		return;

	Rest_Distance += Ball_Speed;

	while (Rest_Distance >= AsConfig::Moving_Size_Step)
	{
		got_hit = false;

		next_x_pos = Center_X_Pos + AsConfig::Moving_Size_Step * cos(Ball_Direction);
		next_y_pos = Center_Y_Pos - AsConfig::Moving_Size_Step * sin(Ball_Direction);

		for (i = 0; i < Hit_Checker_Count; i++)
			got_hit |= Hit_Checkers[i]->Check_Hit(next_x_pos, next_y_pos, this);

		if (got_hit)
			continue;

		Center_X_Pos = next_x_pos;
		Center_Y_Pos = next_y_pos;
		Rest_Distance -= AsConfig::Moving_Size_Step;

		if (Ball_State == EBS_Lost)
			break;
	}

	Redraw();

	if (Ball_State == EBS_On_Parachute)
	{
		Prev_Parachute_Rect = Parachute_Rect;

		Parachute_Rect.bottom = Ball_Rect.bottom;
		Parachute_Rect.top = Ball_Rect.bottom - Parachute_Size * AsConfig::Global_Scale;

		Redraw_Parachute();
	}
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State() const
{
	return Ball_State;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, double ball_x_pos, double ball_y_pos)
{
	switch (new_state)
	{
	case EBS_Normal:
		Ball_Speed = 6.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI_4;
		Center_X_Pos = ball_x_pos;
		Center_Y_Pos = ball_y_pos;
		
		if (Ball_State == EBS_Off_Parachute)
			Ball_Direction = M_PI / 3.0 * (1 + AsConfig::Rand(2) );

		Redraw();
		break;

	case EBS_Lost:
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;

		if (Ball_State == EBS_On_Parachute)
			Redraw_Parachute();

		break;

	case EBS_On_Platform:
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Center_X_Pos = ball_x_pos;
		Center_Y_Pos = ball_y_pos;
		Redraw();
		break;

	case EBS_Off_Parachute:
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Redraw();
		Redraw_Parachute();
		break;

	case EBS_Teleporting:
		if (!(Ball_State == EBS_Normal or Ball_State == EBS_Teleporting) )
			return;

		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Center_X_Pos = ball_x_pos;
		Center_Y_Pos = ball_y_pos;
		Redraw();

		if (Ball_State == EBS_On_Parachute)
			Redraw_Parachute();
	}

	Prev_Ball_State = Ball_State;
	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
double ABall::Get_Direction() const
{
	return Ball_Direction;
}
//------------------------------------------------------------------------------------------------------------

void ABall::Set_Direction(double new_direction)
{
	const double pi_2 = M_PI * 2;

	while (new_direction >= pi_2)
		new_direction -= pi_2;

	while (new_direction < 0)
		new_direction += pi_2;

	Ball_Direction = new_direction;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Reflect(bool is_hit_from_horizontal)
{
	if (is_hit_from_horizontal)
		Set_Direction(-Ball_Direction);
	else
		Set_Direction(M_PI - Ball_Direction);
}
//------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Up() const
{
	if (Ball_Direction > 0 and Ball_Direction < M_PI)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Left() const
{
	if (Ball_Direction > M_PI_2 and Ball_Direction < M_PI + M_PI_2)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Add_Hit_Checker(AHit_Checker* hit_checker)
{
	if (Hit_Checker_Count >= sizeof(Hit_Checkers) / sizeof(Hit_Checkers[0]))
		return;

	Hit_Checkers[Hit_Checker_Count++] = hit_checker;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_On_Parachute(int brick_x, int brick_y)
{
	int x_pos, y_pos;

	Ball_State = EBS_On_Parachute;
	Ball_Direction = M_PI + M_PI_2;
	Rest_Distance = 0.0;
	Ball_Speed = AsConfig::Global_Scale / 2.0;

	x_pos = AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width;
	y_pos = AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height;

	Parachute_Rect.left = x_pos * AsConfig::Global_Scale;
	Parachute_Rect.top = y_pos * AsConfig::Global_Scale;
	Parachute_Rect.right = Parachute_Rect.left + Parachute_Size * AsConfig::Global_Scale;
	Parachute_Rect.bottom = Parachute_Rect.top + Parachute_Size * AsConfig::Global_Scale;

	Prev_Parachute_Rect = Parachute_Rect;

	Center_X_Pos = x_pos + AsConfig::Brick_Width / 2.0;
	Center_Y_Pos = y_pos + Parachute_Size - Radius + 0.5 / AsConfig::Global_Scale;

	Redraw_Parachute();
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw_Parachute(HDC hdc, RECT& paint_area)
{
	RECT rect{};
	RECT intersection_rect{};
	const int& scale = AsConfig::Global_Scale;
	int center_x_pos = (int)(Center_X_Pos * scale);
	int center_y_pos = (int)(Center_Y_Pos * scale);

	if (!IntersectRect(&intersection_rect, &paint_area, &Parachute_Rect))
		return;

	Clear_Parachute(hdc);

	AsConfig::Blue_Color.Select(hdc);
	Chord(hdc, Parachute_Rect.left, Parachute_Rect.top, Parachute_Rect.right - 1, Parachute_Rect.bottom,
		Parachute_Rect.right - 1, Parachute_Rect.top + Parachute_Size * scale / 2,
		Parachute_Rect.left, Parachute_Rect.top + Parachute_Size * scale / 2);

	AsConfig::BG_Color.Select(hdc);

	rect.left = Parachute_Rect.left;
	rect.top = Parachute_Rect.top + 6 * scale;
	rect.right = rect.left + 5 * scale;
	rect.bottom = rect.top + 4 * scale;
	Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

	rect.left = Parachute_Rect.left + 4 * scale;
	rect.top = Parachute_Rect.top + 5 * scale;
	rect.right = rect.left + 7 * scale;
	rect.bottom = rect.top + 6 * scale;
	Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

	rect.left = Parachute_Rect.left + 10 * scale;
	rect.top = Parachute_Rect.top + 6 * scale;
	rect.right = rect.left + 5 * scale;
	rect.bottom = rect.top + 4 * scale;
	Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

	AsConfig::White_Color.Select_Pen(hdc);
	MoveToEx(hdc, Parachute_Rect.left + 1, Parachute_Rect.top + Parachute_Size * scale / 2, 0);
	LineTo(hdc, center_x_pos, center_y_pos);

	MoveToEx(hdc, Parachute_Rect.left + 1 + 4 * scale, Parachute_Rect.top + Parachute_Size * scale / 2 - scale, 0);
	LineTo(hdc, center_x_pos, center_y_pos);

	MoveToEx(hdc, Parachute_Rect.right + 1 - scale, Parachute_Rect.top + Parachute_Size * scale / 2, 0);
	LineTo(hdc, center_x_pos, center_y_pos);

	MoveToEx(hdc, Parachute_Rect.right + 1 - 5 * scale, Parachute_Rect.top + Parachute_Size * scale / 2 - scale, 0);
	LineTo(hdc, center_x_pos, center_y_pos);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Parachute()
{
	InvalidateRect(AsConfig::Hwnd, &Prev_Parachute_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Parachute_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Clear_Parachute(HDC hdc)
{
	AsConfig::BG_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, Prev_Parachute_Rect, 0);
}
//------------------------------------------------------------------------------------------------------------