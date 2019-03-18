#include "GameController.h"

GameLevel* GameController::currentLevel;
bool GameController::Loading;
GameLevel* GameController::QueuedNextLevel;
bool GameController::QueueExitGame;
unsigned int GameController::_score;
int GameController::_lives;

void GameController::Init()
{
	Loading = true;
	QueuedNextLevel = nullptr;
	QueueExitGame = false;

	currentLevel = 0;
	_score = 0;
	_lives = 3;
}

void GameController::LoadInitialLevel(GameLevel* lev)
{
	Loading = true; //This can help us avoid loading activity while rendering
	currentLevel = lev;
	currentLevel->Load();
	Loading = false;
}

void GameController::SwitchLevel(GameLevel* lev)
{
	Loading = true;
	currentLevel->Unload();
	lev->Load();
	delete currentLevel;
	currentLevel = lev;
	Loading = false;
}

void GameController::Render()
{
	if (Loading) return;//nice! Do not update or render if the scene is loading.
	currentLevel->Render();
}

void GameController::Update()
{
	if (Loading) return; //nice! Do not update or render if the scene is loading.
	currentLevel->Update();
}

void GameController::HandleInput()
{
	currentLevel->HandleInput();
}

unsigned int GameController::GetScore()
{
	return _score;
}

void GameController::SetScore(unsigned int score)
{
	_score = score;
}

int GameController::GetLives()
{
	return _lives;
}

void GameController::SetLives(int lives)
{
	_lives = lives;
}
