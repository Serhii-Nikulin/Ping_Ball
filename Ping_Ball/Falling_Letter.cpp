#include "Falling_Letter.h"


// AFalling_Letter
//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
    : Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Letter_Cell{}, Prev_Letter_Cell{}, Rotation_Step(0), Got_Hit(false), Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step), Falling_Letter_State(EFLS_Normal)
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

	if (Falling_Letter_State == EFLS_Finalizing)
	{
		Falling_Letter_State = EFLS_Finished;
		return;
	}

    if (IntersectRect(&intersection_rect, &Letter_Cell, &paint_area) )
        Draw_Brick_Letter(hdc);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
    int y_offset;

    if (Falling_Letter_State != EFLS_Normal)
        return;

    if (Letter_Cell.top >= (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale)
    {
        Finalize();
        return;
    }

    y_offset = AsConfig::Global_Scale;

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
    if (Falling_Letter_State == EFLS_Finished)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Get_Letter_Cell(RECT &letter_rect)
{
    letter_rect = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Finalize()
{
    if (Falling_Letter_State == EFLS_Normal)
        Falling_Letter_State = EFLS_Finalizing;

    InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Test_Draw_All_Steps(HDC hdc)
{
    int i;
    int x_offset = AsConfig::Cell_Width * AsConfig::Global_Scale;
   
    for (i = 0; i < Max_Rotation_Step; i++)
    {
        Draw_Brick_Letter(hdc);
        Rotation_Step += 1;
        X += x_offset;
        Letter_Cell.left += x_offset;
        Letter_Cell.right += x_offset;
    }
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{
    XFORM old_xform{}, new_xform{};
    double rotation_angle;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    int offset;

    if (! (Brick_Type == EBT_Blue || Brick_Type == EBT_Red))
        return;

    Rotation_Step %= Max_Rotation_Step;

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
            switch_color = false;
        else
            switch_color = true;
    }

    Set_Brick_Letter_Color(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (Rotation_Step == 4 or Rotation_Step == 12)
    {
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, X, Y + Brick_Half_Height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height);


        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, X, Y + Brick_Half_Height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height + AsConfig::Global_Scale - 1);
    }
    else
    {

        rotation_angle = Rotation_Step * 2.0 * M_PI / Max_Rotation_Step;
        new_xform.eM11 = (FLOAT)1;
        new_xform.eM12 = (FLOAT)0;

        new_xform.eM21 = (FLOAT)0;
        new_xform.eM22 = -(FLOAT)cos(rotation_angle);

        new_xform.eDx = (FLOAT)X;
        new_xform.eDy = (FLOAT)(Y + Brick_Half_Height);

        offset = sin(rotation_angle) * AsConfig::Global_Scale * 3.0 / 2.0;

        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &new_xform);

        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, 0, 0 - Brick_Half_Height + offset, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + Brick_Half_Height + offset);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, 0 - Brick_Half_Height, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + Brick_Half_Height);

        if (Rotation_Step >= 5 and Rotation_Step <= 12)
        {
            switch (Letter_Type)
            {
                case ELT_S:
                    SelectObject(hdc, AsConfig::Letter_Pen);

                    //верхн€€ горизонтальна€
                    Draw_Line(hdc, 5, 1, 10, 1);
                 
                    // Ћева€ верхн€€ вертикаль
                    Draw_Line(hdc, 5, 1, 5, 3);
                  
                    // —редн€€ горизонтальна€
                    Draw_Line(hdc, 5, 3, 10, 3);
                 
                    // ѕрава€ нижн€€ вертикаль
                    Draw_Line(hdc, 10, 3, 10, 5);
              
                    // Ќижн€€ горизонтальна€ лини€
                    Draw_Line(hdc, 10, 5, 5, 5);
                    break;

                case ELT_L:
                    SelectObject(hdc, AsConfig::Letter_Pen);

                    //верхн€€ горизонтальна€
                    Draw_Line(hdc, 6, 1, 6, 5);

                    // Ћева€ верхн€€ вертикаль
                    Draw_Line(hdc, 6, 5, 9, 5);

                case ELT_HP:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 4, 1, 4, 5);
                    Draw_Line(hdc, 7, 1, 7, 5);
                    Draw_Line(hdc, 4, 3, 7, 3);

                    Draw_Line(hdc, 9, 1, 9, 5);
                    Draw_Line(hdc, 9, 1, 11, 1);
                    Draw_Line(hdc, 11, 1, 11, 3);
                    Draw_Line(hdc, 11, 3, 9, 3);
                    break;

                case ELT_T:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 4, 1, 10, 1);
                    Draw_Line(hdc, 7, 1, 7, 5);
                    break;
                   
                case ELT_X:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 5, 1, 10, 5);
                    Draw_Line(hdc, 10, 1, 5, 5);
                    break;

                case ELT_C:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 6, 1, 9, 1);
                    Draw_Line(hdc, 9, 1, 9, 2);
                    Draw_Line(hdc, 6, 1, 6, 5);
                    Draw_Line(hdc, 6, 5, 9, 5);
                    Draw_Line(hdc, 9, 5, 9, 4);
                    break;

                case ELT_W:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 4, 1, 5, 5);
                    Draw_Line(hdc, 5, 5, 7, 2);
                    Draw_Line(hdc, 7, 2, 9, 5);
                    Draw_Line(hdc, 9, 5, 10, 1);
                    break;

                case ELT_M:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 5, 5, 5, 1);
                    Draw_Line(hdc, 5, 1, 7, 4);
                    Draw_Line(hdc, 7, 4, 9, 1);
                    Draw_Line(hdc, 9, 1, 9, 5);
                    break;

                case ELT_F:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 6, 1, 6, 5);
                    Draw_Line(hdc, 6, 1, 9, 1);
                    Draw_Line(hdc, 6, 3, 9, 3);
                    break;

                case ELT_R:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 6, 1, 9, 1);
                    Draw_Line(hdc, 9, 1, 9, 3);
                    Draw_Line(hdc, 9, 3, 6, 3);
                    Draw_Line(hdc, 6, 3, 9, 5);
                    Draw_Line(hdc, 6, 1, 6, 5);
                    break;

                case ELT_Plus:
                    SelectObject(hdc, AsConfig::Letter_Pen);
                    Draw_Line(hdc, 7, 1, 7, 5);
                    Draw_Line(hdc, 4, 3, 10, 3);
                    break;
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
void AFalling_Letter::Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2)
{
    MoveToEx(hdc, x_1 * AsConfig::Global_Scale, y_1 * AsConfig::Global_Scale - Brick_Half_Height + 1, NULL);
    LineTo(hdc, x_2 * AsConfig::Global_Scale, y_2 * AsConfig::Global_Scale - Brick_Half_Height + 1);
}
//------------------------------------------------------------------------------------------------------------