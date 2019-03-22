#pragma once
#include "DrawableAsset.h"
#include "TileFactory.h"
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

	TileFactory* _squareFactory;
	AssetFactory* _assetFactory;
	GameWorld* _world;

	std::chrono::time_point<std::chrono::steady_clock> roadTimer;
	//std::clock_t roadTimer;
	int roadShift; //Shifts the road to the right
	float verticalOffset;

	std::vector<GameBoardTile*> _plantObstacles; //Plants
	void placePlants();
	void placePlants(int row);

	void AddObstacle(GameBoardTile* obstacle);
	void RemoveObstacle(GameBoardTile* obstacle);

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
	GameBoardTile* squares[boardWidth][boardHeight];

	GameBoardTile** GetSquares() { return *squares; }
	std::vector<GameBoardTile*>& GetObstacles() { return _plantObstacles; }
	GameBoardTile* GetSquare(int column, int row) { return squares[column][row]; }
	AssetFactory* GetAssetFactory() { return _assetFactory; }

	void SetAssetFactory(AssetFactory* assetFactory);
	DrawableAsset* _grassTerrain;
	DrawableAsset* _roadTerrain;

	GameBoardTile* CreateSquare(int column, int row);
	GameBoardTile* FindSquare(float xPos, float yPos);

	void ScrollBoard();

	void Draw();
	GameBoardTile* FindLeftRoadSquare(int row);
	GameBoardTile * FindRightRoadSquare(int row);
	void DrawLeftRoadMask(GameBoardTile * leftRoadSquare, GameBoardTile * leftRoadSquareAbove);
	void DrawRightRoadMask(GameBoardTile * rightRoadSquare, GameBoardTile * rightRoadSquareAbove);
};

