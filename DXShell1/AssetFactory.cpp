#include "AssetFactory.h"

Graphics* AssetFactory::gfx;

SpriteSheet* AssetFactory::_carSprite;
SpriteSheet* AssetFactory::_treeSprite;
SpriteSheet* AssetFactory::_tree2Sprite;
SpriteSheet* AssetFactory::_shrubSprite;
SpriteSheet* AssetFactory::_explosionSprite;
SpriteSheet* AssetFactory::_spyHunterArt;

DrawableAsset* AssetFactory::_nullAsset = new NullAsset(); //TODO: delete?

void AssetFactory::Init(Graphics* graphics)
{
	gfx = graphics;
	DrawableAsset::SetGraphics(graphics);

	_carSprite = new SpriteSheet(L"Assets\\playercar.bmp", gfx);
	_treeSprite = new SpriteSheet(L"Assets\\tree1.bmp", gfx);
	_tree2Sprite = new SpriteSheet(L"Assets\\tree2.bmp", gfx);
	_shrubSprite = new SpriteSheet(L"Assets\\shrub1.bmp", gfx);
	_explosionSprite = new SpriteSheet(L"Assets\\explosion.bmp", gfx);
	_spyHunterArt = new SpriteSheet(L"Assets\\spy_hunter_artwork.bmp", gfx);
}

AssetFactory::~AssetFactory()
{
	if (_carSprite) delete _carSprite;
	if (_treeSprite) delete _treeSprite;
	if (_tree2Sprite) delete _tree2Sprite;
	if (_shrubSprite) delete _shrubSprite;
	if (_explosionSprite) delete _explosionSprite;
	if (_spyHunterArt) delete _spyHunterArt;
}

DrawableAsset* AssetFactory::GetAsset(DrawableAsset::AssetTypes assetType)
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
		asset = new SpriteSheetAsset(_carSprite, assetType); //TODO: make static (flyweight)
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
	case DrawableAsset::NULL_ASSET:
		asset = _nullAsset;
		break;
	case DrawableAsset::MAIN_MENU_BACKGROUND_ASSET:
		asset = new MainMenuBackgroundAsset();
		break;
	case DrawableAsset::TEXT_ASSET:
		asset = new TextAsset();
		break;
	case DrawableAsset::SPY_HUNTER_ART:
		asset = new SpriteSheetAsset(_spyHunterArt, assetType);
		break;
	default:
		asset = _nullAsset;
		break;
	}
	return asset;
}
