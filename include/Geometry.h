#pragma once

#include "Definitions.h"

struct Position
{
	geometry::Vec2 inner;

	Position(int x, int y)
		: inner{ x, y }
	{}
};

struct Visibility
{
	bool shown;

	Visibility(bool state)
		: shown{ state }
	{}
};