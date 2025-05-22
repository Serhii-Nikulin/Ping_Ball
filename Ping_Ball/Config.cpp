#include "Config.h"

//AColor
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b)
{
}
//------------------------------------------------------------------------------------------------------------




//AsConfig
//------------------------------------------------------------------------------------------------------------
int AsConfig::Current_Timer_Tick = 0;
const AColor AsConfig::BG_Color(15, 15, 35);
const AColor AsConfig::Red_Brick_Color(220, 100, 80);
const AColor AsConfig::Blue_Brick_Color(80, 140, 210);

HPEN AsConfig::BG_Pen = 0;
HBRUSH AsConfig::BG_Brush = 0;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
    Create_Pen_Brush(BG_Pen, BG_Brush, BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(HPEN& pen, HBRUSH& brush, unsigned char r, unsigned char g, unsigned char b)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b) );
    brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(HPEN& pen, HBRUSH& brush, const AColor &color)
{
    pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B) );
    brush = CreateSolidBrush(RGB(color.R, color.G, color.B) );
}
//------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int min, int max)
{
    return rand() * max / RAND_MAX + min;
}
//------------------------------------------------------------------------------------------------------------