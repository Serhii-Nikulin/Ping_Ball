#include "Level.h"

//------------------------------------------------------------------------------------------------------------
unsigned char ALevel::Level_01[ALevel::Level_Height][ALevel::Level_Width] = {
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
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//------------------------------------------------------------------------------------------------------------
//ALevel
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
    : Level_Rect{}, Current_Brick_Left_Pos(0), Current_Brick_Right_Pos(0), Current_Brick_Top_Pos(0), Current_Brick_Bottom_Pos(0), Current_Level{}, Active_Bricks_Count(0), Active_Bricks{}, Falling_Letter_Count(0), Falling_Letters{}
{
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
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

    min_level_x = (min_ball_x - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;
    max_level_x = (max_ball_x - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;
    min_level_y = (min_ball_y - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;
    max_level_y = (max_ball_y - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;


    for (i = max_level_y; i >= min_level_y; i--)
        for (j = min_level_x; j <= max_level_x; j++)
        {
            if(Current_Level[i][j] == 0)
                continue;

            Current_Brick_Left_Pos = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
            Current_Brick_Right_Pos = Current_Brick_Left_Pos + AsConfig::Brick_Width;
            Current_Brick_Top_Pos = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
            Current_Brick_Bottom_Pos = Current_Brick_Top_Pos + AsConfig::Brick_Height;

			is_hit_from_vertical = Check_Hit_From_Vertical(next_x_pos, next_y_pos, ball, j, i, distance_y);
			is_hit_from_horizontal = Check_Hit_From_Horizontal(next_x_pos, next_y_pos, ball, j, i, distance_x);

            if (is_hit_from_vertical and is_hit_from_horizontal)
            {
                if(distance_x < distance_y)
					ball->Reflect(true);
				else
					ball->Reflect(false);
                
                On_Hit(j, i);
                
				return true;
            }
			else if (is_hit_from_vertical)
			{
				ball->Reflect(false);
                On_Hit(j, i);

				return true;
			}
			else if (is_hit_from_horizontal)
			{
				ball->Reflect(true);
                On_Hit(j, i);

				return true;
			}
        }

    return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit_From_Horizontal(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance)
{
    double direction = ball->Get_Direction();
    double radius = ball->Radius;

    // Hit from bottom side of the brick
    if (ball->Is_Moving_Up() )
    {
        if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Bottom_Pos, next_x_pos, Current_Brick_Left_Pos, Current_Brick_Right_Pos, radius, distance) )
        {
            if (brick_y == Level_Height - 1 or (brick_y < Level_Height - 1 and Current_Level[brick_y + 1][brick_x] == 0) )
                return true;
            else
                return false;
        }
    }
    else // Hit from top side of the brick
    {
        if (Hit_Circle_On_Line(Current_Brick_Top_Pos - next_y_pos, next_x_pos, Current_Brick_Left_Pos, Current_Brick_Right_Pos, radius, distance) )
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
bool ALevel::Check_Hit_From_Vertical(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance)
{
    double direction = ball->Get_Direction();
    double radius = ball->Radius;

    // Hit from right side of the brick
    if (ball->Is_Moving_Left() )
    {
        if (Hit_Circle_On_Line(next_x_pos - Current_Brick_Right_Pos, next_y_pos, Current_Brick_Top_Pos, Current_Brick_Bottom_Pos, radius, distance) )
        {
            if (brick_x < Level_Width - 1 and Current_Level[brick_y][brick_x + 1] == 0)
                return true;
            else
                return false;
        }
    }
    else // Hit from left side of the brick
    {
        if (Hit_Circle_On_Line(Current_Brick_Left_Pos - next_x_pos, next_y_pos, Current_Brick_Top_Pos, Current_Brick_Bottom_Pos, radius, distance) )
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
void ALevel::Add_Active_Brick(EBrick_Type brick_type, int brick_x, int brick_y)
{
    int i;
    AActive_Brick *active_brick = 0;

    if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
        return;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Red:
    case EBT_Blue:
        active_brick = new AActive_Brick(brick_type, brick_x, brick_y);
        Current_Level[brick_y][brick_x] = EBT_None;
        break;

    default:
        return;
    }

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
bool ALevel::Add_Falling_Letter(EBrick_Type brick_type, int brick_x, int brick_y)
{
    int i;
    AFalling_Letter *falling_letter;
    int letter_x, letter_y;
 
    ELetter_Type letter_type = ELT_O;

    if (brick_type == EBT_Blue or brick_type == EBT_Red)
    {
        if (AsConfig::Rand(AsConfig::Hits_Per_Letter) == 0)
        {
            if (Falling_Letter_Count < AsConfig::Max_Falling_Letter_Count)
            {
                for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
                {
                    if (Falling_Letters[i] == 0)
                    {
                        letter_x = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
                        letter_y = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
                        falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);

                        Falling_Letters[i] = falling_letter;
                        Falling_Letter_Count += 1;

                        break;
                    }

                }

                return true;
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::On_Hit(int brick_x, int brick_y)
{
    EBrick_Type brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

    if (Add_Falling_Letter(brick_type, brick_x, brick_y) )
        Current_Level[brick_y][brick_x] = EBT_None;
    else
        Add_Active_Brick(brick_type, brick_x, brick_y);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
    Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
    Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
    Level_Rect.right = Level_Rect.left + Level_Width * AsConfig::Cell_Width * AsConfig::Global_Scale;
    Level_Rect.bottom = Level_Rect.top + Level_Height * AsConfig::Cell_Height * AsConfig::Global_Scale;

    AsConfig::Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255) );

    AsConfig::Create_Pen_Brush(AsConfig::Brick_Red_Pen, AsConfig::Brick_Red_Brush, AsConfig::Red_Brick_Color);
    AsConfig::Create_Pen_Brush(AsConfig::Brick_Blue_Pen, AsConfig::Brick_Blue_Brush, AsConfig::Blue_Brick_Color);

    memset(Active_Bricks, 0, sizeof(Active_Bricks) );
    memset(Falling_Letters, 0, sizeof(Falling_Letters) );
    memset(Current_Level, 0, sizeof(Current_Level) );
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Current_Level(unsigned char level[ALevel::Level_Height][ALevel::Level_Width])
{
    memcpy(Current_Level, level, sizeof(Current_Level) );
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type) const
{
    HBRUSH brush = 0;
    HPEN pen = 0;

    switch (brick_type)
    {
    case EBT_None:
        pen = AsConfig::BG_Pen;
        brush = AsConfig::BG_Brush;
        break;

    case EBT_Red:
        pen = AsConfig::Brick_Red_Pen;
        brush = AsConfig::Brick_Red_Brush;
        break;

    case EBT_Blue:
        pen = AsConfig::Brick_Blue_Pen;
        brush = AsConfig::Brick_Blue_Brush;
        break;

    default:
        return;
    }

    if (pen and brush)
    {
        SelectObject(hdc, pen);
        SelectObject(hdc, brush);
    }

    RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right - 1, brick_rect.bottom - 1, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)
{
	int i, j;
	RECT brick_rect;
	RECT intersection_rect;

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

		Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);
	}

	Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letter_Count, Falling_Letter_Count);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Act()
{
    Act_Objects((AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);

    Act_Objects((AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letter_Count, Falling_Letter_Count);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Act_Objects(AGraphics_Object **object_array, const int max_objects_count, int &objects_count)
{
    int i; 

    for (i = 0; i < max_objects_count; i++)
    {
        if (object_array[i])
        {
            object_array[i]->Act();

            if (object_array[i]->Is_Finished() )
            {
                delete object_array[i];
                object_array[i] = 0;

                objects_count -= 1;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **object_array, const int max_objects_count, int &objects_count)
{
    int i;

    for (i = 0; i < max_objects_count; i++)
        if (object_array[i])
            object_array[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------