#include "Ball.h"

//ABall
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
    :Ball_Pen(0), Ball_Brush(0), Ball_Rect{}, Prev_Ball_Rect{}, Ball_X_Pos(60), Ball_Y_Pos(190), Ball_Speed(6.0), Ball_Direction(M_PI_4)
{}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
    AsConfig::Create_Pen_Brush(Ball_Pen, Ball_Brush, 255, 255, 255);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw(HWND hwnd)
{
    Prev_Ball_Rect = Ball_Rect;

    Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
    Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
    Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale - 1;
    Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale - 1;

    InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area, HPEN &bg_pen, HBRUSH &bg_brush)
{
    RECT intersection_rect{};

    if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
        return;

    SelectObject(hdc, bg_pen);
    SelectObject(hdc, bg_brush);
    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

    SelectObject(hdc, Ball_Pen);
    SelectObject(hdc, Ball_Brush);
    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(HWND hwnd, ALevel *level, AsPlatform *platform)
{
    int next_x_pos, next_y_pos;

    next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
    next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

    int min_x_pos = AsBorder::Border_X_Offset;
    int max_x_pos = AsBorder::Max_X_Pos - Ball_Size + 1;
    int min_y_pos = AsBorder::Border_Y_Offset;
    int max_y_pos = AsBorder::Max_Y_Pos - Ball_Size + 1;

    int platform_y_pos = AsPlatform::Y_Pos - Ball_Size + 1;

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
        Ball_Direction = -Ball_Direction;
        next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
    }

    if (next_y_pos >= platform_y_pos and next_y_pos <= platform_y_pos + AsPlatform::Height)
    {
        if (next_x_pos >= platform->X_Pos and next_x_pos <= platform->X_Pos + platform->Width)
        {
            Ball_Direction = -Ball_Direction;
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
        }
    }

    level->Check_Level_Brick_Hit(next_x_pos, next_y_pos, Ball_Direction);

    //Ball_Direction = fmod(Ball_Direction, 2 * M_PI);

    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;

    Redraw(hwnd);
}
//------------------------------------------------------------------------------------------------------------