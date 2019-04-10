#include "GameWorld.h"
#include "GameBoard.h"
#include <vector>
#include "GameObject.h"
#include "Graphics.h"
#include <algorithm>
#include "GameObject.h"
#include "AssetFactory.h"
#include "CollisionResolutionStrategy.h"
#include "PhysicsComponent.h"


GameWorld::GameWorld(AssetFactory* assetFactory)
{

	float squareWidth = (float)GraphicsLocator::GetGraphics()->Window_Width / GameBoard::boardWidth;
	float squareHeight = (float)GraphicsLocator::GetGraphics()->Window_Height / GameBoard::boardHeight;

	GameBoard* gb = new GameBoard(squareWidth, squareHeight, assetFactory, this);
	gb->Init();




	_gameBoard = gb;
	_player = new NullGameObject();

	_gameObjects.push_back(_player);
}


GameWorld::~GameWorld()
{
	for each (GameObject * gameObject in _gameObjects)
	{
		delete gameObject;
	}

	delete _gameBoard;
}

void GameWorld::SetGameBoard(GameBoard* gameBoard)
{
	if (gameBoard != nullptr) {
		delete _gameBoard;

		_gameBoard = gameBoard;
	}
}

void GameWorld::SetPlayer(GameObject* player)
{

	if (player != nullptr) {
		RemoveGameObject(_player);
		delete _player;

		AddGameObject(player);
		_player = player;
	}

}

void GameWorld::AddGameObject(GameObject* obj)
{
	if (obj != nullptr) {
		_gameObjects.push_back(obj);
	}
}

void GameWorld::RemoveGameObject(GameObject* obj)
{
	if (obj != nullptr) {
		_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), obj), _gameObjects.end());
	}
}

void GameWorld::AddUIObject(GameObject* obj)
{
	if (obj != nullptr) {
		_uiObjects.push_back(obj);
	}
}

void GameWorld::RemoveUIObject(GameObject* obj)
{
	if (obj != nullptr) {
		_uiObjects.erase(std::remove(_uiObjects.begin(), _uiObjects.end(), obj), _uiObjects.end());
	}
}

void GameWorld::Draw() {
	_gameBoard->Draw();

	for (GameObject* gameObject : _gameObjects) {
		if (gameObject != _player) {
			gameObject->Draw();
		}
	}
	_player->Draw(); //Draw player last.

	for (GameObject* uiObject : _uiObjects) {
		uiObject->Draw();
	}
}

void GameWorld::HandleInput()
{
	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->HandleInput();
	}

	for (GameObject* uiObject : _uiObjects)
	{
		uiObject->HandleInput();
	}
}

void GameWorld::Update()
{
	ProcessAllCommands();

	for (GameObject* gameObject : _gameObjects) {
		gameObject->Update();


		if (gameObject->GetPhysicsComponent()->IsCollidable() && gameObject->GetPhysicsComponent()->IsCollisionDetected())
		{
			for (GameObject* collidedGameObject : gameObject->GetPhysicsComponent()->GetCollisionObjects())
			{
				gameObject->GetPhysicsComponent()->GetCollisionStrategy()->CollideWith(collidedGameObject);
			}
		}
	}
}
