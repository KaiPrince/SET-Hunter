#pragma once

#include "Graphics.h"
#include "GameBoard.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "GameWorld.h"

/*
Class Name: GameLevel
Purpose: This class represents the base for each level in the game.
	Static shared objects are kept here.
*/
class GameLevel
{
protected:
	static Graphics* gfx;
	static GameWorld* world;
	static GameObject* player; //TODO: delete in destructor
	static AssetFactory* _assetFactory;


	bool _returnToMainMenu; //Player ran out of lives
	std::chrono::duration<float, std::milli> _returnToMainMenuCountdown;

public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
		//_assetFactory = new AssetFactory(gfx);
		_assetFactory->Init(gfx);
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput() = 0;


	void ReturnToMainMenu_Update();
	void ReturnToMainMenu_Detect();
	void ReturnToMainMenu_Render();


	
};