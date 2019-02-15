#pragma once
#include "DrawableAsset.h"
#include "SpriteSheet.h"

class SpriteSheetAsset :
	public DrawableAsset
{
	SpriteSheet* sprite;
public:
	SpriteSheetAsset(SpriteSheet* sprite, DrawableAsset::AssetTypes assetType);
	~SpriteSheetAsset();

	virtual void Draw(float x, float y, float width, float height);
};

