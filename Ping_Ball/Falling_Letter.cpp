#include "Falling_Letter.h"


// AFalling_Letter
//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
    : Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Letter_Cell{}, Prev_Letter_Cell{}, Rotation_Step(0), Got_Hit(false), Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step)
{
    Letter_Cell.left = X;
    Letter_Cell.top = Y;
    Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

    Prev_Letter_Cell = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT &paint_area)
{
    RECT intersection_rect{};

    if (IntersectRect(&intersection_rect, &Prev_Letter_Cell, &paint_area) )
    {
        SelectObject(hdc, AsConfig::BG_Pen);
        SelectObject(hdc, AsConfig::BG_Brush);
        Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
    }

    if (IntersectRect(&intersection_rect, &Letter_Cell, &paint_area) )
        Draw_Brick_Letter(hdc);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
    int y_offset = AsConfig::Global_Scale;

    Prev_Letter_Cell = Letter_Cell;

    Y += y_offset;
    Letter_Cell.top += y_offset;
    Letter_Cell.bottom += y_offset;

    if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
    {
        Rotation_Step += 1;
        Next_Rotation_Tick += Ticks_Per_Step;
    }

    InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
    if (Got_Hit or Letter_Cell.top > (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{
    XFORM old_xform{}, new_xform{};
    int brick_half_height;
    double rotation_angle;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    int offset;

    if (! (Brick_Type == EBT_Blue || Brick_Type == EBT_Red))
        return;

    brick_half_height = (int)(AsConfig::Brick_Height * AsConfig::Global_Scale / 2.0);
    Rotation_Step %= 16;

    if (Rotation_Step >= 5 and Rotation_Step <= 12)
    {
        if (Brick_Type == EBT_Blue)
            switch_color = false;
        else
            switch_color = true;
    }
    else
    {
        if (Brick_Type == EBT_Red)
            switch_color = true;
        else
            switch_color = false;
    }

    Set_Brick_Letter_Color(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (Rotation_Step == 4 or Rotation_Step == 12)
    {
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, X, Y + brick_half_height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height);


        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, X, Y + brick_half_height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height + AsConfig::Global_Scale - 1);
    }
    else
    {

        rotation_angle = Rotation_Step * 2.0 * M_PI / 16.0;
        new_xform.eM11 = (FLOAT)1;
        new_xform.eM12 = (FLOAT)0;

        new_xform.eM21 = (FLOAT)0;
        new_xform.eM22 = (FLOAT)cos(rotation_angle);

        new_xform.eDx = (FLOAT)X;
        new_xform.eDy = (FLOAT)(Y + brick_half_height);

        offset = sin(rotation_angle) * AsConfig::Global_Scale * 3.0 / 2.0;

        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &new_xform);

        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, 0, 0 - brick_half_height - offset, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + brick_half_height - offset);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, 0 - brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + brick_half_height);

        if (Rotation_Step >= 5 and Rotation_Step <= 12)
        {
            if (Letter_Type == ELT_O)
            {
                SelectObject(hdc, AsConfig::Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, 0 - 5 * AsConfig::Global_Scale / 2, 0 + 10 * AsConfig::Global_Scale, 0 + 5 * AsConfig::Global_Scale / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush) const
{
    if (is_switch_color == true)
    {
        front_pen = AsConfig::Brick_Blue_Pen;
        front_brush = AsConfig::Brick_Blue_Brush;

        back_pen = AsConfig::Brick_Red_Pen;
        back_brush = AsConfig::Brick_Red_Brush;
    }
    else
    {
        front_pen = AsConfig::Brick_Red_Pen;
        front_brush = AsConfig::Brick_Red_Brush;

        back_pen = AsConfig::Brick_Blue_Pen;
        back_brush = AsConfig::Brick_Blue_Brush;
    }

}
//------------------------------------------------------------------------------------------------------------