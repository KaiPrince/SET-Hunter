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
#include <random>
#include <chrono>



void Level2::Load()
{
	//Change Theme
	AudioLocator::GetAudio()->playSound(Audio::LEVEL2_THEME);

	player = world->GetPlayer();
	player->AddObserver(this);

	InitScoreHUD();

	//Reset countdown
	using namespace std::chrono;
	_spawnActorsCountdown = duration<float, std::milli>(kLevel2_SpawnActorsDelay);
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
	if (_spawnActorsCountdown.count() > 0) {

		_spawnActorsCountdown -= duration<float, std::milli>(elapsedTimeInMS);
	}

	//Spawn enemies.
	if (_spawnActorsCountdown.count() <= 0) {

		unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> dice_carType(1, kLevel2_UnliklihoodOfEnemyCar);

		float randomXPos = 400.0f; //TODO
		const float screenHeight = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Height);

		GameObject* newActor;
		if (dice_carType(generator) == 1) {
			//Enemy Car

		GameObject* enemy = new GameObject(randomXPos, screenHeight - 1, kEnemyCarWidth, kEnemyCarHeight, _assetFactory->GetAsset(DrawableAsset::ENEMY_CAR), world, 0.0f, kEnemyCarYVelocity);
		enemy->SetInputComponent(new StayOnRoadInputComponent(enemy));
		enemy->SetPhysicsComponent(new CollidablePhysicsComponent(enemy, world));
		enemy->UpdateState(new ShootPlayerState(enemy));
		enemy->GetPhysicsComponent()->SetCollisionStrategy(new DeathTouchCollisionStrategy(enemy));

		newActor = enemy;

		}
		else {
			//Civilian Car
			GameObject* friendly = new GameObject(randomXPos, screenHeight - 1, kFriendlyCarWidth, kFriendlyCarHeight, _assetFactory->GetAsset(DrawableAsset::FRIENDLY_CAR), world, 0.0f, kFriendlyCarYVelocity);
			friendly->SetInputComponent(new StayOnRoadInputComponent(friendly));
			friendly->SetPhysicsComponent(new CollidablePhysicsComponent(friendly, world));
			friendly->GetPhysicsComponent()->SetCollisionStrategy(new NullCollisionStrategy()); //TODO: maybe replace create an excludableCollisionStrategy?

			newActor = friendly;


		}

		
		newActor->AddObserver(this);

		world->AddGameObject(newActor);
		_actors.push_back(newActor);

		//Reset countdown
		_spawnActorsCountdown = duration<float, std::milli>(kLevel2_SpawnActorsDelay);

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
	if (std::find(this->_actors.begin(), this->_actors.end(), subject) != this->_actors.end()) {

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
