#pragma once
#include "DrawableAsset.h"

class GrassTerrain :
	public DrawableAsset
{
public:
	GrassTerrain();
	~GrassTerrain();

	void Draw(float x, float y, float width, float height) {
		GetGraphics()->FillRect(x, y, width, height, 76.0f / 255.0f, 169.0f / 255.0f, 70.0f / 255.0f, 1.0f);
	}
};

