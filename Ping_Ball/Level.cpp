#include "Level.h"

//------------------------------------------------------------------------------------------------------------
unsigned char ALevel::Level_01[ALevel::Level_Height][ALevel::Level_Width] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//------------------------------------------------------------------------------------------------------------
//ALevel
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
    : Active_Brick(EBT_Red), Level_Rect{}, Letter_Pen(0), Brick_Red_Pen(0), Brick_Blue_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0),
    Current_Brick_Left_Pos(0), Current_Brick_Right_Pos(0), Current_Brick_Top_Pos(0), Current_Brick_Bottom_Pos(0), Current_Level{}
{
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
    int i, j;
    int min_ball_x, max_ball_x;
    int min_ball_y, max_ball_y;
    int min_level_x, max_level_x;
    int min_level_y, max_level_y;

    bool is_hit_from_vertical;
    bool is_hit_from_horizontal;
    double distance_x, distance_y;

    double radius = ball->Radius;
	
	min_ball_x = (int)(next_x_pos - radius);
	max_ball_x = (int)(next_x_pos + radius);
	min_ball_y = (int)(next_y_pos - radius);
	max_ball_y = (int)(next_y_pos + radius);

	if (max_ball_y > AsConfig::Level_Y_Offset + (Level_Height - 1) * Cell_Height + AsConfig::Brick_Height)
		return false;

    min_level_x = (min_ball_x - AsConfig::Level_X_Offset) / Cell_Width;
    max_level_x = (max_ball_x - AsConfig::Level_X_Offset) / Cell_Width;
    min_level_y = (min_ball_y - AsConfig::Level_Y_Offset) / Cell_Height;
    max_level_y = (max_ball_y - AsConfig::Level_Y_Offset) / Cell_Height;


    for (i = max_level_y; i >= min_level_y; i--)
        for (j = min_level_x; j <= max_level_x; j++)
        {
            if(Level_01[i][j] == 0)
                continue;

            Current_Brick_Left_Pos = AsConfig::Level_X_Offset + j * Cell_Width;
            Current_Brick_Right_Pos = Current_Brick_Left_Pos + AsConfig::Brick_Width;
            Current_Brick_Top_Pos = AsConfig::Level_Y_Offset + i * Cell_Height;
            Current_Brick_Bottom_Pos = Current_Brick_Top_Pos + AsConfig::Brick_Height;

			is_hit_from_vertical = Check_Hit_From_Vertical(next_x_pos, next_y_pos, ball, j, i, distance_y);
			is_hit_from_horizontal = Check_Hit_From_Horizontal(next_x_pos, next_y_pos, ball, j, i, distance_x);

            if (is_hit_from_vertical and is_hit_from_horizontal)
            {
                if(distance_x < distance_y)
                {
					ball->Reflect(true);
					return true;
				}
				else
				{
					ball->Reflect(false);
					return true;
                }
            }
			else if (is_hit_from_vertical)
			{
				ball->Reflect(false);
				return true;
			}
			else if (is_hit_from_horizontal)
			{
				ball->Reflect(true);
				return true;
			}
        }

    return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit_From_Horizontal(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance)
{
    double direction = ball->Get_Direction();
    double radius = ball->Radius;

    // Hit from bottom side of the brick
    if (ball->Is_Moving_Up() )
    {
        if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Bottom_Pos, next_x_pos, Current_Brick_Left_Pos, Current_Brick_Right_Pos, radius, distance) )
        {
            if (brick_y == Level_Height - 1 or (brick_y < Level_Height - 1 and Current_Level[brick_y + 1][brick_x] == 0) )
                return true;
            else
                return false;
        }
    }
    else // Hit from top side of the brick
    {
        if (Hit_Circle_On_Line(Current_Brick_Top_Pos - next_y_pos, next_x_pos, Current_Brick_Left_Pos, Current_Brick_Right_Pos, radius, distance) )
        {
            if (brick_y > 0 and Current_Level[brick_y - 1][brick_x] == 0)
                return true;
            else
                return false;
        }
    }
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit_From_Vertical(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance)
{
    double direction = ball->Get_Direction();
    double radius = ball->Radius;

    // Hit from right side of the brick
    if (ball->Is_Moving_Left() )
    {
        if (Hit_Circle_On_Line(next_x_pos - Current_Brick_Right_Pos, next_y_pos, Current_Brick_Top_Pos, Current_Brick_Bottom_Pos, radius, distance) )
        {
            if (brick_x < Level_Width - 1 and Current_Level[brick_y][brick_x + 1] == 0)
                return true;
            else
                return false;
        }
    }
    else // Hit from left side of the brick
    {
        if (Hit_Circle_On_Line(Current_Brick_Left_Pos - next_x_pos, next_y_pos, Current_Brick_Top_Pos, Current_Brick_Bottom_Pos, radius, distance) )
        {
            if (brick_x > 0 and Current_Level[brick_y][brick_x - 1] == 0)
                return true;
            else
                return false;
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
    Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
    Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
    Level_Rect.right = Level_Rect.left + Level_Width * Cell_Width * AsConfig::Global_Scale;
    Level_Rect.bottom = Level_Rect.top + Level_Height * Cell_Height * AsConfig::Global_Scale;

    Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255) );

    AsConfig::Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush, AsConfig::Red_Brick_Color);
    AsConfig::Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush, AsConfig::Blue_Brick_Color);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Current_Level(unsigned char level[ALevel::Level_Height][ALevel::Level_Width])
{
    memcpy(Current_Level, level, sizeof(Current_Level) );
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush) const
{
    if (is_switch_color == true)
    {
        front_pen = Brick_Blue_Pen;
        front_brush = Brick_Blue_Brush;

        back_pen = Brick_Red_Pen;
        back_brush = Brick_Red_Brush;
    }
    else
    {
        front_pen = Brick_Red_Pen;
        front_brush = Brick_Red_Brush;

        back_pen = Brick_Blue_Pen;
        back_brush = Brick_Blue_Brush;
    }

}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    XFORM old_xform{}, new_xform{};
    int brick_half_height;
    double rotation_angle;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;

    brick_half_height = (int)(AsConfig::Brick_Height * AsConfig::Global_Scale / 2.0);

    if (rotation_step >= 5 and rotation_step <= 12)
    {
        if (brick_type == EBT_Blue)
            switch_color = false;
        else
            switch_color = true;
    }
    else
    {
        if (brick_type == EBT_Blue)
            switch_color = true;
        else
            switch_color = false;
    }

    Set_Brick_Letter_Color(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (rotation_step == 4 or rotation_step == 12)
    {
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height);


        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height + AsConfig::Global_Scale - 1);
        return;
    }

    SetGraphicsMode(hdc, GM_ADVANCED);
    GetWorldTransform(hdc, &old_xform);

    rotation_angle = rotation_step * 2.0 * M_PI / 16.0;
    new_xform.eM11 = (FLOAT)1;
    new_xform.eM12 = (FLOAT)0;

    new_xform.eM21 = (FLOAT)0;
    new_xform.eM22 = (FLOAT)cos(rotation_angle);

    new_xform.eDx = (FLOAT)x;
    new_xform.eDy = (FLOAT)(y + brick_half_height);

    int offset = (int)round(sin(rotation_angle) * AsConfig::Global_Scale * 3.0);
    SetWorldTransform(hdc, &new_xform);


    SelectObject(hdc, back_pen);
    SelectObject(hdc, back_brush);

    Rectangle(hdc, 0, 0 - brick_half_height - offset, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + brick_half_height - offset);

    SelectObject(hdc, front_pen);
    SelectObject(hdc, front_brush);

    Rectangle(hdc, 0, 0 - brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + brick_half_height);

    if (rotation_step >= 5 and rotation_step <= 12)
    {
        if (letter_type == ELT_O)
        {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, 0 - 5 * AsConfig::Global_Scale / 2, 0 + 10 * AsConfig::Global_Scale, 0 + 5 * AsConfig::Global_Scale / 2);
        }
    }

}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type) const
{
    RECT brick{};

    HBRUSH brush = 0;
    HPEN pen = 0;

    brick.left = x * AsConfig::Global_Scale;
    brick.top = y * AsConfig::Global_Scale;
    brick.right = brick.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    brick.bottom = brick.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

    switch (brick_type)
    {
    case EBT_Red:
        pen = Brick_Red_Pen;
        brush = Brick_Red_Brush;
        break;

    case EBT_Blue:
        pen = Brick_Blue_Pen;
        brush = Brick_Blue_Brush;
        break;

    default:
        return;
    }

    if (pen and brush)
    {
        SelectObject(hdc, pen);
        SelectObject(hdc, brush);
    }

    RoundRect(hdc, brick.left, brick.top, brick.right, brick.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)
{
    int i, j;
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
        return;

    for (i = 0; i < Level_Height; i++)
        for (j = 0; j < Level_Width; j++)
            Draw_Brick(hdc, AsConfig::Level_X_Offset + j * Cell_Width, AsConfig::Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(Current_Level[i][j]) );

    Active_Brick.Draw(hdc);
}
//------------------------------------------------------------------------------------------------------------
