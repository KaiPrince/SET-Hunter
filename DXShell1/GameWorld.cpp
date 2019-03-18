#include "GameWorld.h"
#include "GameBoard.h"
#include <vector>
#include "GameObject.h"
#include <algorithm>



GameWorld::GameWorld(GameBoard* gb)
{
}


GameWorld::~GameWorld()
{
}

void GameWorld::AddGameObject(GameObject * obj)
{
	if (obj != nullptr) {
		_gameObjects.push_back(obj);
	}
}

void GameWorld::RemoveGameObject(GameObject * obj)
{
	if (obj != nullptr) {
		_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), obj));
	}
}
