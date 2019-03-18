#include "GameLevel.h"
#include "GameWorld.h"

Graphics* GameLevel::gfx;
GameBoard* GameLevel::gb;
AssetFactory* GameLevel::_assetFactory;
Actor* GameLevel::player;
GameWorld* GameLevel::world;

void GameLevel::visit(GameObject * gameobject)
{
}

void GameLevel::visit(Actor * actor)
{
	actor->HandleInput();
}
