#include "Border.h"

//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
{
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	bool got_hit = false;

	int min_x_pos = AsConfig::Border_X_Offset;
	int max_x_pos = AsConfig::Max_X_Pos + 1;
	int min_y_pos = AsConfig::Border_Y_Offset;
	int max_y_pos = AsConfig::Max_Y_Pos + 1;

	const double& radius = ball->Radius;

	if (next_x_pos + radius > max_x_pos)//right limit
	{
		ball->Reflect(false);
		got_hit = true;
	}

	if (next_y_pos - radius < min_y_pos)//top limit
	{
		ball->Reflect(true);
		got_hit = true;
	}


	if (next_x_pos - radius < min_x_pos)//left limit
	{
		ball->Reflect(false);
		got_hit = true;
	}

	if (next_y_pos + radius > max_y_pos)//bottom limit
	{
		if (AsConfig::Has_Floor)
		{
			ball->Reflect(true);
			return true;
		}
		else
			if (next_y_pos > max_y_pos + radius)
				ball->Set_State(EBS_Lost);
	}

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc)
{
	int i;

	for (i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 3 + 4 * i, 0, true);
		Draw_Element(hdc, 2, 1 + 4 * i, false);
		Draw_Element(hdc, 202, 1 + 4 * i, false);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border) const
{
	//contour
	AsConfig::Red_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	//innner part
	AsConfig::Blue_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	//insertion
	AsConfig::BG_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 3) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 2) * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
