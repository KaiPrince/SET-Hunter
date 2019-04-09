#include "TileFactory.h"
#include "GameBoard.h"
#include "AssetFactory.h"
#include "GameWorld.h"



TileFactory::TileFactory(GameWorld* gb, AssetFactory* assetFactory)
{ 
	this->gb = gb;
	this->_assetFactory = assetFactory;
}


TileFactory::~TileFactory()
{
}

GameBoardTile* TileFactory::CreateSquare(int x, int y, float width, float height)
{
	float xPos = x * width; //TODO: change this to use gb->squareWidth
	float yPos = y * height;

	//Give the square default assets
	DrawableAsset* defaultTerrain = (DrawableAsset*) _assetFactory->GetAsset(DrawableAsset::GRASS_TERRAIN);
	GameBoardTile* square = new GameBoardTile(xPos, yPos, width, height, defaultTerrain, gb);
	square->SetGbX(x);
	square->SetGbY(y);


	return square;
}
