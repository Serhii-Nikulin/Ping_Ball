#include "Engine.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
//: Game_State(EGS_Play_Level)
	: Game_State(EGS_Lost_Ball), Balls{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	int i;

	AsConfig::Hwnd = hwnd;
	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);
	srand(file_time.dwLowDateTime);

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);

	AActive_Brick_Red_Blue::Setup_Colors();

	ABall::Add_Hit_Checker(&Border);
	ABall::Add_Hit_Checker(&Level);
	ABall::Add_Hit_Checker(&Platform);

	Platform.Set_State(EPS_Normal);
	Platform.Redraw();

	Level.Init();
	AFalling_Letter::Init();

	Level.Set_Current_Level(AsLevel::Level_01);

	if (Game_State == EGS_Lost_Ball)
	{
		Balls[0].Set_State(EBS_Lost);

		for (i = 1; i < AsConfig::Max_Balls_Count; i++)
			Balls[i].Set_State(EBS_Disable);

		Platform.Set_State(EPS_Missing);
		//Ball.Redraw();
		Platform.Redraw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
{
	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);

	Level.Draw(hdc, paint_area);
	Border.Draw(hdc);
	Platform.Draw(hdc, paint_area);

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	int i;

	switch (key_type)
	{
	case EKT_Left:
		Platform.Move(true);
		break;

	case EKT_Right:
		Platform.Move(false);
		break;

	case EKT_Space:

		for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		{
			if (Balls[i].Get_State() == EBS_On_Platform)
			{
				Balls[i].Set_State(EBS_Normal);
				Platform.Set_State(EPS_Normal);
			}
		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	int i;

	AsConfig::Current_Timer_Tick += 1;

	switch (Game_State)
	{
	case EGS_Play_Level:

		for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		{
			Balls[i].Move();

			if (Balls[i].Get_State() == EBS_Lost)
			{
				Game_State = EGS_Lost_Ball;
				Platform.Set_State(EPS_Meltdown);
			}
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
			Balls[0].Set_State(EBS_On_Platform);

			for (i = 1; i < AsConfig::Max_Balls_Count; i++)
				Balls[i].Set_State(EBS_Disable);
			//Balls[i].Set_State(EBS_On_Platform);
		}
		break;
	}

	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int index = 0;
	AFalling_Letter* falling_letter;
	Platform.Act();
	Level.Act();

	while (Level.Get_Next_Falling_Letter(index, &falling_letter))
		if (Platform.Hit_By(falling_letter))
			On_Falling_Letter(falling_letter);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter* falling_letter)
{
	ELetter_Type letter_type;
	letter_type = falling_letter->Letter_Type;

	switch (letter_type)
	{
	case ELT_X: // cancel effects
		break;
	case ELT_R: // reverse
		break;
	case ELT_S: // slow ball
		break;
	case ELT_M: // kill monster
		break;
	case ELT_HP: // get one more life
		break;
	case ELT_W: // wide platform
		break;
	case ELT_C: // clay platform 7
		break;
	case ELT_T: // triple ball 3
		int xxx;
		break;
	case ELT_L: // lazer platform 3
		break;
	case ELT_F: // floor 3
		break;
	case ELT_Plus: // next level 1 
		break;
	default:
		AsConfig::Throw();
	}

	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------