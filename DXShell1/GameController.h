#pragma once

#include "GameLevel.h"
#include "Graphics.h"
#include "GameBoard.h"
#include "AssetFactory.h"

//This will be a Singleton class (constructor is private)

class GameController
{
	GameController() {}
	static GameLevel* currentLevel;
	static AssetFactory* _terrainFactory;
public:
	static bool Loading;
	static void Init();
	static void LoadInitialLevel(GameLevel* lev);
	static void SwitchLevel(GameLevel* lev);
	static void Render();
	static void Update();

};