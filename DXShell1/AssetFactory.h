#pragma once
#include "DrawableAsset.h"
#include "Graphics.h"

/*
Class Name: AssetFactory
Purpose: This class is used to implement the flyweight pattern.
	Static singleton assets will be managed by this class.
*/
class AssetFactory
{
	static Graphics* gfx;

	static SpriteSheet* _carSprite; 
	static SpriteSheet* _treeSprite;
	static SpriteSheet* _tree2Sprite;
	static SpriteSheet* _shrubSprite;
	static SpriteSheet* _explosionSprite;
	static SpriteSheet* _splashScreenArt;
	static SpriteSheet* _enemyCar;
	static SpriteSheet* _rocket;
	static SpriteSheet* _road;
	static SpriteSheet* _grass;
	static SpriteSheet* _coin;
	static SpriteSheet* _friendlyCar;


	static DrawableAsset* _nullAsset;

	AssetFactory();
public:
	~AssetFactory();

	static void Init(Graphics* graphics);

	static DrawableAsset* GetAsset(DrawableAsset::AssetTypes assetType);

	static DrawableAsset* GetNullAsset() { return GetAsset(DrawableAsset::NULL_ASSET); }

};

