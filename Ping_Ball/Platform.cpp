#include "Platform.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
    : Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Side_Pen(0), Platform_Inner_Pen(0), Platform_Side_Brush(0), Platform_Inner_Brush(0), Width(28), Inner_Width(21), X_Pos(50), X_Step(AsConfig::Global_Scale * 2)
{
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
    Highlight_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255) );

    //Platform
    AsConfig::Create_Pen_Brush(Platform_Side_Pen, Platform_Side_Brush, 120, 120, 120);
    AsConfig::Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 60, 60, 60);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw(HWND hwnd)
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = (X_Pos) * AsConfig::Global_Scale;
    Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + AsConfig::Platform_Height * AsConfig::Global_Scale;

    InvalidateRect(hwnd, &Platform_Rect, FALSE);
    InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle(HDC hdc, int x, int y)
{
    Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Platform_Height) * AsConfig::Global_Scale, (y + AsConfig::Platform_Height) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area, HPEN &bg_pen, HBRUSH &bg_brush)
{ 
    RECT intersection_rect;

    int x = X_Pos;
    int y = AsConfig::Platform_Y_Pos;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
        return;

    SelectObject(hdc, bg_pen);
    SelectObject(hdc, bg_brush);
    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    SelectObject(hdc, Platform_Side_Pen);
    SelectObject(hdc, Platform_Side_Brush);

    Draw_Circle(hdc, x, y);
    Draw_Circle(hdc, x + Inner_Width, y);

    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);
    RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale, AsConfig::Global_Scale * 3, AsConfig::Global_Scale * 3);

    SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + AsConfig::Platform_Height- 1) * AsConfig::Global_Scale, (y + AsConfig::Platform_Height - 1) * AsConfig::Global_Scale, 
        (x + AsConfig::Platform_Height / 2) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, 
        x * AsConfig::Global_Scale, (y + AsConfig::Platform_Height / 2 + 1) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------