#include "AssetFactory.h"

SpriteSheet* AssetFactory::_carSprite;
SpriteSheet* AssetFactory::_treeSprite;
SpriteSheet* AssetFactory::_tree2Sprite;
SpriteSheet* AssetFactory::_shrubSprite;
SpriteSheet* AssetFactory::_explosionSprite;
DrawableAsset* AssetFactory::_emptySprite = new EmptyAsset(); //TODO: delete?

AssetFactory::AssetFactory(Graphics* graphics)
{
	gfx = graphics;
	DrawableAsset::SetGraphics(graphics);

	_carSprite = new SpriteSheet(L"Assets\\playercar.bmp", gfx);
	_treeSprite = new SpriteSheet(L"Assets\\tree1.bmp", gfx);
	_tree2Sprite = new SpriteSheet(L"Assets\\tree2.bmp", gfx);
	_shrubSprite = new SpriteSheet(L"Assets\\shrub1.bmp", gfx);
	_explosionSprite = new SpriteSheet(L"Assets\\explosion.bmp", gfx);
}

AssetFactory::~AssetFactory()
{
	//TODO: delete spritesheets
}

DrawableAsset* AssetFactory::CreateDrawableAsset(DrawableAsset::AssetTypes assetType)
{
	DrawableAsset* asset = nullptr;
	switch (assetType)
	{
	case DrawableAsset::GRASS_TERRAIN:
		asset = new GrassTerrain();
		break;
	case DrawableAsset::ROAD_TERRAIN:
		asset = new RoadTerrain();
		break;
	case DrawableAsset::CAR_SPRITE:
		asset = new SpriteSheetAsset(_carSprite, assetType);
		break;
	case DrawableAsset::TREE_SPRITE:
		asset = new SpriteSheetAsset(_treeSprite, assetType);
		break;
	case DrawableAsset::TREE2_SPRITE:
		asset = new SpriteSheetAsset(_tree2Sprite, assetType);
		break;
	case DrawableAsset::SHRUB_SPRITE:
		asset = new SpriteSheetAsset(_shrubSprite, assetType);
		break;
	case DrawableAsset::EXPLOSION_SPRITE:
		asset = new SpriteSheetAsset(_explosionSprite, assetType);
		break;
	case DrawableAsset::EMPTY_ASSET:
		asset = new EmptyAsset();
		break;
	case DrawableAsset::MAIN_MENU_BACKGROUND_ASSET:
		asset = new MainMenuBackgroundAsset();
		break;
	case DrawableAsset::TEXT_ASSET:
		asset = new TextAsset();
		break;
	default:
		//asset = new TerrainAsset();
		//TODO: throw error
		break;
	}
	return asset;
}
