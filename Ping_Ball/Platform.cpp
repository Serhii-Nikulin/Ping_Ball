#include "Platform.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::~AsPlatform()
{
    if (Normal_Platform_Image)
    {
        delete[] Normal_Platform_Image;
        Normal_Platform_Image = 0;
    }
}
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: Platform_State(EPS_Normal), Platform_Rect{}, Prev_Platform_Rect{}, Width(28), Inner_Width(21), X_Pos(0), X_Step(AsConfig::Global_Scale * 2), Meltdown_Y_Pos{}, Rotation_Step(13), Normal_Platform_Image(0), Normal_Platform_Image_Width(Width * AsConfig::Global_Scale), Normal_Platform_Image_Height(Height * AsConfig::Global_Scale), Highlight_Color(255, 255, 255), Platform_Side_Color(120, 120, 120), Platform_Inner_Color(60, 60, 60)
{
    X_Pos = (AsConfig::Max_X_Pos + 1 + AsConfig::Border_X_Offset + 1 - Normal_Width) / 2;
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
    if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos)
        return false;

    if (Reflect_From_Circle(next_x_pos, next_y_pos, ball, X_Pos + Circle_Size / 2.0) )
        return true;

    if (Reflect_From_Circle(next_x_pos, next_y_pos, ball, X_Pos + Width - Circle_Size / 2.0) )
        return true;

    if (Reflect_From_Center(next_x_pos, next_y_pos, ball) )
        return true;

    return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Move(bool to_left)
{
    if (Get_State() != EPS_Normal)
        return;

	if (to_left)
	{
		X_Pos -= X_Step;

		if (X_Pos <= AsConfig::Border_X_Offset)
			X_Pos = AsConfig::Border_X_Offset;
	}
	else
	{
		X_Pos += X_Step;

		if (X_Pos >= AsConfig::Max_X_Pos + 1 - Width)
			X_Pos = AsConfig::Max_X_Pos + 1 - Width;
	}

    Redraw();
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_From_Circle(double next_x_pos, double next_y_pos, ABall *ball, double circle_x_pos)
{
    double delta;
    double falling_angle, reflecting_angle;
    double distance;
    double radius = ball->Radius;
    double circle_radius = Circle_Size / 2.0;
    double circle_y_pos = AsConfig::Platform_Y_Pos + circle_radius;

    double dx = next_x_pos - circle_x_pos;
    double dy = circle_y_pos - next_y_pos;

    distance = sqrt(dx * dx + dy * dy);

    if (distance - (radius + circle_radius) > 1.0 / AsConfig::Global_Scale)
        return false;

    falling_angle = atan2(dy, dx);
    delta = ball->Get_Direction() - falling_angle;

    while (delta >= 2 * M_PI)
        delta -= 2 * M_PI;

    while (delta < 0)
        delta += 2 * M_PI;

    if (delta > M_PI_2 and delta < M_PI + M_PI_2)
    {
        reflecting_angle = falling_angle + M_PI - ball->Get_Direction();
        ball->Set_Direction(reflecting_angle + falling_angle);

        return true;
    }

    return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_From_Center(double next_x_pos, double next_y_pos, ABall *ball)
{
    double value_pos;
    int left_x_pos = X_Pos + Circle_Size - 1;
    int inner_y_pos;
    int top_y_pos = AsConfig::Platform_Y_Pos + 1;
    int low_y_pos = AsConfig::Platform_Y_Pos + Height - 1;;
    int right_x_pos = X_Pos + Width - (Circle_Size - 1);

    const double &radius = ball->Radius;

    if (ball->Is_Moving_Up() )
        inner_y_pos = low_y_pos;
    else
        inner_y_pos = top_y_pos;

    if (Hit_Circle_On_Line(next_y_pos - inner_y_pos, next_x_pos, left_x_pos, right_x_pos, radius, value_pos) )
    {
        ball->Reflect(true);
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act()
{
	switch (Platform_State)
	{
    case EPS_Meltdown:
    case EPS_Rolling:
    case EPS_Expanding_Rolling:
        Redraw();
        break;

    default:
        return;
    }
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State() const
{
	return Platform_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
    int i;
    int len = sizeof(Meltdown_Y_Pos) / sizeof(Meltdown_Y_Pos[i]);

	if (new_state == Platform_State)
        return;

	if (new_state == EPS_Meltdown)
	{
		for (i = 0; i < len; i++)
			Meltdown_Y_Pos[i] = Platform_Rect.top;
	}

    if (new_state == EPS_Rolling)
        X_Pos = AsConfig::Max_X_Pos + 2;

    if (new_state == EPS_Expanding_Rolling)
    {
        Width = 0;
        Redraw();
    }

    Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw()
{
    int width;

    Prev_Platform_Rect = Platform_Rect;

	if (Platform_State == EPS_Rolling)
		width = Circle_Size;
	else 
		width = Width;

    Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
    Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
    Platform_Rect.right = Platform_Rect.left + width * AsConfig::Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

    if (Platform_State == EPS_Meltdown)
        Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;
 
	InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
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
    case EPS_Is_Ready:
		Draw_Normal_State(hdc);
		break;

    case EPS_Meltdown:
		Draw_Meltdown_State(hdc);
		break;

    case EPS_Rolling:
        Draw_Rolling_State(hdc);
        break;

	case EPS_Expanding_Rolling:
		Draw_Expanding_Rolling_State(hdc);
		break;

    default:
        return;
    }
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Hit_By(AFalling_Letter *falling_letter)
{
    RECT letter_rect{};
    RECT intersection_rect{};

    falling_letter->Get_Letter_Cell(letter_rect);

    if (IntersectRect(&intersection_rect, &letter_rect, &Platform_Rect) )
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc) const
{
	AsConfig::BG_Color.Select(hdc);
    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle(HDC hdc, int x, int y)
{
    Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Height) * AsConfig::Global_Scale - 1, (y + Height) * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y) const
{
	Highlight_Color.Select_Pen(hdc);
    Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Circle_Size - 1) * AsConfig::Global_Scale - 1, (y + Circle_Size - 1) * AsConfig::Global_Scale - 1, 
        (x + Circle_Size / 2) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, 
        x * AsConfig::Global_Scale, (y + Circle_Size / 2 + 1) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc)
{ 
    int i, j;
    int offset;
    int x = X_Pos;
    int y = AsConfig::Platform_Y_Pos;

    Clear_BG(hdc);

    Platform_Side_Color.Select(hdc);

    Draw_Circle(hdc, x + Width - Circle_Size, y);
    Draw_Circle(hdc, x, y);

    Platform_Inner_Color.Select(hdc);
    RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Width - 4) * AsConfig::Global_Scale - 1, (y + Circle_Size - 1) * AsConfig::Global_Scale - 1, AsConfig::Global_Scale * 3, AsConfig::Global_Scale * 3);

    Highlight_Color.Select(hdc);
    Draw_Circle_Highlight(hdc, x, y);

    if (Normal_Platform_Image == 0)
    {
        if (Platform_State == EPS_Normal)
        {
            offset = 0;
            Normal_Platform_Image = new int[Normal_Platform_Image_Height * Normal_Platform_Image_Width];
            
            for (i = 0; i < Normal_Platform_Image_Height; i++)
                for (j = 0; j < Normal_Platform_Image_Width; j++)
                    Normal_Platform_Image[offset++] = GetPixel(hdc, x * AsConfig::Global_Scale + j, y * AsConfig::Global_Scale + i);
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc)
{
    int i, j;
    int x, y;
    int len;
    int y_offset;
    const AColor *color;

    int offset_colons_count = 0;

	int max_y_pos = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;// + platform_height;

    for (i = 0; i < Normal_Platform_Image_Width; i++)
    {
        if (Meltdown_Y_Pos[i] >= max_y_pos)
            continue;

        y_offset = AsConfig::Rand(Meltdown_Speed);

        x = Platform_Rect.left + i;
        y = Meltdown_Y_Pos[i];

        j = 0;
        len = 0;

        y += y_offset;

        while (Get_Platform_Image_Stroke_Color(i, j, len, &color) )
        {
            color->Select_Pen(hdc);
            MoveToEx(hdc, x, y, 0);

            y += len;

            LineTo(hdc, x, y);

            j += len;
            len = 0;
        }

        y = Meltdown_Y_Pos[i];

        AsConfig::BG_Color.Select_Pen(hdc);
        MoveToEx(hdc, x, y, 0);
        LineTo(hdc, x, y + y_offset);

        Meltdown_Y_Pos[i] += y_offset;
        offset_colons_count += 1;
    }

    if (offset_colons_count == 0)
        Set_State(EPS_Missing);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, int &len, const AColor **color)
{
    int i;

    int offset = y * Normal_Platform_Image_Width + x;
    int color_value = Normal_Platform_Image[offset];
    len = 1;

    if (color_value == AsConfig::BG_Color.Get_RGB() )
        *color = &AsConfig::BG_Color;
    else if (color_value == Platform_Side_Color.Get_RGB() )
        *color = &Platform_Side_Color;
    else if (color_value == Platform_Inner_Color.Get_RGB() )
        *color = &Platform_Inner_Color;
    else if (color_value == Highlight_Color.Get_RGB() )
        *color = &Highlight_Color;
    else
        *color = 0;

	if (*color == 0)
        return false;

    for (i = y; i <= Normal_Platform_Image_Height; i++)
    {
        offset += Normal_Platform_Image_Width;

        if (color_value == Normal_Platform_Image[offset])
            len += 1;
        else
            return true;
    }

    return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Rolling_State(HDC hdc)
{
    XFORM old_xform{}, new_xform{};
    double rotation_angle;

    int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;
    const int &scale = AsConfig::Global_Scale;
	int radius = Circle_Size * scale / 2;

    Clear_BG(hdc);

    //Circle
	Platform_Side_Color.Select(hdc);
    Draw_Circle(hdc, x, y);

    //BG Rectangle
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

	AsConfig::BG_Color.Select(hdc);
    Rectangle(hdc, 0 - 1 * scale / 2, 0 - radius, 0 + 1 * scale / 2, 0 + radius);

    SetWorldTransform(hdc, &old_xform);

    //Highlight
    Highlight_Color.Select_Pen(hdc);
    Draw_Circle_Highlight(hdc, x, y);

	//Check if the platform is finished rolling
    if (Finish_Rolling_X_Pos == X_Pos)
    {
        Set_State(EPS_Expanding_Rolling);
        return;
    }

    if (Finish_Rolling_X_Pos >= X_Pos)
    {
        X_Pos = Finish_Rolling_X_Pos;
        return;
    }

    //roll platform
    X_Pos -= Rolling_Speed;

}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Rolling_State(HDC hdc)
{
    Draw_Normal_State(hdc);


    Width += 3;
    X_Pos -= 1;

    if (Width >= Normal_Width)
    {
        Width = Normal_Width;
        Set_State(EPS_Is_Ready);
        Redraw();
    }

}
//------------------------------------------------------------------------------------------------------------