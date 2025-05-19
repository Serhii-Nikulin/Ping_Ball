#include "Config.h"

#include "Border.h"

//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
    : Border_Blue_Pen(0), Border_Red_Pen(0), Border_Blue_Brush(0), Border_Red_Brush(0)
{}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Init()
{
    //Border
    AsConfig::Create_Pen_Brush(Border_Red_Pen, Border_Red_Brush, 220, 100, 80);
    AsConfig::Create_Pen_Brush(Border_Blue_Pen, Border_Blue_Brush, 80, 140, 210);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN &bg_pen, HBRUSH &bg_brush)
{
    //contour
    SelectObject(hdc, Border_Red_Pen);
    SelectObject(hdc, Border_Red_Brush);

    if (top_border)
        Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale);
    else
        Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

    //innner part
    SelectObject(hdc, Border_Blue_Pen);
    SelectObject(hdc, Border_Blue_Brush);

    if (top_border)
        Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);
    else
        Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

    //insertion
    SelectObject(hdc, bg_pen);
    SelectObject(hdc, bg_brush);

    if (top_border)
        Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 3) * AsConfig::Global_Scale);
    else
        Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, HPEN &bg_pen, HBRUSH &bg_brush)
{
    int i; 

    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 3 + 4 * i, 0, true, bg_pen, bg_brush);
        Draw_Element(hdc, 2, 1 + 4 * i, false, bg_pen, bg_brush);
        Draw_Element(hdc, 202, 1 + 4 * i, false, bg_pen, bg_brush);
    }
}
//------------------------------------------------------------------------------------------------------------