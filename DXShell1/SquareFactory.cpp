#include "SquareFactory.h"
#include "GameBoard.h"
#include "AssetFactory.h"



SquareFactory::SquareFactory(GameBoard* gb, AssetFactory* assetFactory)
{ 
	this->gb = gb;
	this->_assetFactory = assetFactory;
}


SquareFactory::~SquareFactory()
{
}

Square* SquareFactory::CreateSquare(int x, int y, float width, float height)
{
	float xPos = x * width; //TODO: change this to use gb->squareWidth
	float yPos = y * height;

	//Give the square default assets
	DrawableAsset* defaultTerrain = (DrawableAsset*) _assetFactory->GetAsset(DrawableAsset::GRASS_TERRAIN);
	Square* square = new Square(xPos, yPos, width, height, defaultTerrain, gb);
	square->SetGbX(x);
	square->SetGbY(y);


	return square;
}
