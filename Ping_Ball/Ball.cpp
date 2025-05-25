#include "Ball.h"



//ABall
//------------------------------------------------------------------------------------------------------------
const double ABall::Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - Radius;
const double ABall::Start_Ball_X_Pos = (AsConfig::Border_X_Offset + AsConfig::Max_X_Pos) / 2 + 1;
const double ABall::Radius = 2.0;
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
    : Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0), Ball_Rect{}, Prev_Ball_Rect{}, Center_X_Pos(Start_Ball_X_Pos), Center_Y_Pos(190.0), Ball_Speed(6.0), Ball_Direction(M_PI_4), Rest_Distance(0.0)
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
    Ball_Rect.right = (int)((Center_X_Pos + Radius) * AsConfig::Global_Scale - 1);
    Ball_Rect.bottom = (int)((Center_Y_Pos + Radius) * AsConfig::Global_Scale - 1);

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
        Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);
    }

    if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
    {
        SelectObject(hdc, Ball_Pen);
        SelectObject(hdc, Ball_Brush);
        Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
    }
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(ALevel *level, int platform_x_pos, int platform_width, AHit_Checker *hit_checker)
{
    bool got_hit;
    double next_x_pos, next_y_pos;
    double size_step;

    if (Ball_State != EBS_Normal)
        return;

    int platform_y_pos = AsConfig::Platform_Y_Pos + 1;
    size_step = 1.0 / AsConfig::Global_Scale;
    Rest_Distance += Ball_Speed;

    while (Rest_Distance >= size_step)
    {
        got_hit = false;

        next_x_pos = Center_X_Pos + size_step * cos(Ball_Direction);
        next_y_pos = Center_Y_Pos - size_step * sin(Ball_Direction);
    
		got_hit = hit_checker->Check_Hit(next_x_pos, next_y_pos, this);

        if (got_hit)
            continue;

        Center_X_Pos = next_x_pos;
        Center_Y_Pos = next_y_pos;
        Rest_Distance -= size_step;
    }

    /*if (next_y_pos >= platform_y_pos and next_y_pos <= platform_y_pos + AsConfig::Platform_Height)
    {
        if (next_x_pos >= platform_x_pos and next_x_pos <= platform_x_pos + platform_width)
        {
            Ball_Direction = -Ball_Direction;
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
        }
    }

    level->Check_Level_Brick_Hit(next_x_pos, next_y_pos, Ball_Direction);*/

    //Ball_Direction = fmod(Ball_Direction, 2 * M_PI);

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
