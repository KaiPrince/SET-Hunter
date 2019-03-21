#include "GameWorld.h"
#include "GameBoard.h"
#include <vector>
#include "GameObject.h"
#include "GraphicsLocator.h"
#include <algorithm>
#include "Actor.h"
#include "AssetFactory.h"



GameWorld::GameWorld(AssetFactory* assetFactory)
{
	float squareWidth = (float)GraphicsLocator::GetGraphics()->Window_Width / GameBoard::boardWidth;
	float squareHeight = (float)GraphicsLocator::GetGraphics()->Window_Height / GameBoard::boardHeight;
	GameBoard* gb = new GameBoard(squareWidth, squareHeight, assetFactory, this);
	gb->Init();




	_gameBoard = gb;
	_player = new NullActor();

	_gameObjects.push_back(_player);
}


GameWorld::~GameWorld()
{
	for each (GameObject* gameObject in _gameObjects)
	{
		delete gameObject;
	}

	delete _gameBoard;
}

void GameWorld::SetGameBoard(GameBoard * gameBoard)
{
	if (gameBoard != nullptr) {
		delete _gameBoard;

		_gameBoard = gameBoard;
	}
}

void GameWorld::SetPlayer(Actor * player)
{

	if (player != nullptr) {
		RemoveGameObject(_player);
		delete _player;

		AddGameObject(player);
		_player = player;
	}

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
		_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), obj), _gameObjects.end());
	}
}

void GameWorld::Draw() {
	_gameBoard->Draw();

	for (GameObject* gameObject : _gameObjects) {
		gameObject->Draw();
	}
}

void GameWorld::HandleInput()
{
	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->accept(*this);
	}
}

void GameWorld::visit(GameObject * gameobject)
{
}

void GameWorld::visit(Actor * actor)
{
	actor->HandleInput();
}

void GameWorld::Update()
{

	for (GameObject* gameObject : _gameObjects) {
		gameObject->Update();
	}
}