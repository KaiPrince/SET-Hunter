#pragma once
#include "TerrainAsset.h"
#include "SpriteSheet.h"
#include "DrawableAsset.h"

/*
Class Name: Square
Purpose: This class is used to represent a single square of the game board.
	It will manage its position and assets, and draw itself to the screen.
	It also manages its collidability state.
*/
class Square
{
	TerrainAsset* terrain;
	DrawableAsset* assets;

	int x;
	int y;
	float width;
	float height;

	bool isCollidable;
public:
	Square();
	Square(int x, int y, float width, float height);
	~Square();

	void SetTerrain(TerrainAsset* terrain);
	TerrainAsset* GetTerrain();

	void SetAssets(DrawableAsset * assets);
	DrawableAsset* GetAssets();

	void SetRectBoundary(int x, int y, float width, float height);

	int GetX() { return x; }
	int GetY() { return y; }
	float GetXPos() { return x * width; }
	float GetYPos() { return y * height; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	bool IsCollidable() { return isCollidable; }

	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }
	void SetCollidable(bool isCollidable) { this->isCollidable = isCollidable; }

	bool ContainsPoint(float x, float y);

	void Draw();
};

