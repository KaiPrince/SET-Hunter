#pragma once
#include "TerrainAsset.h"
#include "GrassTerrain.h"
#include "RoadTerrain.h"
#include "SpriteSheetAsset.h"
#include "Graphics.h"

class AssetFactory
{
	Graphics* gfx;

	//TODO: create Spritesheet factory. make static
	static SpriteSheet* _carSprite; 
	static SpriteSheet* _treeSprite;
	static SpriteSheet* _tree2Sprite;
	static SpriteSheet* _shrubSprite;
	static SpriteSheet* _explosionSprite;
public:
	AssetFactory(Graphics* graphics);
	~AssetFactory();

	DrawableAsset* CreateDrawableAsset(DrawableAsset::AssetTypes assetType);

	static DrawableAsset* _emptySprite;
};

