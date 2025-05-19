#include "Engine.h"

//ABall
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
    :Ball_Pen(0), Ball_Brush(0), Ball_Rect{}, Prev_Ball_Rect{}, Ball_X_Pos(60), Ball_Y_Pos(190), Ball_Speed(6.0), Ball_Direction(M_PI_4)
{}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
    AsConfig::Create_Pen_Brush(Ball_Pen, Ball_Brush, 255, 255, 255);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw(AsEngine *engine)
{
    Prev_Ball_Rect = Ball_Rect;

    Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
    Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
    Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale - 1;
    Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale - 1;

    InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
    RECT intersection_rect{};

    if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
        return;

    SelectObject(hdc, engine->BG_Pen);
    SelectObject(hdc, engine->BG_Brush);
    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

    SelectObject(hdc, Ball_Pen);
    SelectObject(hdc, Ball_Brush);
    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
    int next_x_pos, next_y_pos;

    next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
    next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

    int min_x_pos = AsBorder::Border_X_Offset;
    int max_x_pos = AsBorder::Max_X_Pos - Ball_Size + 1;
    int min_y_pos = AsBorder::Border_Y_Offset;
    int max_y_pos = AsBorder::Max_Y_Pos - Ball_Size + 1;

	int platform_y_pos = AsPlatform::Y_Pos - Ball_Size + 1;

    if (next_x_pos > max_x_pos)//right limit
    {
        Ball_Direction = M_PI - Ball_Direction;
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
    }

    if (next_y_pos < min_y_pos)//top limit
    {
        Ball_Direction = -Ball_Direction;
        next_y_pos = min_y_pos + (min_y_pos - next_y_pos);
    }

    if (next_x_pos < min_x_pos)//left limit
    {
        Ball_Direction = M_PI - Ball_Direction;
        next_x_pos = min_x_pos + (min_x_pos - next_x_pos);
    }

    if (next_y_pos > max_y_pos)//bottom limit
    {
        Ball_Direction = -Ball_Direction;
        next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
    }

	if (next_y_pos >= platform_y_pos and next_y_pos <= platform_y_pos + AsPlatform::Height)
	{
		if (next_x_pos >= platform->X_Pos and next_x_pos <= platform->X_Pos + platform->Width)
		{
			Ball_Direction = -Ball_Direction;
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
		}
	}

    level->Check_Level_Brick_Hit(next_x_pos, next_y_pos, Ball_Direction);

	//Ball_Direction = fmod(Ball_Direction, 2 * M_PI);

    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;

    Redraw(engine);
}
//------------------------------------------------------------------------------------------------------------




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
void AsPlatform::Redraw(AsEngine *engine)
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = (X_Pos) * AsConfig::Global_Scale;
    Platform_Rect.top = Y_Pos * AsConfig::Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

    InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
    InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle(HDC hdc, int x, int y)
{
    Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Height) * AsConfig::Global_Scale, (y + Height) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{ 
    RECT intersection_rect;

    int x = X_Pos;
    int y = Y_Pos;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
        return;

    SelectObject(hdc, engine->BG_Pen);
    SelectObject(hdc, engine->BG_Brush);
    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    SelectObject(hdc, Platform_Side_Pen);
    SelectObject(hdc, Platform_Side_Brush);

    Draw_Circle(hdc, x, y);
    Draw_Circle(hdc, x + Inner_Width, y);

    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);
    RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale, AsConfig::Global_Scale * 3, AsConfig::Global_Scale * 3);

    SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Height - 1) * AsConfig::Global_Scale, (y + Height - 1) * AsConfig::Global_Scale, 
        (x + Height / 2) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, 
        x * AsConfig::Global_Scale, (y + Height / 2 + 1) * AsConfig::Global_Scale);

}
//------------------------------------------------------------------------------------------------------------




//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
    :Hwnd(0),BG_Pen(0), BG_Brush(0)
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
    Hwnd = hwnd;

    SetTimer(Hwnd, Timer_ID, 1000 / 20, NULL);

    Ball.Redraw(this);
    Platform.Redraw(this);

    Ball.Init();
    Level.Init();
    Platform.Init();
    Border.Init();

    //BG
    AsConfig::Create_Pen_Brush(BG_Pen, BG_Brush, 15, 15, 35);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
    int i;
    RECT intersection_rect{};

    Ball.Draw(hdc, paint_area, this);
    Level.Draw(hdc, paint_area);
    Platform.Draw(hdc, paint_area, this);
    Border.Draw(hdc, BG_Pen, BG_Brush);

    /*for (i = 0; i < 16; i++)
    {
    Draw_Brick_Letter(hdc, 100 + i * Cell_Width * AsConfig::Global_Scale, 250, EBT_Blue, ELT_O, i);
    Draw_Brick_Letter(hdc, 100 + i * Cell_Width * AsConfig::Global_Scale, 300, EBT_Red, ELT_O, i);
    }*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.X_Pos -= Platform.X_Step;

        if (Platform.X_Pos <= AsBorder::Border_X_Offset)
            Platform.X_Pos = AsBorder::Border_X_Offset;

        Platform.Redraw(this);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= AsBorder::Max_X_Pos + 1 - Platform.Width)
            Platform.X_Pos = AsBorder::Max_X_Pos + 1 - Platform.Width;

        Platform.Redraw(this);
        break;

    case EKT_Space:
        break;
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
    Ball.Move(this, &Level, &Platform);
    return 0;
}
//------------------------------------------------------------------------------------------------------------