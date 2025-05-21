#include "Engine.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
    :Hwnd(0)
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
    Hwnd = hwnd;

    SetTimer(Hwnd, Timer_ID, 1000 / FPS, NULL);

    AActive_Brick::Setup_Colors();

    Ball.Redraw(Hwnd);
    Platform.Redraw(Hwnd);

    Ball.Init();
    Level.Init();
    Platform.Init();
    Border.Init();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
    int i;
    RECT intersection_rect{};

    Ball.Draw(hdc, paint_area);
    Level.Draw(hdc, paint_area, Hwnd);
    Platform.Draw(hdc, paint_area);
    Border.Draw(hdc);

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

        if (Platform.X_Pos <= AsConfig::Border_X_Offset)
            Platform.X_Pos = AsConfig::Border_X_Offset;

        Platform.Redraw(Hwnd);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= AsConfig::Max_X_Pos + 1 - Platform.Width)
            Platform.X_Pos = AsConfig::Max_X_Pos + 1 - Platform.Width;

        Platform.Redraw(Hwnd);
        break;

    case EKT_Space:
        break;
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
    Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);

    Level.Active_Brick.Act(Hwnd);

    return 0;
}
//------------------------------------------------------------------------------------------------------------