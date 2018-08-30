#pragma once

struct BoardConfig
{
	// Support only squared board for now
	float width_ = 200; // Width of the destination board in mm
	float border_ = 5; // dead zone around the board in mm
	//float board_height = 200; // Height of the destination board in mm

	float pinThickness_ = 2.0f;
};