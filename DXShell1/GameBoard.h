#pragma once
#include "TerrainAsset.h"
#include "SquareFactory.h"
#include "AssetFactory.h"
#include <ctime>
#include <chrono>
#include <vector>

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

	std::vector<Square*> _obstacles; //Plants
	void placePlants();
	void placePlants(int row);

	void AddObstacle(Square* obstacle);
	void RemoveObstacle(Square* obstacle);
public:
	GameBoard(float squareWidth, float squareHeight, AssetFactory* assetFactory);
	~GameBoard();
	void Init();

	static const int boardWidth = 10; //TODO: make these global const.
	static const int boardHeight = 10;
	static const int roadWidth = 4;

	//TODO: make this private, getters only.
	float squareWidth;
	float squareHeight;
	Square* squares[boardWidth][boardHeight];

	Square** GetSquares() { return *squares; }
	Square* GetSquare(int column, int row) { return squares[column][row]; }
	AssetFactory* GetAssetFactory() { return _assetFactory; }

	void SetAssetFactory(AssetFactory* assetFactory);
	TerrainAsset* _grassTerrain;
	TerrainAsset* _roadTerrain;

	Square* CreateSquare(int column, int row);
	Square* FindSquare(float xPos, float yPos);

	void ScrollBoard();

	void Draw();
	Square* FindLeftRoadSquare(int row);
};

