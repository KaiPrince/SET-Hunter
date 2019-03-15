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

	//TODO: make this private
	static const int boardWidth = 10;
	static const int boardHeight = 10;
	static const int roadWidth = 4;
	float squareWidth;
	float squareHeight;
	Square* squares[boardWidth][boardHeight];

	Square** GetSquares() { return *squares; }
	AssetFactory* GetAssetFactory() { return _assetFactory; }

	void SetAssetFactory(AssetFactory* assetFactory);
	TerrainAsset* _grassTerrain;
	TerrainAsset* _roadTerrain;

	Square* CreateSquare(int column, int row);
	Square* FindSquare(float xPos, float yPos);
};

