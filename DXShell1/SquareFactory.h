#pragma once
#include "Square.h"
#include "AssetFactory.h"

class GameBoard;

class SquareFactory
{
	GameBoard* gb;
	AssetFactory* _assetFactory;
public:
	SquareFactory(GameBoard* gb, AssetFactory* assetFactory);
	~SquareFactory();
	Square * CreateSquare(int x, int y, float width, float height);
};

