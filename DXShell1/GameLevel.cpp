#include "GameLevel.h"
#include "GameWorld.h"

Graphics* GameLevel::gfx;
AssetFactory* GameLevel::_assetFactory;

void GameLevel::visit(GameObject * gameobject)
{
}

void GameLevel::visit(Actor * actor)
{
	actor->HandleInput();
}
