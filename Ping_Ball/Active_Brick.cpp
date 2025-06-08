#include "Active_Brick.h"

//AActive_Brick
AColor AActive_Brick::Fading_Red_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick::Fading_Blue_Brick_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y)
    : Brick_Type(brick_type), Brick_Rect{}, Fade_Step(0)
{
    Brick_Rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
    Brick_Rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
    Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
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
void AActive_Brick::Draw(HDC hdc, RECT &paint_area)
{
    switch(Brick_Type)
    {
    case EBT_Red:
        Fading_Red_Brick_Colors[Fade_Step].Select(hdc);
        break;

    case EBT_Blue:
        Fading_Blue_Brick_Colors[Fade_Step].Select(hdc);
        break;

    default:
        return;
    }

    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right - 1, Brick_Rect.bottom - 1, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
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
void AActive_Brick::Setup_Colors()
{
    int i;

    for (i = 0; i < Max_Fade_Step; i++)
    {
        Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Brick_Colors[i]);
        Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
    }
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick::Get_Fading_Value(int step, int max_value, int min_value)
{
	return static_cast<unsigned char>(max_value - step * (max_value - min_value) / (Max_Fade_Step - 1) );
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Get_Fading_Color(const AColor &origin_color, int step, AColor &fading_color)
{
    unsigned char r, g, b;

    r = Get_Fading_Value(step, origin_color.R, AsConfig::BG_Color.R);
    g = Get_Fading_Value(step, origin_color.G, AsConfig::BG_Color.G);
    b = Get_Fading_Value(step, origin_color.B, AsConfig::BG_Color.B);

    fading_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------
