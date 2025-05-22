#include "Platform.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: Platform_State(EPS_Normal), Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Side_Pen(0), Platform_Inner_Pen(0), Platform_Side_Brush(0), Platform_Inner_Brush(0), Width(28), Inner_Width(21), X_Pos(50), X_Step(AsConfig::Global_Scale * 2), Meltdown_Y_Pos{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
    Highlight_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255) );

    AsConfig::Create_Pen_Brush(Platform_Side_Pen, Platform_Side_Brush, 120, 120, 120);
    AsConfig::Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 60, 60, 60);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act(HWND hwnd)
{
    int i;
    int len = sizeof(Meltdown_Y_Pos) / sizeof(Meltdown_Y_Pos[i]);

    if (Platform_State == EPS_Normal)
    {
        Platform_State = EPS_Meltdown;

        for (i = 0; i < len; i++)
            Meltdown_Y_Pos[i] = Platform_Rect.bottom - 1;
    }

	if (Platform_State == EPS_Meltdown)
		Redraw(hwnd);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw(HWND hwnd)
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
    Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + AsConfig::Platform_Height * AsConfig::Global_Scale;

    if (Platform_State == EPS_Meltdown)
        Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;
 
	InvalidateRect(hwnd, &Platform_Rect, FALSE);
	InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
        return;

	switch (Platform_State)
	{
	case EPS_Normal:
		Draw_Normal(hdc);
		break;

    case EPS_Meltdown:
		Draw_Meltdown(hdc);
		break;

    default:
        return;
    }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown(HDC hdc)
{
    int i, j;
    int x, y;
    int y_offset;
    COLORREF pixel;
    COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

	int platform_width = Normal_Width * AsConfig::Global_Scale;
	int platform_height = AsConfig::Platform_Height * AsConfig::Global_Scale;

    for (i = 0; i < platform_width; i++)
    {
		x = Platform_Rect.left + i;
        y_offset = AsConfig::Rand(1, Meltdown_Speed);

        for (j = 0; j < platform_height; j++)
        {
            y = Meltdown_Y_Pos[i] - j;

            pixel = GetPixel(hdc, x, y);
            SetPixel(hdc, x, y + y_offset, pixel);
        }

		for (j = 0; j < y_offset; j++)
			SetPixel(hdc, x, y + j, bg_pixel);

        Meltdown_Y_Pos[i] += y_offset;
    }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle(HDC hdc, int x, int y)
{
    Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Platform_Height) * AsConfig::Global_Scale, (y + AsConfig::Platform_Height) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal(HDC hdc)
{ 
    int x = X_Pos;
    int y = AsConfig::Platform_Y_Pos;

    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);
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