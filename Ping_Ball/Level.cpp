#include "Level.h"

//------------------------------------------------------------------------------------------------------------
unsigned char AsLevel::Level_01[AsLevel::Level_Height][AsLevel::Level_Width] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
//AsLevel
//------------------------------------------------------------------------------------------------------------
AsLevel::~AsLevel()
{
	delete[] Teleport_Bricks;
	delete Advertisement;
}
//------------------------------------------------------------------------------------------------------------
AsLevel::AsLevel()
	: Cancel_All_Activity(false), Level_Rect{}, Current_Brick_Left_Pos(0), Current_Brick_Right_Pos(0), Current_Brick_Top_Pos(0), Current_Brick_Bottom_Pos(0), Current_Level{}, Active_Bricks_Count(0), Active_Bricks{}, Falling_Letters_Count(0), Falling_Letters{}, Teleport_Bricks(0), Teleport_Bricks_Count(0), Advertisement(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	int i, j;
	int min_ball_x, max_ball_x;
	int min_ball_y, max_ball_y;
	int min_level_x, max_level_x;
	int min_level_y, max_level_y;

	bool is_hit_from_vertical;
	bool is_hit_from_horizontal;
	double distance_x, distance_y;

	double radius = ball->Radius;

	min_ball_x = (int)(next_x_pos - radius);
	max_ball_x = (int)(next_x_pos + radius);
	min_ball_y = (int)(next_y_pos - radius);
	max_ball_y = (int)(next_y_pos + radius);

	if (max_ball_y > AsConfig::Level_Y_Offset + (Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height)
		return false;

	if (max_ball_x > AsConfig::Level_X_Offset + (Level_Width - 1) * AsConfig::Cell_Width + AsConfig::Brick_Width)
		return false;

	// validation
	min_level_x = (min_ball_x - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;

	if (min_level_x < 0)
		min_level_x = 0;

	max_level_x = (max_ball_x - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;

	if (max_level_x >= Level_Width - 1)
		max_level_x = Level_Width - 1;

	min_level_y = (min_ball_y - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;

	if (min_level_y < 0)
		min_level_y = 0;

	max_level_y = (max_ball_y - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;

	if (max_level_y >= Level_Height - 1)
		max_level_y = Level_Height - 1;

	for (i = max_level_y; i >= min_level_y; i--)
		for (j = min_level_x; j <= max_level_x; j++)
		{
			if (Current_Level[i][j] == 0)
				continue;

			Current_Brick_Left_Pos = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
			Current_Brick_Right_Pos = Current_Brick_Left_Pos + AsConfig::Brick_Width;
			Current_Brick_Top_Pos = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
			Current_Brick_Bottom_Pos = Current_Brick_Top_Pos + AsConfig::Brick_Height;

			is_hit_from_vertical = Check_Hit_From_Vertical(next_x_pos, next_y_pos, ball, j, i, distance_y);
			is_hit_from_horizontal = Check_Hit_From_Horizontal(next_x_pos, next_y_pos, ball, j, i, distance_x);

			if (is_hit_from_vertical and is_hit_from_horizontal)
			{
				if (On_Hit(j, i, ball, true))
				{
					if (distance_x < distance_y)
						ball->Reflect(true);
					else
						ball->Reflect(false);
				}

				return true;
			}
			else if (is_hit_from_vertical)
			{
				if (On_Hit(j, i, ball, true))
					ball->Reflect(false);

				return true;
			}
			else if (is_hit_from_horizontal)
			{
				if (On_Hit(j, i, ball, false))
				{
					ball->Reflect(true);
				}

				return true;
			}
		}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit_From_Horizontal(double next_x_pos, double next_y_pos, ABall* ball, int brick_x, int brick_y, double& distance)
{
	double direction = ball->Get_Direction();
	double radius = ball->Radius;

	// Hit from bottom side of the brick
	if (ball->Is_Moving_Up())
	{
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Bottom_Pos, next_x_pos, Current_Brick_Left_Pos, Current_Brick_Right_Pos, radius, distance))
		{
			if (brick_y == Level_Height - 1 or (brick_y < Level_Height - 1 and Current_Level[brick_y + 1][brick_x] == 0))
				return true;
			else
				return false;
		}
	}
	else // Hit from top side of the brick
	{
		if (Hit_Circle_On_Line(Current_Brick_Top_Pos - next_y_pos, next_x_pos, Current_Brick_Left_Pos, Current_Brick_Right_Pos, radius, distance))
		{
			if (brick_y > 0 and Current_Level[brick_y - 1][brick_x] == 0)
				return true;
			else
				return false;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit_From_Vertical(double next_x_pos, double next_y_pos, ABall* ball, int brick_x, int brick_y, double& distance)
{
	double direction = ball->Get_Direction();
	double radius = ball->Radius;

	// Hit from right side of the brick
	if (ball->Is_Moving_Left())
	{
		if (Hit_Circle_On_Line(next_x_pos - Current_Brick_Right_Pos, next_y_pos, Current_Brick_Top_Pos, Current_Brick_Bottom_Pos, radius, distance))
		{
			if (brick_x < Level_Width - 1 and Current_Level[brick_y][brick_x + 1] == 0)
				return true;
			else
				return false;
		}
	}
	else // Hit from left side of the brick
	{
		if (Hit_Circle_On_Line(Current_Brick_Left_Pos - next_x_pos, next_y_pos, Current_Brick_Top_Pos, Current_Brick_Bottom_Pos, radius, distance))
		{
			if (brick_x > 0 and Current_Level[brick_y][brick_x - 1] == 0)
				return true;
			else
				return false;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Create_New_Active_Brick(EBrick_Type brick_type, int brick_x, int brick_y, ABall* ball, bool vertical_hit)
{
	AActive_Brick* active_brick = 0;

	if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
		return true;

	switch (brick_type)
	{
	case EBT_None:
		return true;

	case EBT_Red:
	case EBT_Blue:
		active_brick = new AActive_Brick_Red_Blue(brick_type, brick_x, brick_y);
		Current_Level[brick_y][brick_x] = EBT_None;
		break;

	case EBT_Unbreakable:
		active_brick = new AActive_Brick_Unbreakable(brick_type, brick_x, brick_y);
		break;

	case EBT_Multihit_1:
		Current_Level[brick_y][brick_x] = EBT_None;
		active_brick = new AActive_Brick_Multihit(EBT_Multihit_1, brick_x, brick_y);

		break;

	case EBT_Multihit_2:
	case EBT_Multihit_3:
	case EBT_Multihit_4:
		Current_Level[brick_y][brick_x] = brick_type - 1;
		break;

	case EBT_Parachute:
		AsConfig::Throw();

	case EBT_Teleport:
		Add_Active_Brick_Teleport(brick_x, brick_y, ball, vertical_hit);
		return false;

	case EBT_Ad:
		active_brick = new AActive_Brick_Ad(EBT_Ad, brick_x, brick_y, Advertisement);
		Current_Level[brick_y][brick_x] = EBT_Invisible;
		break;

	case EBT_Invisible:
		break;

	default:
		AsConfig::Throw();
	}

	if (active_brick != 0)
		Add_New_Active_Brick(active_brick);

	return true;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall* ball, bool vertical_hit)
{
	int i;
	bool got_hit;
	int dest_brick_x, dest_brick_y;
	AActive_Brick* active_brick = 0;
	AActive_Brick* teleport_destination_brick = 0;
	EDirection_Type direction;

	double before_brick_pos_x, before_brick_pos_y;
	double inside_brick_pos_x, inside_brick_pos_y;

	ball->Get_Center(before_brick_pos_x, before_brick_pos_y);

	teleport_destination_brick = Select_Teleport_Destination_Brick(brick_x, brick_y);

	teleport_destination_brick->Get_Level_Pos(dest_brick_x, dest_brick_y);
	ball->Get_Center(inside_brick_pos_x, inside_brick_pos_y);

	active_brick = new AActive_Brick_Teleport(EBT_Teleport, brick_x, brick_y, ball, teleport_destination_brick);

	if (vertical_hit)
	{
		if (before_brick_pos_y < inside_brick_pos_y)
			direction = EDT_Down;
		else
			direction = EDT_Up;
	}
	else
	{
		if (before_brick_pos_x < inside_brick_pos_x)
			direction = EDT_Right;
		else
			direction = EDT_Left;
	}

	got_hit = false;

	for (i = 0; i < 4; i++)
	{
		switch (direction)
		{
		case EDT_Left:
			if (dest_brick_x > 0 and Current_Level[dest_brick_y][dest_brick_x - 1] == 0)
			{
				got_hit = true;
				ball->Set_Direction(M_PI - M_PI_4);
			}
			break;

		case EDT_Up:
			if ((dest_brick_y > 0) and Current_Level[dest_brick_y - 1][dest_brick_x] == 0)
			{
				got_hit = true;
				ball->Set_Direction(M_PI_4);
			}
			break;

		case EDT_Right:
			if (dest_brick_x < (Level_Width - 1) and Current_Level[dest_brick_y][dest_brick_x + 1] == 0)
			{
				got_hit = true;
				ball->Set_Direction(M_PI_4);
			}
			break;

		case EDT_Down:
			if (dest_brick_x < (Level_Height - 1) and Current_Level[dest_brick_y + 1][dest_brick_x] == 0)
			{
				got_hit = true;
				ball->Set_Direction(-M_PI_4);
			}
			break;

		default:
			AsConfig::Throw();
		}

		if (got_hit)
			break;

		direction = (EDirection_Type)((direction + 1) % 4);
		//ball->Set_Direction(ball->Get_Direction() + M_PI_2);
	}

	((AActive_Brick_Teleport*)teleport_destination_brick)->Release_Direction = direction;

	Add_New_Active_Brick(active_brick);
	Add_New_Active_Brick(teleport_destination_brick);
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick* AsLevel::Select_Teleport_Destination_Brick(int brick_x, int brick_y)
{
	int dest_index;
	AActive_Brick* teleport_destination_brick = 0;

	if (Teleport_Bricks_Count < 2)
		AsConfig::Throw();

	dest_index = AsConfig::Rand(Teleport_Bricks_Count);

	if (brick_x == Teleport_Bricks[dest_index].X and brick_y == Teleport_Bricks[dest_index].Y)
		dest_index = (dest_index + 1) % Teleport_Bricks_Count;


	teleport_destination_brick = new AActive_Brick_Teleport(EBT_Teleport, Teleport_Bricks[dest_index].X, Teleport_Bricks[dest_index].Y, (ABall*)0, (AActive_Brick*)0);
	//teleport_destination_brick = new AActive_Brick_Teleport(EBT_Teleport, 1, 1, (ABall*)0, (AActive_Brick*)0);

	return teleport_destination_brick;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_New_Active_Brick(AActive_Brick* active_brick)
{
	int i;

	for (i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
	{
		if (Active_Bricks[i] == 0)
		{
			Active_Bricks[i] = active_brick;
			Active_Bricks_Count += 1;
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Redraw_Brick(int brick_x, int brick_y)
{
	RECT brick_rect{};

	brick_rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	brick_rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	InvalidateRect(AsConfig::Hwnd, &brick_rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(EBrick_Type brick_type, int brick_x, int brick_y)
{
	int i;
	AFalling_Letter* falling_letter;
	int letter_x, letter_y;

	ELetter_Type letter_type;

	if (!(brick_type == EBT_Blue or brick_type == EBT_Red))
		return false;

	if (AsConfig::Rand(AsConfig::Hits_Per_Letter) != 0)
		return false;

	if (Falling_Letters_Count >= AsConfig::Max_Falling_Letter_Count)
		return false;

	for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
	{
		if (Falling_Letters[i] == 0)
		{
			letter_x = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
			letter_y = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;

			//letter_type = AFalling_Letter::Get_Random_Letter_Type();
			letter_type = ELT_T;
			falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);

			Falling_Letters[i] = falling_letter;
			Falling_Letters_Count += 1;

			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::On_Hit(int brick_x, int brick_y, ABall* ball, bool vertical_hit)
{
	bool can_reflect = true;
	EBrick_Type brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

	if (brick_type == EBT_Parachute)
	{
		ball->Set_On_Parachute(brick_x, brick_y);
		Current_Level[brick_y][brick_x] = EBT_None;
		can_reflect = false;
	}
	else if (Add_Falling_Letter(brick_type, brick_x, brick_y))
		Current_Level[brick_y][brick_x] = EBT_None;
	else
		can_reflect = Create_New_Active_Brick(brick_type, brick_x, brick_y, ball, vertical_hit);

	Redraw_Brick(brick_x, brick_y);

	return can_reflect;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Init()
{
	Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + Level_Width * AsConfig::Cell_Width * AsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + Level_Height * AsConfig::Cell_Height * AsConfig::Global_Scale;

	AsConfig::Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255));

	memset(Active_Bricks, 0, sizeof(Active_Bricks));
	memset(Falling_Letters, 0, sizeof(Falling_Letters));
	memset(Current_Level, 0, sizeof(Current_Level));
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Set_Current_Level(unsigned char level[AsLevel::Level_Height][AsLevel::Level_Width])
{
	int i, j;
	int teleport_index = 0;
	EBrick_Type brick_type;

	memcpy(Current_Level, level, sizeof(Current_Level));

	Teleport_Bricks_Count = 0;

	for (i = 0; i < Level_Height; i++)
	{
		for (j = 0; j < Level_Width; j++)
		{
			brick_type = (EBrick_Type)Current_Level[i][j];

			if (brick_type == EBT_Teleport)
				Teleport_Bricks_Count += 1;
		}
	}

	if (Teleport_Bricks_Count == 1)
		AsConfig::Throw();

	delete[] Teleport_Bricks;
	Teleport_Bricks = 0;

	Teleport_Bricks = new SPoint[Teleport_Bricks_Count];

	for (i = 0; i < Level_Height; i++)
	{
		for (j = 0; j < Level_Width; j++)
		{
			brick_type = (EBrick_Type)Current_Level[i][j];

			if (brick_type == EBT_Teleport)
			{
				Teleport_Bricks[teleport_index] = SPoint{ j, i };
				teleport_index += 1;
			}
		}
	}

	Advertisement = new AsAdvertisement(9, 6, 2, 3);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Get_Next_Falling_Letter(int& index, AFalling_Letter** falling_letter)
{
	AFalling_Letter* current_letter;

	if (Falling_Letters_Count == 0)
		return false;

	if (index < 0 or index >= AsConfig::Max_Falling_Letter_Count)
		return false;

	while (index < AsConfig::Max_Falling_Letter_Count)
	{
		current_letter = Falling_Letters[index++];

		if (current_letter != 0)
		{
			*falling_letter = current_letter;
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Brick(HDC hdc, RECT& brick_rect, EBrick_Type brick_type) const
{
	switch (brick_type)
	{
	case EBT_None:
	case EBT_Red:
	case EBT_Blue:
		AActive_Brick_Red_Blue::Draw_In_Level(hdc, brick_type, brick_rect);
		break;

	case EBT_Unbreakable:
		AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
		break;

	case EBT_Multihit_1:
	case EBT_Multihit_2:
	case EBT_Multihit_3:
	case EBT_Multihit_4:
		AActive_Brick_Multihit::Draw_In_Level(hdc, brick_type, brick_rect);
		break;

	case EBT_Parachute:
		Draw_Parachute_In_Level(hdc, brick_rect);
		break;

	case EBT_Teleport:
		AActive_Brick_Teleport::Draw_In_Level(hdc, brick_rect);
		break;

	case EBT_Ad:
		AActive_Brick_Ad::Draw_In_Level(hdc, brick_rect);
		break;

	case EBT_Invisible:
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j;
	RECT brick_rect{};
	RECT intersection_rect{};

	if (Advertisement)
		Advertisement->Draw(hdc, paint_area);

	Clear_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letter_Count);

	if (Cancel_All_Activity)
		Destroy_All_Active_Objects();

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{
		for (i = 0; i < Level_Height; i++)
			for (j = 0; j < Level_Width; j++)
			{
				brick_rect.left = (AsConfig::Level_X_Offset + j * AsConfig::Cell_Width) * AsConfig::Global_Scale;
				brick_rect.top = (AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height) * AsConfig::Global_Scale;
				brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
				brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

				if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
					Draw_Brick(hdc, brick_rect, static_cast<EBrick_Type>(Current_Level[i][j]));
			}

		Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);
	}

	Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letter_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act()
{

	Act_Objects((AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);

	Act_Objects((AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letter_Count, Falling_Letters_Count);

	if (Advertisement)
		Advertisement->Act();
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Stop_Activity()
{
	Cancel_All_Activity = true;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act_Objects(AGraphics_Object** object_array, const int max_objects_count, int& objects_count)
{
	int i;

	for (i = 0; i < max_objects_count; i++)
	{
		if (object_array[i])
		{
			object_array[i]->Act();

			if (object_array[i]->Is_Finished())
			{
				delete object_array[i];
				object_array[i] = 0;

				objects_count -= 1;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, const int max_objects_count)
{
	int i;

	for (i = 0; i < max_objects_count; i++)
		if (object_array[i])
			object_array[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Clear_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, const int max_objects_count)
{
	int i;

	for (i = 0; i < max_objects_count; i++)
		if (object_array[i])
			object_array[i]->Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Destroy_All_Active_Objects()
{
	Cancel_All_Activity = false;
	Destroy_Objects((AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);
	Destroy_Objects((AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letter_Count, Falling_Letters_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Destroy_Objects(AGraphics_Object** object_array, const int max_objects_count, int& objects_count)
{
	int i;

	for (i = 0; i < max_objects_count; i++)
		if (object_array[i])
		{
			delete object_array[i];
			object_array[i] = 0;
		}

	objects_count = 0;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Parachute_In_Level(HDC hdc, RECT& brick_rect) const
{
	Draw_Elements_Of_Parachute(hdc, brick_rect, 0, 4);
	Draw_Elements_Of_Parachute(hdc, brick_rect, 4, 6);
	Draw_Elements_Of_Parachute(hdc, brick_rect, 10, 4);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Elements_Of_Parachute(HDC hdc, RECT& brick_rect, int offset, int width) const
{
	RECT rect{};
	const int& scale = AsConfig::Global_Scale;

	AsConfig::Parachute_Color.Select(hdc);

	rect.left = brick_rect.left + offset * scale + 1;
	rect.top = brick_rect.top + 1;
	rect.right = rect.left + width * scale + 1;
	rect.bottom = brick_rect.top + 4 * scale - 1;

	AsConfig::Round_Rect(hdc, rect);

	rect.top += 3 * scale;
	rect.bottom += 3 * scale;

	AsConfig::Round_Rect(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------



