#include "SpriteSheetAsset.h"



SpriteSheetAsset::SpriteSheetAsset(SpriteSheet* sprite, DrawableAsset::AssetTypes assetType) : DrawableAsset()
{
	this->sprite = sprite;
	this->type = assetType;
}


SpriteSheetAsset::~SpriteSheetAsset()
{
}

void SpriteSheetAsset::Draw(float x, float y, float width, float height)
{
	sprite->Draw(x, y, width, height);
}
