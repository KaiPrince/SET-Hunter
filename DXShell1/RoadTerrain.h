#pragma once
#include "DrawableAsset.h"
class RoadTerrain :
	public DrawableAsset
{
public:
	RoadTerrain();
	~RoadTerrain();

	void Draw(float x, float y, float width, float height) {
		GetGraphics()->FillRect(x, y, width, height, 0.0f, 0.0f, 0.0f, 1.0f);
	}
};

