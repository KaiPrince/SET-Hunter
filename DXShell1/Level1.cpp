#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "SpriteSheetAsset.h"


void Level1::Load()
{
	continueRoadScolling = true;
	player->AddObserver(this);
}


void Level1::Unload()
{
}


void Level1::Update()
{

	player->Update();

	if (continueRoadScolling) {

		gb->ScrollBoard();
	}

}

void Level1::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	gb->Draw();

	//DEBUG
	for each (Square* square in gb->squares)
	{
		gfx->DrawRect(square->GetXPos(), square->GetYPos(), (float)gb->squareWidth, (float)gb->squareHeight, 1.0f, 0.0f, 1.0f, 1.0f);
	}

	player->Draw();

	/*char ScoreMessage[500] = "";
	gfx->WriteText(0, 0, ScoreMessage, sprintf_s(ScoreMessage, 500, "Score: %d\n Lives %d\n",
		GameController::GetScore(), GameController::GetLives()));*/


		//DEBUG
	int sX = 0;
	int sY = 0;
	Square* test = gb->FindSquare(player->GetXPos() + (player->GetWidth() / 2), player->GetYPos());
	if (test != nullptr) {
		sX = test->GetGbX();
		sY = test->GetGbY();
	}

	gfx->DrawRect(test->GetXPos(), test->GetYPos(), (float)gb->squareWidth, (float)gb->squareHeight, 1.0f, 0.0f, 0.0f, 1.0f);


	char msg[500] = "";
	gfx->WriteDebugText(msg, sprintf_s(msg, 500, "x: %d\n"
		"y: %d\n sX: %d\n sY: %d\n mX: %d\n mY: %d\n", (int)player->GetXPos(), (int)player->GetYPos(), sX, sY, gfx->MouseX, gfx->MouseY));
}

void Level1::Notify(Observable* subject) {

	//TODO: check subject type is Player
	switch (player->GetState()->GetType())
	{
	case ActorState::ALIVE_STATE:
		continueRoadScolling = true;
		break;
	case ActorState::DEAD_STATE:
		continueRoadScolling = false;
		break;
	default:
		break;
	}
}