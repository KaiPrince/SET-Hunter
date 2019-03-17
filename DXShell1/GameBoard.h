#pragma once
#include "TerrainAsset.h"
#include "SquareFactory.h"
#include "AssetFactory.h"
#include <ctime>
#include <chrono>

/*
Class Name: GameBoard
Purpose: This class is used to manage the Squares of the level
*/
class GameBoard
{

	SquareFactory* _squareFactory;
	AssetFactory* _assetFactory;

	//std::chrono::time_point<std::chrono::steady_clock> roadTimer;
	std::clock_t roadTimer;
	int roadShift; //Shifts the road to the right
	float verticalOffset;

	void placePlants();
	void placePlants(int row);
public:
	GameBoard();
	~GameBoard();
	void Init();

	static const int boardWidth = 10;
	static const int boardHeight = 10;
	static const int roadWidth = 4;

	//TODO: make this private, getters only.
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

	void ScrollBoard();

	void Draw();
};

