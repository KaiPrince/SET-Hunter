#pragma once
#include "GameBoardTile.h"
#include "AssetFactory.h"

class GameBoard;

class TileFactory
{
	GameBoard* gb;
	AssetFactory* _assetFactory;
public:
	TileFactory(GameBoard* gb, AssetFactory* assetFactory);
	~TileFactory();
	GameBoardTile * CreateSquare(int x, int y, float width, float height);
};

