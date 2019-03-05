#pragma once
#include "Square.h"

class SquareFactory
{
public:
	SquareFactory();
	~SquareFactory();
	Square* CreateSquare();
	Square * CreateSquare(int x, int y, float width, float height);
};

