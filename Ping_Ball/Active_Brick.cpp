#include "Active_Brick.h"

// AGraphics_Object
//------------------------------------------------------------------------------------------------------------
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick
//------------------------------------------------------------------------------------------------------------
AActive_Brick::~AActive_Brick()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y)
    : Brick_Type(brick_type), Brick_Rect{}
{
    Brick_Rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
    Brick_Rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
    Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Red_Blue
AColor AActive_Brick_Red_Blue::Fading_Red_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick_Red_Blue::Fading_Blue_Brick_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int brick_x, int brick_y)
	: AActive_Brick(brick_type, brick_x, brick_y), Fade_Step(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Act()
{
    if (Fade_Step < Max_Fade_Step - 1)
    {
        Fade_Step += 1;
        InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
    }
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw(HDC hdc, RECT &paint_area)
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

    AsConfig::Round_Rect(hdc, Brick_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Red_Blue::Is_Finished()
{
    if (Fade_Step >= Max_Fade_Step - 1)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Setup_Colors()
{
    int i;

    for (i = 0; i < Max_Fade_Step; i++)
    {
        Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Brick_Colors[i]);
        Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
    }
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Red_Blue::Get_Fading_Value(int step, int max_value, int min_value)
{
	return static_cast<unsigned char>(max_value - step * (max_value - min_value) / (Max_Fade_Step - 1) );
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor &origin_color, int step, AColor &fading_color)
{
    unsigned char r, g, b;

    r = Get_Fading_Value(step, origin_color.R, AsConfig::BG_Color.R);
    g = Get_Fading_Value(step, origin_color.G, AsConfig::BG_Color.G);
    b = Get_Fading_Value(step, origin_color.B, AsConfig::BG_Color.B);

    fading_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw_In_Level(HDC hdc, EBrick_Type brick_type, RECT &rect)
{
    const AColor *color = 0;

    switch (brick_type)
    {
    case EBT_Red:
		color = &AsConfig::Red_Color;
		break;

    case EBT_Blue:
	    color = &AsConfig::Blue_Color;
        break;

    case EBT_None:
		color = &AsConfig::BG_Color;
		break;
    }

    if (color)
    {
        color->Select(hdc);
        AsConfig::Round_Rect(hdc, rect);
    }
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Unbreakable
AColor AActive_Brick_Unbreakable::Red_Highlight(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);
AColor AActive_Brick_Unbreakable::Blue_Highlight(AsConfig::Blue_Color, 1 * AsConfig::Global_Scale);
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
    DeleteObject(Region);
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(EBrick_Type brick_type, int brick_x, int brick_y)
	: AActive_Brick(brick_type, brick_x, brick_y), Animation_Step(0), Region(0)
{
    Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale - 1, 2 * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
    if (Animation_Step <= Max_Animation_Step)
    {
        InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
        Animation_Step += 1;
    }
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
    int offset;

	AActive_Brick_Unbreakable::Draw_In_Level(hdc, Brick_Rect);
	SelectClipRgn(hdc, Region);
    offset = - 9 * AsConfig::Global_Scale + Animation_Step * AsConfig::Global_Scale * 2;

	Blue_Highlight.Select_Pen(hdc);
    MoveToEx(hdc, Brick_Rect.left - 1 * AsConfig::Global_Scale + offset, Brick_Rect.top + 8 * AsConfig::Global_Scale, 0);
    LineTo(hdc, Brick_Rect.left + 9 * AsConfig::Global_Scale + offset, Brick_Rect.top - 2 * AsConfig::Global_Scale);

	Red_Highlight.Select_Pen(hdc);
    MoveToEx(hdc, Brick_Rect.left + 1 * AsConfig::Global_Scale + offset, Brick_Rect.top + 8 * AsConfig::Global_Scale, 0);
    LineTo(hdc, Brick_Rect.left + 10 * AsConfig::Global_Scale + offset, Brick_Rect.top - 1 * AsConfig::Global_Scale);

    SelectClipRgn(hdc, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
    if (Animation_Step > Max_Animation_Step)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT &rect)
{
    AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------
