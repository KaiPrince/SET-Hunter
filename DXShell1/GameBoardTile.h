#pragma once
#include "DrawableAsset.h"
#include "DrawableAsset.h"
#include "GameObject.h"


/*
Class Name: GameBoardTile
Purpose: This class is used to represent a single square of the game board.
	It will manage its position and assets, and draw itself to the screen.
	It also manages its collidability state.
*/
class GameBoardTile : public GameObject
{
	DrawableAsset* assets;

	int gbX;
	int gbY;
public:
	GameBoardTile(float x, float y, float width, float height, DrawableAsset* terrain, GameBoard* gb, int column = 0, int row = 0);
	//GameBoardTile(int x, int y, float width, float height, TerrainAsset* terrain, GameBoard* gb);
	~GameBoardTile();

	void SetTerrain(DrawableAsset* terrain);
	DrawableAsset* GetTerrain();

	void SetAssets(DrawableAsset * assets);
	DrawableAsset* GetAssets();

	//void SetRectBoundary(int x, int y, float width, float height);

	int GetGbX();
	int GetGbY();

	void SetGbX(int x);
	void SetGbY(int y);

	void Draw() override;
};

