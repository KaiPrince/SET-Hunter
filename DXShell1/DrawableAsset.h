#pragma once
#include <vector>
#include "Graphics.h"
class DrawableAsset
{
	static Graphics* gfx;
protected:
	Graphics* GetGraphics() {
		return gfx;
	}

public:
	DrawableAsset();
	virtual ~DrawableAsset();

	static void SetGraphics(Graphics* graphics) {
		gfx = graphics;
	}


	enum AssetTypes {
		GRASS_TERRAIN, ROAD_TERRAIN, CAR_SPRITE, TREE_SPRITE, TREE2_SPRITE, SHRUB_SPRITE
	};
protected: //This is only here because the AssetTypes var must be below its definition.
	DrawableAsset::AssetTypes type;

public:
	DrawableAsset::AssetTypes GetType() { return type; }

	virtual void Draw(float x, float y, float width, float height) = 0;
};

