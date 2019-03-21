#pragma once
#include "DrawableAsset.h"
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
	GameWorld* _world;

	//std::chrono::time_point<std::chrono::steady_clock> roadTimer;
	std::clock_t roadTimer;
	int roadShift; //Shifts the road to the right
	float verticalOffset;

	std::vector<Square*> _plantObstacles; //Plants
	void placePlants();
	void placePlants(int row);

	void AddObstacle(Square* obstacle);
	void RemoveObstacle(Square* obstacle);

	void DrawRoadMask();

	
			
public:
	GameBoard(float squareWidth, float squareHeight, AssetFactory* assetFactory, GameWorld* world);
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
	std::vector<Square*>& GetObstacles() { return _plantObstacles; }
	Square* GetSquare(int column, int row) { return squares[column][row]; }
	AssetFactory* GetAssetFactory() { return _assetFactory; }

	void SetAssetFactory(AssetFactory* assetFactory);
	DrawableAsset* _grassTerrain;
	DrawableAsset* _roadTerrain;

	Square* CreateSquare(int column, int row);
	Square* FindSquare(float xPos, float yPos);

	void ScrollBoard();

	void Draw();
	Square* FindLeftRoadSquare(int row);
	Square * FindRightRoadSquare(int row);
	void DrawLeftRoadMask(Square * leftRoadSquare, Square * leftRoadSquareAbove);
	void DrawRightRoadMask(Square * rightRoadSquare, Square * rightRoadSquareAbove);
};

