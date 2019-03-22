#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "MainMenuLevel.h"


void Level1::Load()
{
	//Create World
	//world = new GameWorld(_assetFactory);

	continueRoadScolling = true;

	//{
	//	//Player starts at bottom middle of screen.
	//	GameBoard* gb = world->GetGameBoard();
	//	float player_StartX = (float)((gb->boardWidth * gb->squareWidth / 2) - gb->squareWidth);
	//	float player_StartY = ((float)gb->boardHeight * gb->squareHeight) - gb->squareHeight;

	//	GameObject* player = new GameObject(player_StartX, player_StartY,
	//		gb->squareWidth, gb->squareHeight, _assetFactory->GetAsset(DrawableAsset::CAR_SPRITE), gb);
	//	player->SetPhysicsComponent(new PlayerPhysicsComponent(player, world));
	//	player->SetInputComponent(new PlayerInputComponent(player));

	//	world->SetPlayer(player);
	//}

	player = world->GetPlayer();
	player->AddObserver(this);
}


void Level1::Unload()
{
	player->RemoveObserver(this);
	
	//Delete World
	delete world;
}


void Level1::Update()
{

	if (continueRoadScolling) {

		world->GetGameBoard()->ScrollBoard();
	}

	world->Update();

	if (GameController::GetLives() <= 0) {
		GameController::QueuedNextLevel = new MainMenuLevel();
	}
}

void Level1::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	world->Draw();

	//DEBUG
	//gfx->DrawRect((float)column * gb->squareWidth, (float)row * gb->squareHeight, (float)gb->squareWidth, (float)gb->squareHeight, 0.0f, 0.0f, 0.0f, 1.0f);

	/*char ScoreMessage[500] = "";
	gfx->WriteText(0, 0, 200.0f, 100.0f, 10.0f, ScoreMessage, sprintf_s(ScoreMessage, 500, "Score: %u\n Lives %d\n",
		GameController::GetScore(), GameController::GetLives()));*/


	//DEBUG
	int sX = 0;
	int sY = 0;
	Square* test = world->GetGameBoard()->FindSquare(player->GetXPos() + (player->GetWidth() / 2), player->GetYPos());
	if (test != nullptr) {
		sX = test->GetGbX();
		sY = test->GetGbY();
	}

	gfx->DrawRect(test->GetXPos(), test->GetYPos(), test->GetWidth(), test->GetHeight(), 1.0f, 0.0f, 0.0f, 1.0f);


	char msg[500] = "";
	gfx->WriteText(0, 0, 200.0f, 100.0f, 10.0f, msg, sprintf_s(msg, 500, "X Velocity: %2.2g\nY Velocity: %2.2f", player->GetXVelocity(), player->GetYVelocity()));
}

void Level1::HandleInput()
{
	world->HandleInput();
}

void Level1::Notify(Observable* subject) {

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