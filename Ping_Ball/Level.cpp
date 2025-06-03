#include "Level.h"

// AFalling_Letter
//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
    : Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Letter_Cell{}, Prev_Letter_Cell{}, Rotation_Step(0), Got_Hit(false), Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step)
{
    Letter_Cell.left = X;
    Letter_Cell.top = Y;
    Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

    Prev_Letter_Cell = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT &paint_area)
{
    RECT intersection_rect{};

    if (IntersectRect(&intersection_rect, &Prev_Letter_Cell, &paint_area) )
    {
        SelectObject(hdc, AsConfig::BG_Pen);
        SelectObject(hdc, AsConfig::BG_Brush);
        Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
    }

    if (IntersectRect(&intersection_rect, &Letter_Cell, &paint_area) )
        Draw_Brick_Letter(hdc);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
    int y_offset = AsConfig::Global_Scale;

    Prev_Letter_Cell = Letter_Cell;

    Y += y_offset;
    Letter_Cell.top += y_offset;
    Letter_Cell.bottom += y_offset;

    if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
    {
        Rotation_Step += 1;
        Next_Rotation_Tick += Ticks_Per_Step;
    }

    InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
    if (Got_Hit or Letter_Cell.top > (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{
    XFORM old_xform{}, new_xform{};
    int brick_half_height;
    double rotation_angle;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    int offset;

    if (! (Brick_Type == EBT_Blue || Brick_Type == EBT_Red))
        return;

    brick_half_height = (int)(AsConfig::Brick_Height * AsConfig::Global_Scale / 2.0);
    Rotation_Step %= 16;

    if (Rotation_Step >= 5 and Rotation_Step <= 12)
    {
        if (Brick_Type == EBT_Blue)
            switch_color = false;
        else
            switch_color = true;
    }
    else
    {
        if (Brick_Type == EBT_Red)
            switch_color = true;
        else
            switch_color = false;
    }

    Set_Brick_Letter_Color(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (Rotation_Step == 4 or Rotation_Step == 12)
    {
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, X, Y + brick_half_height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height);


        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, X, Y + brick_half_height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height + AsConfig::Global_Scale - 1);
    }
    else
    {

        rotation_angle = Rotation_Step * 2.0 * M_PI / 16.0;
        new_xform.eM11 = (FLOAT)1;
        new_xform.eM12 = (FLOAT)0;

        new_xform.eM21 = (FLOAT)0;
        new_xform.eM22 = (FLOAT)cos(rotation_angle);

        new_xform.eDx = (FLOAT)X;
        new_xform.eDy = (FLOAT)(Y + brick_half_height);

        offset = sin(rotation_angle) * AsConfig::Global_Scale * 3.0 / 2.0;
      
        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &new_xform);

        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, 0, 0 - brick_half_height - offset, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + brick_half_height - offset);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, 0 - brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, 0 + brick_half_height);

        if (Rotation_Step >= 5 and Rotation_Step <= 12)
        {
            if (Letter_Type == ELT_O)
            {
                SelectObject(hdc, AsConfig::Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, 0 - 5 * AsConfig::Global_Scale / 2, 0 + 10 * AsConfig::Global_Scale, 0 + 5 * AsConfig::Global_Scale / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush) const
{
    if (is_switch_color == true)
    {
        front_pen = AsConfig::Brick_Blue_Pen;
        front_brush = AsConfig::Brick_Blue_Brush;

        back_pen = AsConfig::Brick_Red_Pen;
        back_brush = AsConfig::Brick_Red_Brush;
    }
    else
    {
        front_pen = AsConfig::Brick_Red_Pen;
        front_brush = AsConfig::Brick_Red_Brush;

        back_pen = AsConfig::Brick_Blue_Pen;
        back_brush = AsConfig::Brick_Blue_Brush;
    }

}
//------------------------------------------------------------------------------------------------------------




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
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type) const
{
    RECT brick{};

    HBRUSH brush = 0;
    HPEN pen = 0;

    brick.left = x * AsConfig::Global_Scale;
    brick.top = y * AsConfig::Global_Scale;
    brick.right = brick.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    brick.bottom = brick.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

    switch (brick_type)
    {
    case EBT_None:
        return;

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

    RoundRect(hdc, brick.left, brick.top, brick.right - 1, brick.bottom - 1, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)
{
    int i, j;
    RECT intersection_rect;

    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
    {
        for (i = 0; i < Level_Height; i++)
            for (j = 0; j < Level_Width; j++)
                Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, static_cast<EBrick_Type>(Current_Level[i][j]) );

        for (i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
            if (Active_Bricks[i])
                Active_Bricks[i]->Draw(hdc);
    }

    for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
        if (Falling_Letters[i])
            Falling_Letters[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Act()
{
    int i; 

    for (i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
    {
        if (Active_Bricks[i])
        {
            Active_Bricks[i]->Act();

            if (Active_Bricks[i]->Is_Finished() )
            {
                delete Active_Bricks[i];
                Active_Bricks[i] = 0;

                Active_Bricks_Count -= 1;
            }
        }
    }

    for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
    {
        if (Falling_Letters[i])
        {
            Falling_Letters[i]->Act();

            if (Falling_Letters[i]->Is_Finished() )
            {
                delete Falling_Letters[i];
                Falling_Letters[i] = 0;

                Falling_Letter_Count -= 1;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------
