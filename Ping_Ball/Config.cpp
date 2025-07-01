#include "Config.h"

//AColor
//------------------------------------------------------------------------------------------------------------
AColor::AColor()
	:R(0), G(0), B(0), Pen(0), Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b)
{
	Pen = CreatePen(PS_SOLID, 0, RGB(R, G, B));
	Brush = CreateSolidBrush(RGB(R, G, B));
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &color, int pen_size)
	: R(color.R), G(color.G), B(color.B)
{
	Pen = CreatePen(PS_SOLID, pen_size, RGB(R, G, B));
	Brush = CreateSolidBrush(RGB(R, G, B));
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &pen_color, const AColor &brush_color, int pen_size)
	: R(0), G(0), B(0), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, RGB(pen_color.R, pen_color.G, pen_color.B));
	Brush = CreateSolidBrush(RGB(brush_color.R, brush_color.G, brush_color.B));
}
//------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
    return RGB(R, G, B);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select(HDC hdc) const
{
    SelectObject(hdc, Pen);
    SelectObject(hdc, Brush);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select_Pen(HDC hdc) const
{
	SelectObject(hdc, Pen);
}
//------------------------------------------------------------------------------------------------------------




//AsConfig
//------------------------------------------------------------------------------------------------------------
HWND AsConfig::Hwnd = 0;
int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Has_Floor = false;
double AsConfig::Moving_Size_Step = 1.0 / AsConfig::Global_Scale;

const AColor AsConfig::BG_Color(15, 15, 35);
const AColor AsConfig::Red_Color(220, 100, 80);
const AColor AsConfig::Blue_Color(80, 140, 210);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Red_Highlight_Unbreakable(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);
const AColor AsConfig::Blue_Highlight_Unbreakable(AsConfig::Blue_Color, 1 * AsConfig::Global_Scale);
const AColor AsConfig::Parachute_Color(AsConfig::Red_Color, AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Teleport_Color(AsConfig::Blue_Color, AsConfig::BG_Color, 1 * AsConfig::Global_Scale);
HPEN AsConfig::Letter_Pen(0);
//------------------------------------------------------------------------------------------------------------
void AsConfig::Round_Rect(HDC hdc, RECT &rect, int corner_size)
{
	int corner = corner_size * AsConfig::Global_Scale;
	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, corner, corner);
}
//------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)
{
    return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------