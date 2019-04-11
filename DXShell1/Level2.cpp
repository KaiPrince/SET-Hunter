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


Level2::Level2()
{
	this->continueRoadScolling = true;
	this->_returnToMainMenu = false;
	this->_returnToMainMenuCountdown = std::chrono::duration<float, std::milli>(0.0f);
}


Level2::~Level2()
{
}

void Level2::Load()
{
	//Change Theme
	AudioLocator::GetAudio()->playSound(Audio::LEVEL2_THEME);

	player = world->GetPlayer();
	player->AddObserver(this);

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
}

void Level2::Update()
{
	ProcessAllCommands();

	if (continueRoadScolling) {

		world->GetGameBoard()->ScrollBoard();
	}

	world->Update();

	if (GameController::GetLives() <= 0 && _returnToMainMenu == false) {
		//TODO: Add edgar ramone "Game Over"

		_returnToMainMenu = true;

		using namespace std::chrono;
		_returnToMainMenuCountdown = duration<float, std::milli>(kReturnToMainMenuCountdownTime);

	}

	if (_returnToMainMenu) {

		if (_returnToMainMenuCountdown.count() > 0) {

			//Decrement countdown
			using namespace std::chrono;
			float elapsedTimeInMS = GameController::GetDeltaTime();
			_returnToMainMenuCountdown -= duration<float, std::milli>(elapsedTimeInMS);

		}
		else {
			//Transition to main menu.
			GameController::QueuedNextLevel = new MainMenuLevel();
		}
	}
}


void Level2::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	world->Draw();

	char ScoreMessage[500] = "";
	gfx->WriteText(0, 0, 200.0f, 100.0f, 10.0f, ScoreMessage, sprintf_s(ScoreMessage, 500, "Score: %u\n Lives %d\n NumObjects: %zd\n",
		GameController::GetScore(), GameController::GetLives(), world->GetGameObjects().size()));

	if (_returnToMainMenuCountdown.count() > 0) {


		const float screenH = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Height);
		const float screenW = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Width);

		float opacity = (1.0f - 0.0f) * ((kReturnToMainMenuCountdownTime - _returnToMainMenuCountdown.count()) / kReturnToMainMenuCountdownTime);

		//Draw white filter
		GraphicsLocator::GetGraphics()->FillRect(0.0f, 0.0f, screenW, screenH, 1.0f, 1.0f, 1.0f, opacity);

	}
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
