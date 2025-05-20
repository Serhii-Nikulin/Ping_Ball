#include "Active_Brick.h"

//AActive_Brick
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick()
    : Brick_Rect{}, Pen(0), Brush(0), Fade_Step(0)
{
    Brick_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
    Brick_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
    Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc)
{
    Pen = CreatePen(PS_SOLID, 0, RGB(80 - Fade_Step * 80 / Max_Fade_Step, 140 -  Fade_Step * 140 / Max_Fade_Step, 210 - Fade_Step * 210 / Max_Fade_Step) );
    Brush = CreateSolidBrush(RGB(80 - Fade_Step * 80 / Max_Fade_Step, 140 -  Fade_Step * 140 / Max_Fade_Step, 210 - Fade_Step * 210 / Max_Fade_Step) );

    SelectObject(hdc, Pen);
    SelectObject(hdc, Brush);
    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
    if (Fade_Step >= Max_Fade_Step)
        Fade_Step = 0;

    InvalidateRect(hwnd, &Brick_Rect, FALSE);

    Fade_Step += 1;
}
//------------------------------------------------------------------------------------------------------------