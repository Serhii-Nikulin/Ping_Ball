#pragma once

#include "Falling_Letter.h"


//------------------------------------------------------------------------------------------------------------
struct SPoint
{
	int X, Y;
};
//------------------------------------------------------------------------------------------------------------
class AsLevel: public AHit_Checker
{
public:
	~AsLevel();
	AsLevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Act();
	void Stop_Activity();

	static const int Level_Height = 14;
	static const int Level_Width = 12;
	static unsigned char Level_01[AsLevel::Level_Height][AsLevel::Level_Width];

	void Set_Current_Level(unsigned char level[AsLevel::Level_Height][AsLevel::Level_Width]);
	bool Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter);

private:
	void Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type) const;

	bool Check_Hit_From_Vertical(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distance);
	bool Check_Hit_From_Horizontal(double next_x_pos, double next_y_pos, ABall *ball, int brick_x, int brick_y, double &distnace);

	bool Create_New_Active_Brick(EBrick_Type brick_type, int brick_x, int brick_y, ABall *ball, bool vertical_hit);
	void Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall *ball, bool vertical_hit);
	AActive_Brick* Select_Teleport_Destination_Brick(int brick_x, int brick_y);
	void Add_New_Active_Brick(AActive_Brick *active_brick);
	void Redraw_Brick(int brick_x, int brick_y);

	bool Add_Falling_Letter(EBrick_Type brick_type, int brick_x, int brick_y);
	bool On_Hit(int brick_x, int brick_y, ABall *ball, bool vertical_hit);

	void Act_Objects(AGraphics_Object **object_array, const int max_objects_count, int &objects_count);
	void Draw_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **object_array, const int max_objects_count);
	void Clear_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, const int max_objects_count);
	void Destroy_All_Active_Objects();
	void Destroy_Objects(AGraphics_Object **object_array, const int max_objects_count, int &objects_count);
	void Draw_Parachute_In_Level(HDC hdc, RECT &brick_rect) const;
	void Draw_Elements_Of_Parachute(HDC hdc, RECT &brick_rect, int offset, int width) const;

	bool Cancel_All_Activity;
	RECT Level_Rect{};
	unsigned char Current_Level[AsLevel::Level_Height][AsLevel::Level_Width];
 
	int Current_Brick_Left_Pos;
	int Current_Brick_Right_Pos;
	int Current_Brick_Top_Pos;
	int Current_Brick_Bottom_Pos;

	int Active_Bricks_Count;
	AActive_Brick *Active_Bricks[AsConfig::Max_Active_Bricks_Count];

	int Falling_Letters_Count;
	AFalling_Letter *Falling_Letters[AsConfig::Max_Falling_Letter_Count];

	int Teleport_Bricks_Count;
	SPoint *Teleport_Bricks;

	AsAdvertisement *Advertisement;
};
//------------------------------------------------------------------------------------------------------------