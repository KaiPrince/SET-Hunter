#pragma once
#include "Square.h"
#include "TerrainAsset.h"
#include "SquareFactory.h"
#include "AssetFactory.h"
class GameBoard
{

	SquareFactory* _squareFactory;
	AssetFactory* _assetFactory;
public:
	GameBoard();
	~GameBoard();
	void Init();

	//Square** GetSquares();
	//TODO: make this private
	static const int boardWidth = 10;
	static const int boardHeight = 10;
	float squareWidth;
	float squareHeight;
	Square* squares[boardWidth][boardHeight];

	void SetAssetFactory(AssetFactory* assetFactory);
	TerrainAsset* _grassTerrain;
	TerrainAsset* _roadTerrain;
};

