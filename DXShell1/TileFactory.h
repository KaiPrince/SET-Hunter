#pragma once
#include "GameBoardTile.h"
#include "AssetFactory.h"

class GameWorld;

class TileFactory
{
	GameWorld* gb;
	AssetFactory* _assetFactory;
public:
	TileFactory(GameWorld* gb, AssetFactory* assetFactory);
	~TileFactory();
	GameBoardTile * CreateSquare(int x, int y, float width, float height);
};

