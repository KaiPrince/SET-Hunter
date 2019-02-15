#include "SquareFactory.h"



SquareFactory::SquareFactory()
{
}


SquareFactory::~SquareFactory()
{
}

Square* SquareFactory::CreateSquare()
{
	Square* square = new Square();
	return square;
}

Square* SquareFactory::CreateSquare(float x, float y, float width, float height)
{
	Square* square = new Square();
	square->SetRectBoundary(x, y, width, height);
	return square;
}
