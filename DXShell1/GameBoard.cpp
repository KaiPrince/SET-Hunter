#include "GameBoard.h"



GameBoard::GameBoard()
{
	for each (Square* square in squares)
	{
		square = nullptr;
	};
	_squareFactory = nullptr;
	_assetFactory = nullptr;
	_grassTerrain = nullptr;
	_roadTerrain = nullptr;
}


GameBoard::~GameBoard()
{
}

void GameBoard::Init()
{
	_squareFactory = new SquareFactory();
	_grassTerrain = (TerrainAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::GRASS_TERRAIN);
	_roadTerrain = (TerrainAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::ROAD_TERRAIN);

	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			squares[column][row] = _squareFactory->CreateSquare(column, row, squareWidth, squareHeight);
			//Square* thisSquare = squares[column][row];
			
		}
	}
}

void GameBoard::SetAssetFactory(AssetFactory * assetFactory)
{
	_assetFactory = assetFactory;
}


