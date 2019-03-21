#pragma once
#include <vector>
//#include "GameObject.h"
//#include "GameBoard.h"
#include "Visitor.h"

class GameObject;
class GameBoard;
class Actor;
class AssetFactory;

/*
Class Name: GameWorld
Purpose: This class aggregates all GameObjects in the world, plus the gameboard.
	This class will be used by the PhysicsComponent for the purposes of collision detection.
*/
class GameWorld : public Visitor
{
	std::vector<GameObject*> _gameObjects;
	std::vector<GameObject*> _uiObjects;
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

	void AddUIObject(GameObject * obj);
	void RemoveUIObject(GameObject * obj);

	void HandleInput();
	void Update();
	void Draw();

	// Inherited via Visitor
	virtual void visit(GameObject * gameobject) override;
	virtual void visit(Actor * actor) override;
};

