#pragma once
#include "TerrainAsset.h"
class RoadTerrain :
	public TerrainAsset
{
public:
	RoadTerrain();
	~RoadTerrain();

	void Draw(float x, float y, float width, float height) {
		GetGraphics()->FillRect(x, y, width, height, 0.0f, 0.0f, 0.0f, 1.0f);
	}
};

