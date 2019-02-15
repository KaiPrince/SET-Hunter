#pragma once
#include "Square.h"

class SquareFactory
{
public:
	SquareFactory();
	~SquareFactory();
	Square* CreateSquare();
	Square * CreateSquare(float x, float y, float width, float height);
};

