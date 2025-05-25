#include "Ball.h"

//ABall
//------------------------------------------------------------------------------------------------------------
const double ABall::Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - Ball_Size;
const double ABall::Start_Ball_X_Pos = (AsConfig::Border_X_Offset + AsConfig::Max_X_Pos - Ball_Size) / 2 + 1;
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
    : Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0), Ball_Rect{}, Prev_Ball_Rect{}, Ball_X_Pos(Start_Ball_X_Pos), Ball_Y_Pos(190.0), Ball_Speed(6.0), Ball_Direction(M_PI_4)
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

    Ball_Rect.left = (int)(Ball_X_Pos * AsConfig::Global_Scale);
    Ball_Rect.top = (int)(Ball_Y_Pos * AsConfig::Global_Scale);
    Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale - 1;
    Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale - 1;

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
void ABall::Move(ALevel *level, int platform_x_pos, int platform_width)
{
    double next_x_pos, next_y_pos;

    if (Ball_State != EBS_Normal)
        return;

    next_x_pos = Ball_X_Pos + Ball_Speed * cos(Ball_Direction);
    next_y_pos = Ball_Y_Pos - Ball_Speed * sin(Ball_Direction);

	int min_x_pos = AsConfig::Border_X_Offset;
	int max_x_pos = AsConfig::Max_X_Pos - Ball_Size + 1;
	int min_y_pos = AsConfig::Border_Y_Offset;
	int max_y_pos = AsConfig::Max_Y_Pos - Ball_Size + 1;

    int platform_y_pos = AsConfig::Platform_Y_Pos - Ball_Size + 1;

    if (next_x_pos > max_x_pos)//right limit
    {
        Ball_Direction = M_PI - Ball_Direction;
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
    }

	if (next_y_pos < min_y_pos)//top limit
	{
		Ball_Direction = -Ball_Direction;
		next_y_pos = min_y_pos + (min_y_pos - next_y_pos);
	}

    if (next_x_pos < min_x_pos)//left limit
    {
        Ball_Direction = M_PI - Ball_Direction;
        next_x_pos = min_x_pos + (min_x_pos - next_x_pos);
    }

    if (next_y_pos > max_y_pos)//bottom limit
    {
        if (AsConfig::Has_Floor)
        {
            Ball_Direction = -Ball_Direction;
            next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
        }
        else
            if (next_y_pos > max_y_pos + Ball_Size)
                Ball_State = EBS_Lost;
    }
    

    if (next_y_pos >= platform_y_pos and next_y_pos <= platform_y_pos + AsConfig::Platform_Height)
    {
        if (next_x_pos >= platform_x_pos and next_x_pos <= platform_x_pos + platform_width)
        {
            Ball_Direction = -Ball_Direction;
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
        }
    }

    level->Check_Level_Brick_Hit(next_x_pos, next_y_pos, Ball_Direction);

    //Ball_Direction = fmod(Ball_Direction, 2 * M_PI);

    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;

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
        Ball_X_Pos = Start_Ball_X_Pos;
        Ball_Y_Pos = Start_Ball_Y_Pos;        
        Redraw();
        break;

    case EBS_Lost:
        Ball_Speed = 0.0;
        break;

    case EBS_On_Platform:
        Ball_Speed = 0.0;
        Ball_X_Pos = Start_Ball_X_Pos;
        Ball_Y_Pos = Start_Ball_Y_Pos;
        Redraw();
        break;

    }

	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
