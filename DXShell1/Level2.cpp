#include "Level2.h"
#include "GameController.h"
#include "Graphics.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "MainMenuLevel.h"
#include "Audio.h"
#include "GameObjectState.h"
#include "CollisionResolutionStrategy.h"
#include <algorithm>
#include "Constants.h"


void Level2::Load()
{
	//Change Theme
	AudioLocator::GetAudio()->playSound(Audio::LEVEL2_THEME);

	player = world->GetPlayer();
	player->AddObserver(this);

	InitScoreHUD();

	//Spawn 5 enemies.
	for (size_t i = 0; i < 1; i++)
	{

		GameObject* enemy = new GameObject(400, 500, 50, 100, _assetFactory->GetAsset(DrawableAsset::ENEMY_CAR), world);
		enemy->SetInputComponent(new StayOnRoadInputComponent(enemy));
		enemy->SetPhysicsComponent(new CollidablePhysicsComponent(enemy, world));
		enemy->UpdateState(new ShootPlayerState(enemy));
		enemy->GetPhysicsComponent()->SetCollisionStrategy(new DeathTouchCollisionStrategy(enemy));

		enemy->AddObserver(this);

		world->AddGameObject(enemy);
		_enemies.push_back(enemy);
	}
}

void Level2::Unload()
{
	player->RemoveObserver(this);

	DeleteScoreHUD();
}

void Level2::Update()
{
	ProcessAllCommands();

	if (continueRoadScolling) {

		world->GetGameBoard()->ScrollBoard();
	}

	world->Update();

	ReturnToMainMenu_Detect();

	ReturnToMainMenu_Update();
}



void Level2::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	world->Draw();

	RefreshScoreHUDText();

	ReturnToMainMenu_Render();
}


void Level2::HandleInput()
{
	world->HandleInput();
}

void Level2::Notify(Observable * subject)
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

	//Check if subject is an enemy.
	if (std::find(this->_enemies.begin(), this->_enemies.end(), subject) != this->_enemies.end()) {

		GameObject* enemy = static_cast<GameObject*>(subject);
		switch (enemy->GetState()->GetType())
		{
		case GameObjectState::SHOOT_PLAYER_STATE:
		{ //Brackets added to prevent Error C2361
			//shoot

			QueueCommand(new SpawnRocket(enemy, world, _assetFactory));
		}

		break;
		default:
			break;
		}
	}
}
