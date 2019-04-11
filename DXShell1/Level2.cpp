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

	//Reset countdown
	using namespace std::chrono;
	_spawnEnemyCountdown = duration<float, std::milli>(kLevel2SpawnEnemyCarsDelay);
}

void Level2::Unload()
{
	player->RemoveObserver(this);

	DeleteScoreHUD();
}

void Level2::Update()
{
	ProcessAllCommands();


	//Get delta time
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	float elapsedTimeInMS = GameController::GetDeltaTime();

	//Decrement invincibility timer
	if (_spawnEnemyCountdown.count() > 0) {

		_spawnEnemyCountdown -= duration<float, std::milli>(elapsedTimeInMS);
	}

	//Spawn enemies.
	if (_spawnEnemyCountdown.count() <= 0) {

		float randomXPos = 400.0f; //TODO
		const float screenHeight = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Height);
		GameObject* enemy = new GameObject(randomXPos, screenHeight - 1, kEnemyCarWidth, kEnemyCarHeight, _assetFactory->GetAsset(DrawableAsset::ENEMY_CAR), world, 0.0f, kEnemyCarYVelocity);
		enemy->SetInputComponent(new StayOnRoadInputComponent(enemy));
		enemy->SetPhysicsComponent(new CollidablePhysicsComponent(enemy, world));
		enemy->UpdateState(new ShootPlayerState(enemy));
		enemy->GetPhysicsComponent()->SetCollisionStrategy(new DeathTouchCollisionStrategy(enemy));

		enemy->AddObserver(this);

		world->AddGameObject(enemy);
		_enemies.push_back(enemy);

		//Reset countdown
		_spawnEnemyCountdown = duration<float, std::milli>(kLevel2SpawnEnemyCarsDelay);

	}

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
