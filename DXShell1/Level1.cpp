#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include "Level2.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "MainMenuLevel.h"
#include "Constants.h"


void Level1::Load()
{
	//Create World
	//world = new GameWorld(_assetFactory);

	this->continueRoadScolling = true;

	player = world->GetPlayer();
	player->AddObserver(this);

	scoreAsset = static_cast<TextAsset*>(_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET));
	DrawableAsset* backgroundDecoratedAsset = new AssetBackgroundRectangleDecorator(scoreAsset);

	this->scoreHUD = new GameObject(0, 0, 100.0f, 50.0f, backgroundDecoratedAsset, world);
	world->AddUIObject(this->scoreHUD);

}


void Level1::Unload()
{
	player->RemoveObserver(this);

	world->RemoveUIObject(this->scoreHUD);

	delete this->scoreHUD;
	
	//Delete World
	//delete world;
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
	else if (GameController::GetScore() >= kScoreToLevel2) {
		GameController::QueuedNextLevel = new Level2();
	}
	else {
		//No level change needed.
	}
}

void Level1::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	world->Draw();

	char ScoreMessage[500] = "";
	sprintf_s(ScoreMessage, 500, "Score: %u\n Lives %d\n",
		GameController::GetScore(), GameController::GetLives());

	this->scoreAsset->SetText(ScoreMessage);


	//DEBUG
	/*int sX = 0;
	int sY = 0;
	GameBoardTile* test = world->GetGameBoard()->FindSquare(player->GetXPos() + (player->GetWidth() / 2), player->GetYPos());
	if (test != nullptr) {
		sX = test->GetGbX();
		sY = test->GetGbY();
	}

	gfx->DrawRect(test->GetXPos(), test->GetYPos(), test->GetWidth(), test->GetHeight(), 1.0f, 0.0f, 0.0f, 1.0f);


	char msg[500] = "";
	gfx->WriteText(0, 0, 200.0f, 100.0f, 10.0f, msg, sprintf_s(msg, 500, "X Velocity: %2.2g\nY Velocity: %2.2f", player->GetXVelocity(), player->GetYVelocity()));
	*/
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