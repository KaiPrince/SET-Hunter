#pragma once
#include "DrawableAsset.h"

class TerrainAsset : public DrawableAsset
{
public:
	TerrainAsset() : DrawableAsset() {
	}
	virtual ~TerrainAsset();

	virtual void Draw(float x, float y, float width, float height) {}; //TODO: change to pure virtual?

};

