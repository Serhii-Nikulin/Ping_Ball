#include "Engine.h"
#define _USE_MATH_DEFINES
#include <math.h>

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
HPEN Highlight_Pen, Letter_Pen;
HWND Hwnd;
RECT Platform_Rect{}, Prev_Platform_Rect{};
RECT Level_Rect{};

enum EBrick_Type: unsigned char {EBT_None, EBT_Red, EBT_Blue};
enum ELetter_Type {ELT_None, ELT_O};

const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;

const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;

const int Level_Height = 14;
const int Level_Width = 12;

const int Platform_Height = 7;
const int Platform_Y_Pos = 185;
int Platform_Width = 28;
int Platform_Inner_Width = 21;
int Platform_X_Pos = 0;
int Platform_X_Step = Global_Scale * 2;
const int Min_X_Pos = 7;
const int Max_X_Pos = 200;

HPEN Platform_Side_Pen, Platform_Inner_Pen;
HBRUSH Platform_Side_Brush, Platform_Inner_Brush;
HPEN BG_Pen;
HBRUSH BG_Brush;

unsigned char level_01[Level_Height][Level_Width] = {
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
void Create_Pen_Brush(HPEN& pen, HBRUSH& brush, unsigned char r, unsigned char g, unsigned char b)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b) );
	brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void Redraw_Platform()
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = (Min_X_Pos + Platform_X_Pos) * Global_Scale;
    Platform_Rect.top = Platform_Y_Pos * Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void Init_Engine(HWND hwnd)
{
    Hwnd = hwnd;

    Redraw_Platform();

    Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush, 224, 173, 94);
    Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush, 120, 180, 200);

	Highlight_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(200, 100, 180) );
    Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(60, 60, 70) );

    Create_Pen_Brush(Platform_Side_Pen, Platform_Side_Brush, 100, 110, 160);
	Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 180, 185, 230);

    Create_Pen_Brush(BG_Pen, BG_Brush, 40, 44, 52);

    Level_Rect.left = Level_X_Offset * Global_Scale;
    Level_Rect.top = Level_Y_Offset * Global_Scale;
    Level_Rect.right = Level_Rect.left + Level_Width * Cell_Width * Global_Scale;
    Level_Rect.bottom = Level_Rect.top + Level_Height * Cell_Height * Global_Scale;

    /*Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush, 148, 99, 85);
    Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush, 173, 126, 107);

    Highlight_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(130, 180, 140) );
    Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(245, 235, 220) );

    Create_Pen_Brush(Platform_Side_Pen, Platform_Side_Brush, 180, 145, 95);
    Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 100, 115, 130);*/
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
    RECT brick{};

    HBRUSH brush = 0;
    HPEN pen = 0;

    brick.left = x * Global_Scale;
    brick.top = y * Global_Scale;
    brick.right = brick.left + Brick_Width * Global_Scale;
    brick.bottom = brick.top + Brick_Height * Global_Scale;

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

    RoundRect(hdc, brick.left, brick.top, brick.right, brick.bottom, 2 * Global_Scale, 2 * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Circle(HDC hdc, int x, int y)
{
    Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Platform_Height) * Global_Scale, (y + Platform_Height) * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x)
{ 
    SelectObject(hdc, BG_Pen);
    SelectObject(hdc, BG_Brush);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    SelectObject(hdc, Platform_Side_Pen);
    SelectObject(hdc, Platform_Side_Brush);

    Draw_Circle(hdc, x, Platform_Y_Pos);
	Draw_Circle(hdc, x + Platform_Inner_Width, Platform_Y_Pos);

    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);
    RoundRect(hdc, (x + 4) * Global_Scale, (Platform_Y_Pos + 1) * Global_Scale, (x + 4 + Platform_Inner_Width - 1) * Global_Scale, (Platform_Y_Pos + 1 + 5) * Global_Scale, Global_Scale * 3, Global_Scale * 3);

    SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * Global_Scale, (Platform_Y_Pos + 1) * Global_Scale, (x + Platform_Height - 1) * Global_Scale, (Platform_Y_Pos + Platform_Height - 1) * Global_Scale, 
        (x + Platform_Height / 2) * Global_Scale, Platform_Y_Pos * Global_Scale, 
        x * Global_Scale, (Platform_Y_Pos + Platform_Height / 2 + 1) * Global_Scale);

}
//------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
{
    int i, j;

    for (i = 0; i < Level_Height; i++)
        for (j = 0; j < Level_Width; j++)
            Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(level_01[i][j]) );
}
//------------------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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
void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    XFORM old_xform, new_xform;
    int brick_half_height;
    double rotation_angle;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;

	brick_half_height = Brick_Height * Global_Scale / 2.0;

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

        Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);


        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
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

    int offset = (int)round(sin(rotation_angle) * Global_Scale * 3.0);
    SetWorldTransform(hdc, &new_xform);


    SelectObject(hdc, back_pen);
    SelectObject(hdc, back_brush);

    Rectangle(hdc, 0, 0 - brick_half_height - offset, Brick_Width * Global_Scale, 0 + brick_half_height - offset);

    SelectObject(hdc, front_pen);
    SelectObject(hdc, front_brush);

    Rectangle(hdc, 0, 0 - brick_half_height, Brick_Width * Global_Scale, 0 + brick_half_height);

    if (rotation_step >= 5 and rotation_step <= 12)
    {
        if (letter_type == ELT_O)
        {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * Global_Scale, 0 - 5 * Global_Scale / 2, 0 + 10 * Global_Scale, 0 + 5 * Global_Scale / 2);
        }
    }

    SetWorldTransform(hdc, &old_xform);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc, RECT &paint_area)
{
    int i;
    RECT intersection_rect{};

    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
        Draw_Level(hdc);

    if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
        Draw_Platform(hdc, Min_X_Pos + Platform_X_Pos);

    for (i = 0; i < 16; i++)
    {
        Draw_Brick_Letter(hdc, 100 + i * Cell_Width * Global_Scale, 250, EBT_Blue, ELT_O, i);
        Draw_Brick_Letter(hdc, 100 + i * Cell_Width * Global_Scale, 300, EBT_Red, ELT_O, i);
    }
}
//------------------------------------------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform_X_Pos -= Platform_X_Step;

        if (Platform_X_Pos <= 0)
			Platform_X_Pos = 0;

        Redraw_Platform();
        break;

    case EKT_Right:
        Platform_X_Pos += Platform_X_Step;

		if (Platform_X_Pos >= Max_X_Pos - Platform_Width)
			Platform_X_Pos = Max_X_Pos - Platform_Width;

        Redraw_Platform();
        break;

    case EKT_Space:
        break;
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------