#pragma once

#include "Graphics.h"
#include "GameBoard.h"
#include "Actor.h"
#include "InputComponent.h"
#include "GameWorld.h"

class GameLevel
{
protected:
	static Graphics* gfx;
	GameWorld* world;
	Actor* player; //TODO: delete in destructor
	static AssetFactory* _assetFactory;

public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
		_assetFactory = new AssetFactory(gfx);
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput() = 0;


	
};