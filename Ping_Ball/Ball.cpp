#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double distance, double position, double min, double max, double radius, double &value_pos)
{
    double min_pos, max_pos;

    if (distance > radius)
        return false;

    value_pos = sqrt(radius * radius - distance * distance);
    min_pos = position - value_pos;
    max_pos = position + value_pos;

    if ( (min_pos >= min and min_pos <= max) or (max_pos > min and max_pos < max) )
        return true;

    return false;
}
//------------------------------------------------------------------------------------------------------------




//ABall
const double ABall::Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - Radius;
const double ABall::Start_Ball_X_Pos = (AsConfig::Border_X_Offset + AsConfig::Max_X_Pos) / 2 + 1;
const double ABall::Radius = 2.0 - 0.5 / AsConfig::Global_Scale;
int ABall::Hit_Checker_Count = 0;
AHit_Checker* ABall::Hit_Checkers[];
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
    : Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0), Ball_Rect{}, Prev_Ball_Rect{}, Center_X_Pos(Start_Ball_X_Pos), Center_Y_Pos(Start_Ball_Y_Pos), Ball_Speed(6.0), Ball_Direction(M_PI_4), Rest_Distance(0.0)
{}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
    AsConfig::Create_Pen_Brush(Ball_Pen, Ball_Brush, 255, 255, 255);
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
void ABall::Draw(HDC hdc, RECT &paint_area) const
{
    RECT intersection_rect{};

    if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect) )
    {
        SelectObject(hdc, AsConfig::BG_Pen);
        SelectObject(hdc, AsConfig::BG_Brush);
        Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
    }

    if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
    {
        SelectObject(hdc, Ball_Pen);
        SelectObject(hdc, Ball_Brush);
        Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
    }
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move()
{
    int i;
    bool got_hit;
    double next_x_pos, next_y_pos;

    if (Ball_State != EBS_Normal)
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
    }

    Redraw();
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State() const
{
	return Ball_State;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state)
{
    switch (new_state)
    {
    case EBS_Normal:
        Ball_Speed = 6.0;
        Ball_Direction = M_PI_4;
        Center_X_Pos = Start_Ball_X_Pos;
        Center_Y_Pos = Start_Ball_Y_Pos;   
        Rest_Distance = 0.0;
        Redraw();
        break;

    case EBS_Lost:
        Ball_Speed = 0.0;
        break;

    case EBS_On_Platform:
        Ball_Speed = 0.0;
        Center_X_Pos = Start_Ball_X_Pos;
        Center_Y_Pos = Start_Ball_Y_Pos;
        Rest_Distance = 0.0;
        Redraw();
        break;

    }

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
    if (Hit_Checker_Count >= sizeof(Hit_Checkers) / sizeof(Hit_Checkers[0]) )
        return;

    Hit_Checkers[Hit_Checker_Count++] = hit_checker;
}
//------------------------------------------------------------------------------------------------------------