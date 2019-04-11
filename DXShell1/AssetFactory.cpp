#include "AssetFactory.h"

Graphics* AssetFactory::gfx;

SpriteSheet* AssetFactory::_carSprite;
SpriteSheet* AssetFactory::_treeSprite;
SpriteSheet* AssetFactory::_tree2Sprite;
SpriteSheet* AssetFactory::_shrubSprite;
SpriteSheet* AssetFactory::_explosionSprite;
SpriteSheet* AssetFactory::_splashScreenArt;
SpriteSheet* AssetFactory::_enemyCar;
SpriteSheet* AssetFactory::_rocket;
SpriteSheet* AssetFactory::_road;
SpriteSheet* AssetFactory::_grass;
SpriteSheet* AssetFactory::_coin;
SpriteSheet* AssetFactory::_friendlyCar;

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
	_splashScreenArt = new SpriteSheet(L"Assets\\splashScreen.bmp", gfx);
	_enemyCar = new SpriteSheet(L"Assets\\enemycar.bmp", gfx);
	_rocket = new SpriteSheet(L"Assets\\rocket.bmp", gfx);
	_road = new SpriteSheet(L"Assets\\road.bmp", gfx);
	_grass = new SpriteSheet(L"Assets\\grass.bmp", gfx);
	_coin = new SpriteSheet(L"Assets\\coin.bmp", gfx);
	_friendlyCar = new SpriteSheet(L"Assets\\friendlycar.bmp", gfx);
}

AssetFactory::~AssetFactory()
{
	if (_carSprite) delete _carSprite;
	if (_treeSprite) delete _treeSprite;
	if (_tree2Sprite) delete _tree2Sprite;
	if (_shrubSprite) delete _shrubSprite;
	if (_explosionSprite) delete _explosionSprite;
	if (_splashScreenArt) delete _splashScreenArt;
	if (_enemyCar) delete _enemyCar;
	if (_rocket) delete _rocket;
	if (_road) delete _road;
	if (_grass) delete _grass;
	if (_coin) delete _coin;
	if (_friendlyCar) delete _friendlyCar;
}

DrawableAsset* AssetFactory::GetAsset(DrawableAsset::AssetTypes assetType)
{
	DrawableAsset* asset = nullptr;
	switch (assetType)
	{
	case DrawableAsset::GRASS_TERRAIN:
		asset = new SpriteSheetAsset(_grass, assetType);
		break;
	case DrawableAsset::ROAD_TERRAIN:
		asset = new SpriteSheetAsset(_road, assetType);
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
	case DrawableAsset::SPLASH_SCREEN_ART:
	{ //Scope brackets added to prevent error C2361
		SpriteSheetAsset* splashScreenAsset = new SpriteSheetAsset(_splashScreenArt, assetType);
		splashScreenAsset->SetUseChromaKey(false);

		asset = splashScreenAsset;
		break;
	}
	case DrawableAsset::ENEMY_CAR:
		asset = new SpriteSheetAsset(_enemyCar, assetType);
		break;
	case DrawableAsset::ROCKET_SPRITE:
		asset = new SpriteSheetAsset(_rocket, assetType);
		break;
	case DrawableAsset::COIN_SPRITE:
		asset = new SpriteSheetAsset(_coin, assetType);
		break;
	case DrawableAsset::FRIENDLY_CAR:
		asset = new SpriteSheetAsset(_friendlyCar, assetType);
		break;
	default:
		throw; //Unknown/Invalid enum! (Make sure to add all enums to this switch statement.)
		break;
	}
	return asset;
}
