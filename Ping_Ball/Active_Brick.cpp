#include "Active_Brick.h"

//AActive_Brick
//------------------------------------------------------------------------------------------------------------
HPEN AActive_Brick::Fading_Red_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fading_Red_Brick_Brushes[Max_Fade_Step];
HPEN AActive_Brick::Fading_Blue_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fading_Blue_Brick_Brushes[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y)
    : Brick_Type(brick_type), Brick_Rect{}, Pen(0), Brush(0), Fade_Step(0)
{
    Brick_Rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
    Brick_Rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
    Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc)
{
    HPEN pen = 0;
    HBRUSH brush = 0;

    switch(Brick_Type)
    {
    case EBT_Red:
		pen = Fading_Red_Brick_Pens[Fade_Step];
        brush = Fading_Red_Brick_Brushes[Fade_Step];
        break;

    case EBT_Blue:
        pen = Fading_Blue_Brick_Pens[Fade_Step];
        brush = Fading_Blue_Brick_Brushes[Fade_Step];
        break;

    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act()
{
	if (Fade_Step < Max_Fade_Step - 1)
	{
		Fade_Step += 1;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick::Is_Finished()
{
    if (Fade_Step >= Max_Fade_Step - 1)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick::Get_Fading_Value(int step, int max_value, int min_value)
{
	return static_cast<unsigned char>(max_value - step * (max_value - min_value) / (Max_Fade_Step - 1) );
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush)
{
    unsigned char r, g, b;

    r = Get_Fading_Value(step, color.R, AsConfig::BG_Color.R);
    g = Get_Fading_Value(step, color.G, AsConfig::BG_Color.G);
    b = Get_Fading_Value(step, color.B, AsConfig::BG_Color.B);

    AsConfig::Create_Pen_Brush(pen, brush, r, g, b);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Setup_Colors()
{
    int i;

	for (i = 0; i < Max_Fade_Step; i++)
	{
        Get_Fading_Color(AsConfig::Red_Brick_Color, i, Fading_Red_Brick_Pens[i], Fading_Red_Brick_Brushes[i]);
        Get_Fading_Color(AsConfig::Blue_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
	}
}
//------------------------------------------------------------------------------------------------------------