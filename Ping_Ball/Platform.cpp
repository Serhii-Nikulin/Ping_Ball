#include "Platform.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: Platform_State(EPS_Normal), Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Side_Pen(0), Platform_Inner_Pen(0), Platform_Side_Brush(0), Platform_Inner_Brush(0), Width(28), Inner_Width(21), X_Pos(0), X_Step(AsConfig::Global_Scale * 2), Meltdown_Y_Pos{}, Rotation_Step(0)
{
    X_Pos = (AsConfig::Max_X_Pos + 1 + AsConfig::Border_X_Offset + 1 - Normal_Width) / 2;
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
	switch (Platform_State)
	{
    case EPS_Meltdown:
    case EPS_Rolling:
        Redraw(hwnd);
    }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
    int i;
    int len = sizeof(Meltdown_Y_Pos) / sizeof(Meltdown_Y_Pos[i]);

	if (new_state == Platform_State)
        return;

	if (new_state == EPS_Normal)
	{
		for (i = 0; i < len; i++)
			Meltdown_Y_Pos[i] = Platform_Rect.bottom - 1;
	}

    Platform_State = new_state;
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

    case EPS_Rolling:
        Draw_Rolling(hdc);
        break;

    default:
        return;
    }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle(HDC hdc, int x, int y)
{
    Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Platform_Height) * AsConfig::Global_Scale, (y + AsConfig::Platform_Height) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)
{
	SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Circle_Size - 1) * AsConfig::Global_Scale, (y + Circle_Size - 1) * AsConfig::Global_Scale, 
        (x + Circle_Size / 2) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, 
        x * AsConfig::Global_Scale, (y + Circle_Size / 2 + 1) * AsConfig::Global_Scale);
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
    Draw_Circle_Highlight(hdc, x, y);
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
void AsPlatform::Draw_Rolling(HDC hdc)
{
    XFORM old_xform{}, new_xform{};
    double rotation_angle;

    //Circle
    int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;
    const int &scale = AsConfig::Global_Scale;
	int radius = Circle_Size * scale / 2;

    SelectObject(hdc, Platform_Side_Pen);
    SelectObject(hdc, Platform_Side_Brush);
    Draw_Circle(hdc, x, y);

    //Rectangle
    SetGraphicsMode(hdc, GM_ADVANCED);
    GetWorldTransform(hdc, &old_xform);

    Rotation_Step %= Max_Rotation_Step;
    rotation_angle = 2 * M_PI * Rotation_Step / Max_Rotation_Step;
    Rotation_Step += 1;

    new_xform.eM11 = (FLOAT)cos(rotation_angle);
    new_xform.eM12 = (FLOAT)-sin(rotation_angle);

    new_xform.eM21 = (FLOAT)sin(rotation_angle);
    new_xform.eM22 = (FLOAT)cos(rotation_angle);

    new_xform.eDx = (FLOAT)(x * scale + radius);
    new_xform.eDy = (FLOAT)(y * scale + radius);

    SetWorldTransform(hdc, &new_xform);

    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);
    Rectangle(hdc, 0 - 1 * scale / 2, 0 - radius, 0 + 1 * scale / 2, 0 + radius);

    SetWorldTransform(hdc, &old_xform);

    //Highlight
    SelectObject(hdc, Highlight_Pen);
    Draw_Circle_Highlight(hdc, x, y);
}
//------------------------------------------------------------------------------------------------------------