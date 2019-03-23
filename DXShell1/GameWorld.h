#pragma once
#include <vector>
//#include "GameObject.h"
//#include "GameBoard.h"
#include "Visitor.h"

class GameObject;
class GameBoard;
class GameObject;
class AssetFactory;

/*
Class Name: GameWorld
Purpose: This class aggregates all GameObjects in the world, plus the gameboard.
	When the GameLoop runs, this class will call process input, update, and draw on all its objects.
	This class will be used by the PhysicsComponent for the purposes of collision detection.
*/
class GameWorld
{
	std::vector<GameObject*> _gameObjects;
	std::vector<GameObject*> _uiObjects;
	GameBoard* _gameBoard;
	GameObject* _player;
public:
	GameWorld(AssetFactory* assetFactory);
	~GameWorld();

	GameBoard* GetGameBoard() { return _gameBoard; }
	void SetGameBoard(GameBoard* gameBoard);

	GameObject* GetPlayer() { return _player; }
	void SetPlayer(GameObject* player);

	std::vector<GameObject*>& GetGameObjects() { return _gameObjects; }
	void AddGameObject(GameObject* obj);
	void RemoveGameObject(GameObject* obj);

	void AddUIObject(GameObject * obj);
	void RemoveUIObject(GameObject * obj);

	void HandleInput();
	void Update();
	void Draw();

};

