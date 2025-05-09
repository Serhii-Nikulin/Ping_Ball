#include "Engine.h"

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

enum EBrick_Type: unsigned char {EBT_None, EBT_Red, EBT_Blue};

const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;

const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;

const int Level_Height = 14;
const int Level_Width = 12;

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
void Init()
{
    Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(224, 173, 94) );
    Brick_Red_Brush = CreateSolidBrush(RGB(224, 173, 94) );

    Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(120, 180, 200) );
    Brick_Blue_Brush = CreateSolidBrush(RGB(120, 180, 200) );
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
void Draw_Level(HDC hdc)
{
    int i, j;

    for (i = 0; i < Level_Height; i++)
        for (j = 0; j < Level_Width; j++)
            Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(level_01[i][j]) );
}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{
    Draw_Level(hdc);
}
//------------------------------------------------------------------------------------------------------------