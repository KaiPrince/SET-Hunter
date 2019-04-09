#include "Level2.h"
#include "GameController.h"
#include "Graphics.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "MainMenuLevel.h"



Level2::Level2()
{
	this->continueRoadScolling = true;
}


Level2::~Level2()
{
}

void Level2::Load()
{
	player = world->GetPlayer();
	player->AddObserver(this);

	//Spawn 5 enemies.
	GameObject* enemy = new GameObject(400, 100, 50, 100, _assetFactory->GetAsset(DrawableAsset::ENEMY_CAR), world->GetGameBoard());
	enemy->SetInputComponent(new StayOnRoadInputComponent(enemy));
	enemy->SetPhysicsComponent(new CollidablePhysicsComponent(enemy, world));
	//enemy->GetPhysicsComponent()->SetCollisionStrategy(new )

	world->AddGameObject(enemy);
}

void Level2::Unload()
{
	player->RemoveObserver(this);
}

void Level2::Update()
{
	if (continueRoadScolling) {

		world->GetGameBoard()->ScrollBoard();
	}

	world->Update();

	if (GameController::GetLives() <= 0) {
		GameController::QueuedNextLevel = new MainMenuLevel();
	}
}

void Level2::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	world->Draw();

	char ScoreMessage[500] = "";
	gfx->WriteText(0, 0, 200.0f, 100.0f, 10.0f, ScoreMessage, sprintf_s(ScoreMessage, 500, "Score: %u\n Lives %d\n",
		GameController::GetScore(), GameController::GetLives()));
}

void Level2::HandleInput()
{
	world->HandleInput();
}

void Level2::Notify(Observable* subject)
{
	if (subject == player) {

		switch (player->GetState()->GetType())
		{
		case GameObjectState::ALIVE_STATE:
			continueRoadScolling = true;
			break;
		case GameObjectState::DEAD_STATE:
			continueRoadScolling = false;
			break;
		case GameObjectState::INVINCIBLE_STATE:
			continueRoadScolling = true;
			break;
		default:
			break;
		}
	}
}
