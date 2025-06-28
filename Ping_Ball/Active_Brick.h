#pragma once
#include "Ball.h"

enum EBrick_Type: unsigned char {
	EBT_None, 
	EBT_Red, EBT_Blue, 
	EBT_Unbreakable, 
	EBT_Multihit_1, EBT_Multihit_2, EBT_Multihit_3, EBT_Multihit_4,
	EBT_Parachute,
	EBT_Teleport
};
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Draw(HDC hdc, RECT &paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
protected:
	virtual ~AActive_Brick();

	AActive_Brick(EBrick_Type brick_type, int brick_x, int brick_y);

	EBrick_Type Brick_Type;
	RECT Brick_Rect;

	int Level_X, Level_Y;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Red_Blue: public AActive_Brick
{
public:
	virtual ~AActive_Brick_Red_Blue();
	AActive_Brick_Red_Blue(EBrick_Type brick_type, int brick_x, int brick_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Setup_Colors();
	static void Draw_In_Level(HDC hdc, EBrick_Type brick_type, RECT &rect);

private:
	int Fade_Step;

	static unsigned char Get_Fading_Value(int step, int max_value, int min_value);
	static void Get_Fading_Color(const AColor &origin_color, int step, AColor &fading_color);

	static const int Max_Fade_Step = AsConfig::FPS;

	static AColor Fading_Red_Brick_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Brick_Colors[Max_Fade_Step];
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Unbreakable: public AActive_Brick
{
public:
	virtual ~AActive_Brick_Unbreakable();
	AActive_Brick_Unbreakable(EBrick_Type brick_type, int brick_x, int brick_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &rect);

private:
	HRGN Region;
	int Animation_Step;
	static const int Max_Animation_Step = 11;
};
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Multihit
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Multihit : public AActive_Brick
{
public:
	virtual ~AActive_Brick_Multihit();
	AActive_Brick_Multihit(EBrick_Type brick_type, int brick_x, int brick_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, EBrick_Type brick_type, RECT &rect);

private:
	static void Draw_Brick_Elements(HDC hdc, RECT &brick_rect, int width, int elements, int offset);

	int Rotation_Step;
	static const int Rotation_Step_Per_Turn = 20;
	static const int Max_Rotation_Step = 20 * 3;
};
//------------------------------------------------------------------------------------------------------------




// AActive_Brick_Teleport
//------------------------------------------------------------------------------------------------------------
enum ETeleport_State 
{ 
	ETS_Starting, 
	ETS_Finishing, 
	ETS_Done
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Teleport: public AActive_Brick
{
public:
	virtual ~AActive_Brick_Teleport();
	AActive_Brick_Teleport(EBrick_Type brick_type, int brick_x, int brick_y, ABall *ball, AActive_Brick*destination_teleport_brick);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &rect, int step = 0);

	void Set_Ball(ABall *ball);

private:

	ETeleport_State Teleport_State;
	ABall *Ball;
	AActive_Brick* Destination_Teleport_Brick;
	int Animation_Step;
	static const int Max_Animation_Step = 10;
};
//------------------------------------------------------------------------------------------------------------