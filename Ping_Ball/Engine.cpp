#include "Engine.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
    : Game_State(EGS_Play_Level)
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
    AsConfig::Hwnd = hwnd;

    SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / FPS, NULL);

    AActive_Brick::Setup_Colors();

    ABall::Add_Hit_Checker(&Border);
    ABall::Add_Hit_Checker(&Level);
    ABall::Add_Hit_Checker(&Platform);

	Platform.Set_State(EPS_Normal);
    Platform.Redraw();

    Ball.Init();
    Level.Init();
    Platform.Init();
    Border.Init();

    Level.Set_Current_Level(ALevel::Level_01);

    if (Game_State == EGS_Restart_Level)
    {
        Ball.Set_State(EBS_Lost);
        Platform.Set_State(EPS_Rolling);
        Ball.Redraw();
        Platform.Redraw();
    }
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
    int i;
    RECT intersection_rect{};

    Platform.Draw(hdc, paint_area);
    Ball.Draw(hdc, paint_area);
    Level.Draw(hdc, paint_area);
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
		if (Platform.Get_State() != EPS_Normal)
			return 0;

        Platform.X_Pos -= Platform.X_Step;

        if (Platform.X_Pos <= AsConfig::Border_X_Offset)
            Platform.X_Pos = AsConfig::Border_X_Offset;

        Platform.Redraw();
        break;

    case EKT_Right:
        if (Platform.Get_State() != EPS_Normal)
            return 0;

        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= AsConfig::Max_X_Pos + 1 - Platform.Width)
            Platform.X_Pos = AsConfig::Max_X_Pos + 1 - Platform.Width;

        Platform.Redraw();
        break;

    case EKT_Space:
		if (Ball.Get_State() == EBS_On_Platform)
		{
			Ball.Set_State(EBS_Normal);
			Platform.Set_State(EPS_Normal);
			Ball.Redraw();
		}
        break;
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
    AsConfig::Current_Timer_Tick += 1;

    switch (Game_State)
    {
    case EGS_Play_Level:
        Ball.Move();

        if (Ball.Get_State() == EBS_Lost)
        {
			Game_State = EGS_Lost_Ball;
            Platform.Set_State(EPS_Meltdown);
        }

        break;

    case EGS_Lost_Ball:

        if (Platform.Get_State() == EPS_Missing)
        {
            Game_State = EGS_Restart_Level;
            Platform.Set_State(EPS_Rolling);
        }
        break;

    case EGS_Restart_Level:
        if (Platform.Get_State() == EPS_Is_Ready)
        {
            Game_State = EGS_Play_Level;
            Ball.Set_State(EBS_On_Platform);
        }
        break;
    }

    Platform.Act();
    //Level.Active_Brick.Act();

    return 0;
}
//------------------------------------------------------------------------------------------------------------