#include "Level.h"

//------------------------------------------------------------------------------------------------------------
unsigned char ALevel::Level_01[ALevel::Level_Height][ALevel::Level_Width] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------




//ALevel
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
    : Active_Brick(EBT_Blue), Level_Rect{}, Letter_Pen(0), Brick_Red_Pen(0), Brick_Blue_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0) 
{
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
void ALevel::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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

    SetWorldTransform(hdc, &old_xform);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
void ALevel::Draw(HDC hdc, RECT &paint_area, HWND hwnd)
{
    int i, j;
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
        return;

    for (i = 0; i < Level_Height; i++)
        for (j = 0; j < Level_Width; j++)
            Draw_Brick(hdc, AsConfig::Level_X_Offset + j * Cell_Width, AsConfig::Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(Level_01[i][j]) );

    Active_Brick.Draw(hdc);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int next_x_pos, int &next_y_pos, double &ball_direction)
{
    int i, j;
    int brick_x_pos, brick_y_pos;

    for (i = Level_Height - 1; i >= 0; i--)
        for (j = 0; j < Level_Width; j++)
        {
            if(Level_01[i][j] == 0)
                continue;

            brick_x_pos = AsConfig::Level_X_Offset + j * Cell_Width;
            brick_y_pos = AsConfig::Level_Y_Offset + i * Cell_Height + AsConfig::Brick_Height;

            if (next_x_pos >= brick_x_pos and next_x_pos <= brick_x_pos + AsConfig::Brick_Width)
                if (next_y_pos <= brick_y_pos and next_y_pos >= brick_y_pos - AsConfig::Brick_Height)
                {
                    ball_direction = -ball_direction;
                    next_y_pos = brick_y_pos + (brick_y_pos - next_y_pos);

                    return;
                }
        }
}
//------------------------------------------------------------------------------------------------------------


