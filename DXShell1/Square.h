#pragma once
#include "TerrainAsset.h"
#include "SpriteSheet.h"
#include "DrawableAsset.h"

class Square
{
	TerrainAsset* terrain;
	DrawableAsset* assets;

	int x;
	int y;
	float width;
	float height;
public:
	Square();
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

	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }

	bool ContainsPoint(float x, float y);

	void Draw();
};

