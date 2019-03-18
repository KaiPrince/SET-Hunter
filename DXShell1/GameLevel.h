#pragma once

#include "Graphics.h"
#include "GameBoard.h"
#include "Actor.h"
#include "InputComponent.h"
#include "GameWorld.h"
#include "Visitor.h"

class GameLevel : public Visitor
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
	virtual void HandleInput() {

		for (GameObject* gameObject : world->GetGameObjects())
		{
			gameObject->accept(*this);
		}
		
	}


	// Inherited via Visitor
	virtual void visit(GameObject * gameobject) override;
	virtual void visit(Actor * actor) override;
};