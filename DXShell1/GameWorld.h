#pragma once
#include <vector>
//#include "GameObject.h"
//#include "GameBoard.h"

class GameObject;
class GameBoard;
class Actor;
class AssetFactory;

/*
Class Name: GameWorld
Purpose: This class aggregates all GameObjects in the world, plus the gameboard.
	This class will be used by the PhysicsComponent for the purposes of collision detection.
*/
class GameWorld
{
	std::vector<GameObject*> _gameObjects;
	GameBoard* _gameBoard;
	Actor* _player;
public:
	GameWorld(AssetFactory* assetFactory);
	~GameWorld();

	GameBoard* GetGameBoard() { return _gameBoard; }
	void SetGameBoard(GameBoard* gameBoard);

	Actor* GetPlayer() { return _player; }
	void SetPlayer(Actor* player);

	std::vector<GameObject*>& GetGameObjects() { return _gameObjects; }
	void AddGameObject(GameObject* obj);
	void RemoveGameObject(GameObject* obj);

	void Draw();
	void Update();

};

