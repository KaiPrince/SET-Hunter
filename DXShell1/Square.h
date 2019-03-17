#pragma once
#include "TerrainAsset.h"
#include "DrawableAsset.h"
#include "GameObject.h"


/*
Class Name: Square
Purpose: This class is used to represent a single square of the game board.
	It will manage its position and assets, and draw itself to the screen.
	It also manages its collidability state.
*/
class Square : public GameObject
{
	DrawableAsset* assets;

	/*int gbX;
	int gbY;*/
public:
	Square(float x, float y, float width, float height, TerrainAsset* terrain, GameBoard* gb);
	//Square(int x, int y, float width, float height, TerrainAsset* terrain, GameBoard* gb);
	~Square();

	void SetTerrain(TerrainAsset* terrain);
	TerrainAsset* GetTerrain();

	void SetAssets(DrawableAsset * assets);
	DrawableAsset* GetAssets();

	//void SetRectBoundary(int x, int y, float width, float height);

	//int GetGbX();
	int GetGbY();

	void SetGbX(int x);
	void SetGbY(int y);

	void Draw() override;
};

