#pragma once
#include "TerrainAsset.h"
#include "SpriteSheet.h"
#include "DrawableAsset.h"

class Square
{
	TerrainAsset* terrain;
	DrawableAsset* assets;

	float x;
	float y;
	float width;
	float height;
public:
	Square();
	~Square();

	void SetTerrain(TerrainAsset* terrain);
	TerrainAsset* GetTerrain();

	void SetAssets(DrawableAsset * assets);
	DrawableAsset* GetAssets();

	void SetRectBoundary(float x, float y, float width, float height);

	float GetX() { return x; }
	float GetY() { return y; }
	float GetXPos() { return x * width; }
	float GetYPos() { return y * height; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }

	void Draw();
};

