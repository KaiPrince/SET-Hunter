#pragma once
#include <vector>
//#include "GameObject.h"
//#include "GameBoard.h"

class GameObject;
class GameBoard;

/*
Class Name: GameWorld
Purpose: This class aggregates all GameObjects in the world, plus the gameboard.
	This class will be used by the PhysicsComponent for the purposes of collision detection.
*/
class GameWorld
{
	std::vector<GameObject*> _gameObjects;
	GameBoard* _gameBoard;
public:
	GameWorld(GameBoard* gb);
	~GameWorld();

	std::vector<GameObject*> GetGameObjects() { return _gameObjects; }
	GameBoard* GetGameBoard() { return _gameBoard; }

	void AddGameObject(GameObject* obj);
	void RemoveGameObject(GameObject* obj);

};

