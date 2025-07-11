#include "Active_Brick.h"

// AGraphics_Object
//------------------------------------------------------------------------------------------------------------
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Get_Level_Pos(int &dest_brick_x, int &dest_brick_y)
{
	dest_brick_x = Level_X;
	dest_brick_y = Level_Y;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Clear(HDC hdc, RECT& paint_area)
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick::~AActive_Brick()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y)
	: Brick_Type(brick_type), Brick_Rect{}
{
	Level_X = brick_x;
	Level_Y = brick_y;

	Brick_Rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	Brick_Rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Red_Blue
AColor AActive_Brick_Red_Blue::Fading_Red_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick_Red_Blue::Fading_Blue_Brick_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int brick_x, int brick_y)
	: AActive_Brick(brick_type, brick_x, brick_y), Fade_Step(0)
{
	if (! (brick_type == EBT_Red || brick_type == EBT_Blue) )
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Act()
{
	if (Fade_Step < Max_Fade_Step - 1)
	{
		Fade_Step += 1;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw(HDC hdc, RECT& paint_area)
{
	switch (Brick_Type)
	{
	case EBT_Red:
		Fading_Red_Brick_Colors[Fade_Step].Select(hdc);
		break;

	case EBT_Blue:
		Fading_Blue_Brick_Colors[Fade_Step].Select(hdc);
		break;

	default:
		return;
	}

	AsConfig::Round_Rect(hdc, Brick_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Red_Blue::Is_Finished()
{
	if (Fade_Step >= Max_Fade_Step - 1)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Setup_Colors()
{
	int i;

	for (i = 0; i < Max_Fade_Step; i++)
	{
		Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Brick_Colors[i]);
		Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
	}
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Red_Blue::Get_Fading_Value(int step, int max_value, int min_value)
{
	return static_cast<unsigned char>(max_value - step * (max_value - min_value) / (Max_Fade_Step - 1));
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor& origin_color, int step, AColor& fading_color)
{
	unsigned char r, g, b;

	r = Get_Fading_Value(step, origin_color.R, AsConfig::BG_Color.R);
	g = Get_Fading_Value(step, origin_color.G, AsConfig::BG_Color.G);
	b = Get_Fading_Value(step, origin_color.B, AsConfig::BG_Color.B);

	fading_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw_In_Level(HDC hdc, EBrick_Type brick_type, RECT& rect)
{
	const AColor* color = 0;

	switch (brick_type)
	{
	case EBT_None:
		color = &AsConfig::BG_Color;
		break;

	case EBT_Red:
		color = &AsConfig::Red_Color;
		break;

	case EBT_Blue:
		color = &AsConfig::Blue_Color;
		break;

	default:
		AsConfig::Throw();
	}

	if (color)
	{
		color->Select(hdc);
		AsConfig::Round_Rect(hdc, rect);
	}
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Unbreakable
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
	DeleteObject(Region);
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(EBrick_Type brick_type, int brick_x, int brick_y)
	: AActive_Brick(brick_type, brick_x, brick_y), Animation_Step(0), Region(0)
{
	Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale - 1, 2 * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
	if (Animation_Step <= Max_Animation_Step)
	{
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
		Animation_Step += 1;
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
	int offset;

	AActive_Brick_Unbreakable::Draw_In_Level(hdc, Brick_Rect);
	SelectClipRgn(hdc, Region);
	offset = -9 * AsConfig::Global_Scale + Animation_Step * AsConfig::Global_Scale * 2;

	AsConfig::Blue_Highlight_Unbreakable.Select_Pen(hdc);
	MoveToEx(hdc, Brick_Rect.left - 1 * AsConfig::Global_Scale + offset, Brick_Rect.top + 8 * AsConfig::Global_Scale, 0);
	LineTo(hdc, Brick_Rect.left + 9 * AsConfig::Global_Scale + offset, Brick_Rect.top - 2 * AsConfig::Global_Scale);

	AsConfig::Red_Highlight_Unbreakable.Select_Pen(hdc);
	MoveToEx(hdc, Brick_Rect.left + 1 * AsConfig::Global_Scale + offset, Brick_Rect.top + 8 * AsConfig::Global_Scale, 0);
	LineTo(hdc, Brick_Rect.left + 10 * AsConfig::Global_Scale + offset, Brick_Rect.top - 1 * AsConfig::Global_Scale);

	SelectClipRgn(hdc, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
	if (Animation_Step > Max_Animation_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT& rect)
{
	AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Multihit
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::~AActive_Brick_Multihit()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::AActive_Brick_Multihit(EBrick_Type brick_type, int brick_x, int brick_y)
	: AActive_Brick(brick_type, brick_x, brick_y), Rotation_Step(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Act()
{
	if (Rotation_Step < Max_Rotation_Step)
	{
		Rotation_Step += 1;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw(HDC hdc, RECT& paint_area)
{
	RECT rect{};
	XFORM old_xform{}, new_xform{};
	const int& scale = AsConfig::Global_Scale;
	int rotation_step;
	double rotation_angle;

	rotation_step = Rotation_Step % Rotation_Step_Per_Turn;
	rotation_angle = rotation_step * 2 * M_PI / Rotation_Step_Per_Turn;

	new_xform.eM11 = (FLOAT)cos(rotation_angle);     new_xform.eM12 = (FLOAT)0;
	new_xform.eM21 = 0;                              new_xform.eM22 = 1;

	new_xform.eDx = (FLOAT)Brick_Rect.left + AsConfig::Brick_Width * scale / 2.0f;
	new_xform.eDy = (FLOAT)Brick_Rect.top;

	GetWorldTransform(hdc, &old_xform);
	SetWorldTransform(hdc, &new_xform);

	AsConfig::BG_Color.Select(hdc);
	SelectObject(hdc, AsConfig::Letter_Pen);

	MoveToEx(hdc, -5 * scale, 2 * scale, 0);
	LineTo(hdc, -4 * scale, 1 * scale);
	LineTo(hdc, -4 * scale, 6 * scale);

	rect.left = -2 * scale;
	rect.top = 1 * scale;
	rect.right = 1 * scale;
	rect.bottom = 6 * scale;
	AsConfig::Round_Rect(hdc, rect);

	rect.left = 2 * scale;
	rect.top = 1 * scale;
	rect.right = 5 * scale;
	rect.bottom = 6 * scale;
	AsConfig::Round_Rect(hdc, rect);

	SetWorldTransform(hdc, &old_xform);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Multihit::Is_Finished()
{
	if (Rotation_Step >= Max_Rotation_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, EBrick_Type brick_type, RECT& brick_rect)
{
	switch (brick_type)
	{
	case EBT_Multihit_1:
		Draw_Brick_Elements(hdc, brick_rect, 10, 1, 0);
		break;

	case EBT_Multihit_2:
		Draw_Brick_Elements(hdc, brick_rect, 4, 2, 6);
		break;

	case EBT_Multihit_3:
		Draw_Brick_Elements(hdc, brick_rect, 2, 3, 4);
		break;

	case EBT_Multihit_4:
		Draw_Brick_Elements(hdc, brick_rect, 2, 4, 3);
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_Brick_Elements(HDC hdc, RECT& brick_rect, int width, int elements, int offset)
{
	int i;
	int x_pos;
	RECT rect{};
	const int& scale = AsConfig::Global_Scale;

	AsConfig::Red_Color.Select(hdc);
	AsConfig::White_Color.Select_Pen(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	x_pos = brick_rect.left + 3 * scale;

	for (i = 0; i < elements; i++)
	{
		rect.left = x_pos;
		rect.top = brick_rect.top + 3 * scale;
		rect.right = rect.left + width * scale;
		rect.bottom = rect.top + 3 * scale;

		AsConfig::BG_Color.Select(hdc);
		AsConfig::Round_Rect(hdc, rect, 0);

		rect.left -= scale;
		rect.top -= scale;
		rect.right -= scale;
		rect.bottom -= scale;

		AsConfig::Blue_Color.Select(hdc);
		AsConfig::Round_Rect(hdc, rect, 0);

		x_pos += offset * scale;
	}
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Teleport
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport::~AActive_Brick_Teleport()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport::AActive_Brick_Teleport(EBrick_Type brick_type, int brick_x, int brick_y, ABall *ball, AActive_Brick *destination_teleport_brick)
	: AActive_Brick(brick_type, brick_x, brick_y), Animation_Step(0), Ball(0), Teleport_State(ETS_Starting), Destination_Teleport_Brick(destination_teleport_brick)
{
	Set_Ball(ball);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Act()
{
	double direction;
	double ball_x_pos = 0, ball_y_pos = 0;

	if (Animation_Step < Max_Animation_Step)
	{
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
		Animation_Step += 1;
	}
	else
	{
		switch (Teleport_State)
		{
		case ETS_Starting:
			Animation_Step = 0;
			Teleport_State = ETS_Finishing;

			if (Destination_Teleport_Brick != 0)
			{
				( (AActive_Brick_Teleport*)Destination_Teleport_Brick)->Set_Ball(Ball);
				Ball = 0;
			}

			break;

		case ETS_Finishing:
			Animation_Step = 0;
			Teleport_State = ETS_Done;

			if (Ball != 0)
			{
				direction = Ball->Get_Direction();

				switch (Release_Direction)
				{
				case EDT_Left:
					ball_x_pos = Get_Brick_X_Pos(false) - ABall::Radius;
					ball_y_pos = Get_Brick_Y_Pos(true);
					break;

				case EDT_Up:
					ball_x_pos = Get_Brick_X_Pos(true);
					ball_y_pos = Get_Brick_Y_Pos(false) - ABall::Radius;
					break;

				case EDT_Right:
					ball_x_pos = Get_Brick_X_Pos(false) + AsConfig::Brick_Width + ABall::Radius;
					ball_y_pos = Get_Brick_Y_Pos(true);
					break;

				case EDT_Down:
					ball_x_pos = Get_Brick_X_Pos(true);
					ball_y_pos = Get_Brick_Y_Pos(false) + AsConfig::Brick_Height + ABall::Radius;
					break;

				default:
					AsConfig::Throw;
				}

				Ball->Set_State(EBS_Normal, ball_x_pos, ball_y_pos);
				Ball->Set_Direction(direction);
			}

			break;

		case ETS_Done:
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Draw(HDC hdc, RECT& paint_area)
{
	int step = 0;

	switch (Teleport_State)
	{
	case ETS_Starting:
		step = Animation_Step;
		break;

	case ETS_Finishing:
		step = Max_Animation_Step - Animation_Step;
		break;

	default:
		break;
	}

	Draw_In_Level(hdc, Brick_Rect, step);

	if (Ball != 0)
		Ball->Draw_Teleporting(hdc, step);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Teleport::Is_Finished()
{
	if (Teleport_State == ETS_Done)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Draw_In_Level(HDC hdc, RECT& brick_rect, int step)
{
	int top_y, low_y;
	const int& scale = AsConfig::Global_Scale;
	AsConfig::Red_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	top_y = brick_rect.top + 1 + step / 2;
	low_y = brick_rect.top + 6 * scale + 1 - step / 2;

	AsConfig::Teleport_Color.Select(hdc);
	RoundRect(hdc, brick_rect.left + 3 * scale + 1, top_y, brick_rect.left + 11 * scale + 1, low_y, 6 * scale, 6 * scale);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Set_Ball(ABall *ball)
{
	double ball_x, ball_y;

	if (ball == 0)
		return;

	Ball = ball;

	ball_x = AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width + AsConfig::Brick_Width / 2.0;
	ball_y = AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height + AsConfig::Brick_Height / 2.0;

	Ball->Set_State(EBS_Teleporting, ball_x, ball_y);
}
//------------------------------------------------------------------------------------------------------------
double AActive_Brick_Teleport::Get_Brick_X_Pos(bool is_center)
{
	double ball_x_pos;
	ball_x_pos = AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width;

	if (is_center)
		ball_x_pos += AsConfig::Brick_Width / 2.0;

	return ball_x_pos;
}
//------------------------------------------------------------------------------------------------------------
double AActive_Brick_Teleport::Get_Brick_Y_Pos(bool is_center)
{
	double ball_y_pos;
	ball_y_pos = AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height;

	if (is_center)
		ball_y_pos += AsConfig::Brick_Height / 2.0;

	return ball_y_pos;
}
//------------------------------------------------------------------------------------------------------------




//AsAdvertisement
const double AsAdvertisement::Ball_Acc = 0.1;
//------------------------------------------------------------------------------------------------------------
AsAdvertisement::~AsAdvertisement()
{
	int i, j;

	DeleteObject(Ad_Region);

	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
			if (Bricks_Region[i * Width + j] != 0)
			{
				DeleteObject(Bricks_Region[i * Width + j]);
				Bricks_Region[i * Width + j] = 0;
			}

	delete[] Bricks_Region;
}
//------------------------------------------------------------------------------------------------------------
AsAdvertisement::AsAdvertisement(int level_x, int level_y, int width, int height)
	: Level_X(level_x), Level_Y(level_y), Width(width), Height(height), Ad_Rect{}, Ad_Region(0), Table{}, Bricks_Region(0), Ball_Center_X(0), Ball_Center_Y(0), Start_Ball_Y_Pos(0), Distance(0.0), Ratio(0.0), Gradient_Ratio(0.0), Time_Step(0), Time_Gradient(+1)
{
	int ad_x, ad_y;
	int full_distance;
	static const int &scale = AsConfig::Global_Scale;

	Bricks_Region = new HRGN[Width * Height];
	memset(Bricks_Region, 0, Width * Height * sizeof(HRGN) );

	Ad_Rect.left = (AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width) * scale;
	Ad_Rect.top = (AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height) * scale;
	Ad_Rect.right = Ad_Rect.left + (Width * AsConfig::Cell_Width - 1) * scale;
	Ad_Rect.bottom = Ad_Rect.top + (Height * AsConfig::Cell_Height - 1) * scale;

	ad_x = Ad_Rect.left;
	ad_y = Ad_Rect.top;

	Ball_Center_X = ad_x + 10 * scale + int(Ball_Size * scale / 2.0);	
	Ball_Center_Y = ad_y + 1 * scale + int(Ball_Size * scale / 2.0);	

	Start_Ball_Y_Pos = ad_y - Ball_Size;
	Ball_Center_Y = Start_Ball_Y_Pos;
	Top_Theshold = Start_Ball_Y_Pos;
	Bottom_Threshold = ad_y + 12 * scale;

	full_distance = Bottom_Threshold - Top_Theshold;
	Gradient_Ratio = 1.0 / full_distance;

	Table[0] = POINT{ad_x + 1 * scale, ad_y + 15 * scale + 1}; // left
	Table[1] = POINT{ad_x + 15 * scale + 1, ad_y + 10 * scale + 1}; // top
	Table[2] = POINT{ad_x + 30 * scale - 1, ad_y + 15 * scale + 1}; // right
	Table[3] = POINT{ad_x + 15 * scale + 1, ad_y + 20 * scale + 1}; // bottom
}
//------------------------------------------------------------------------------------------------------------
void AsAdvertisement::Act()
{
	int i, j;
	RECT rect{};
	int cell_width = AsConfig::Cell_Width * AsConfig::Global_Scale;
	int cell_height = AsConfig::Cell_Height * AsConfig::Global_Scale;

	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
			if (Bricks_Region[i * Width + j] != 0)
			{
				rect.left = Ad_Rect.left + j * cell_width;
				rect.top = Ad_Rect.top + i * cell_height;
				rect.right = rect.left + cell_width;
				rect.bottom = rect.top + cell_height;

				InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
			}

	if (Ball_Center_Y > Bottom_Threshold)
	{
		Ball_Center_Y = Bottom_Threshold;
		Time_Gradient *= -1;
	}
	else if (Ball_Center_Y < Top_Theshold)
	{
		Ball_Center_Y = Top_Theshold;
		Time_Gradient *= -1;
	}

	Time_Step += Time_Gradient;
	Ratio = 1.0 - Gradient_Ratio / AsConfig::Global_Scale * Distance;

	Distance = Ball_Acc * Time_Step * Time_Step / 2.0;
	Ball_Center_Y = Start_Ball_Y_Pos + (int)Distance;
}
//------------------------------------------------------------------------------------------------------------
void AsAdvertisement::Draw(HDC hdc, RECT &paint_area)
{
	int i, j;
	RECT intersection_rect{};
	int ad_x = Ad_Rect.left;
	int ad_y = Ad_Rect.top;
	const int &scale = AsConfig::Global_Scale;
	int ball_x, ball_y;

	if (! IntersectRect(&intersection_rect, &paint_area, &Ad_Rect) )
		return;

	Ad_Region = CreateRectRgn(0, 0, 0, 0);

	SelectClipRgn(hdc, Ad_Region);

	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
			if (Bricks_Region[i * Width + j] != 0)
				ExtSelectClipRgn(hdc, Bricks_Region[i * Width + j], RGN_OR);

	// 1.bg and blue frame
	AsConfig::BG_Color.Select(hdc);
	AsConfig::Blue_Color.Select_Pen(hdc);
	AsConfig::Round_Rect(hdc, Ad_Rect);

	// 2.1 white table
	AsConfig::White_Color.Select(hdc);
	AsConfig::Blue_Color.Select_Pen(hdc);
	Polygon(hdc, Table, Vertex_Count);

	// 3. blue ball's shadow

	AsConfig::Blue_Color.Select(hdc);
	//Ellipse(hdc, ad_x + 11 * scale, ad_y + 16 * scale, ad_x + 20 * scale, ad_y + 19 * scale - 1);
	Ellipse(hdc, Ball_Center_X - 4.5 * scale * Ratio, ad_y + 17.5 * scale - 1.5 * scale * Ratio, 
		Ball_Center_X + 4.5 * scale * Ratio, ad_y + 17.5 * scale + 1.5 * scale * Ratio - 1);

	// 2.2 red border of table
	AsConfig::Ad_Table_Red_Color.Select(hdc);
	MoveToEx(hdc, ad_x + 1 * scale + 1, ad_y + 17 * scale, 0); // left
	LineTo(hdc, ad_x + 15 * scale + 1, ad_y + 22 * scale - 1); // bottom
	LineTo(hdc, ad_x + 29 * scale + 1, ad_y + 17 * scale); // right

	// 2.3 blue border of table
	AsConfig::Ad_Table_Blue_Color.Select(hdc);

	MoveToEx(hdc, ad_x + 1 * scale, ad_y + 16 * scale - 1, 0); // left
	LineTo(hdc, ad_x + 15 * scale + 1, ad_y + 21 * scale - 1); // bottom
	LineTo(hdc, ad_x + 30 * scale - 1, ad_y + 16 * scale - 1); // right
	
	// 4.1. red ball
	ball_x = Ball_Center_X - Ball_Size * scale / 2.0;
	ball_y = Ball_Center_Y - Ball_Size * scale / 2.0;

	AsConfig::Red_Color.Select(hdc);
	Ellipse(hdc, ball_x, ball_y, ball_x + Ball_Size * scale, ball_y + Ball_Size * scale);

	// 4.2 highlight of red ball
	AsConfig::Ad_Ball_Highlight_Color.Select_Pen(hdc);
	Arc(hdc, ball_x + 2 * scale, ball_y + 2 * scale, ball_x + (Ball_Size - 2) * scale - 1, ball_y + (Ball_Size - 2) * scale - 1, 
		ball_x + (Ball_Size / 2.0) * scale, ball_y, 
		ball_x, ball_y + (Ball_Size / 2.0 + 1) * scale);

	SelectClipRgn(hdc, 0);
	DeleteObject(Ad_Region);
}
//------------------------------------------------------------------------------------------------------------
void AsAdvertisement::Clear(HDC hdc, RECT& paint_area)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsAdvertisement::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsAdvertisement::Show_Under_Brick(int brick_x, int brick_y)
{
	int x, y;
	RECT rect{};
	HRGN region{};

	x = brick_x - Level_X;
	y = brick_y - Level_Y;

	if ( (x < 0 || x >= Width) || (y < 0 || y >= Height) )
		AsConfig::Throw();

	rect.left = Ad_Rect.left + x * AsConfig::Cell_Width * AsConfig::Global_Scale;
	rect.top = Ad_Rect.top + y * AsConfig::Cell_Height * AsConfig::Global_Scale;
	rect.right = rect.left + AsConfig::Cell_Width * AsConfig::Global_Scale;
	rect.bottom = rect.top + AsConfig::Cell_Height * AsConfig::Global_Scale;

	Bricks_Region[y * Width + x] = CreateRectRgnIndirect(&rect);
}
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Ad
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Ad::~AActive_Brick_Ad()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Ad::AActive_Brick_Ad(EBrick_Type brick_type, int brick_x, int brick_y, AsAdvertisement *advertisemetn)
	: AActive_Brick(brick_type, brick_x, brick_y), Advertisement(advertisemetn)
{
	if (Advertisement)
		Advertisement->Show_Under_Brick(brick_x, brick_y);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Act()
{
	/*if (Advertisement)
		Advertisement->Act();*/
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Draw(HDC hdc, RECT &paint_area)
{
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Ad::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Draw_In_Level(HDC hdc, RECT &rect)
{
	int i;
	int x = rect.left;
	int y = rect.top;
	const int& scale = AsConfig::Global_Scale;

	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, rect.left, rect.top, rect.right + 1 * scale, rect.bottom + 1 * scale);

	for (i = 0; i < 2; i++)
	{
		AsConfig::Red_Color.Select(hdc);
		Ellipse(hdc, x, y, x + Ball_Size * scale - 1, y + Ball_Size * scale - 1);

		AsConfig::White_Color.Select(hdc);
		Arc(hdc, x + 1 * scale, y + 1 * scale, x + (Ball_Size - 1) * scale - 1, y + (Ball_Size - 1) * scale - 1, 
			x + (Ball_Size / 2.0) * scale, y, 
			x, y + (Ball_Size / 2.0 + 1) * scale);

		x += (Ball_Size + 1) * AsConfig::Global_Scale;
	}
}
//------------------------------------------------------------------------------------------------------------