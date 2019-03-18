#pragma once
#include "GameLevel.h"


/*
Class Name: GameController
Purpose: This class the man pulling the strings behind the game.
	This singleton class is responsible for starting the game, loading the levels, executing the game loop,
	and ending the game.
*/
class GameController
{
	GameController() {}
	static GameLevel* currentLevel;
	static unsigned int _score;
	static int _lives;

public:
	static bool Loading;
	static GameLevel* QueuedNextLevel;
	static bool QueueExitGame;

	static void Init();
	static void LoadInitialLevel(GameLevel* lev);
	static void SwitchLevel(GameLevel* lev);
	static void Render();
	static void Update();
	static void HandleInput();

	static unsigned int GetScore();
	static void SetScore(unsigned int score);

	static int GetLives();
	static void SetLives(int lives);
};