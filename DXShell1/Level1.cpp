#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "SpriteSheetAsset.h"

#include <random>
#include <chrono>
#include <ctime>

void Level1::placePlants(GameBoard * gb)
{

	for (int row = 0; row < gb->boardHeight; row++)
	{
		placePlants(gb, row);
	}

}

void Level1::placePlants(GameBoard * gb, int row)
{
	//Random number generators
	//std::default_random_engine generator;
	unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	const int unlikelyhoodOfContainingPlantAsset = 20;
	const int numberOfPlantAssetTypes = 3; //TODO: make this dynamically linked to the DrawableAsset::AssetTypes enum
	std::uniform_int_distribution<int> dice_containsPlantAsset(1, unlikelyhoodOfContainingPlantAsset);
	std::uniform_int_distribution<int> dice_typeOfPlantAsset(1, numberOfPlantAssetTypes);

	for (int column = 0; column < gb->boardWidth; column++)
	{
		Square* thisSquare = gb->squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::GRASS_TERRAIN) {
			//clear any current plants
			if (thisSquare->GetAssets() != nullptr) {
				switch (thisSquare->GetAssets()->GetType())
				{
					//intentional fall-through
				case DrawableAsset::TREE_SPRITE:
				case DrawableAsset::TREE2_SPRITE:
				case DrawableAsset::SHRUB_SPRITE:
					thisSquare->SetAssets(nullptr); //TODO: search through composite and replace only where necessary.
				default:
					break;
				}
			}



			//random 1 in 20 chance to have plant asset
			if (dice_containsPlantAsset(generator) == 1) {
				DrawableAsset::AssetTypes plantAssetType;
				switch (dice_typeOfPlantAsset(generator))
				{
				case 1:
					plantAssetType = DrawableAsset::TREE_SPRITE;
					break;
				case 2:
					plantAssetType = DrawableAsset::TREE2_SPRITE;
					break;
				case 3:
					plantAssetType = DrawableAsset::SHRUB_SPRITE;
					break;
				default:
					//ERROR! random number out of expected range!
					plantAssetType = DrawableAsset::CAR_SPRITE; //TODO: actual error handling
					break;
				}
				thisSquare->SetAssets(GameLevel::_assetFactory->CreateDrawableAsset(plantAssetType));
				//TODO: find some way to clear only plant assets from a possible composite asset. Use public type var? (like with terrain?)
			}

		}
	}
}

void Level1::Load()
{

	for (int column = 0; column < gb->boardWidth; column++)
	{
		for (int row = 0; row < gb->boardHeight; row++)
		{
			Square* thisSquare = gb->squares[column][row];

			//Left third or right third
			if (column + 1 <= gb->boardWidth / 3 || column + 1 > gb->boardWidth - (gb->boardWidth / 3))
			{
				thisSquare->SetTerrain(gb->_grassTerrain);
			}
			else
			{
				thisSquare->SetTerrain(gb->_roadTerrain);
			}
		}
	}

	placePlants(gb);

	roadTimer = clock(); //std::chrono::high_resolution_clock::now();
	roadShift = 0;
}


void Level1::Unload()
{
}


void Level1::Update()
{

	player->Update();

	std::clock_t currentTime = clock();

	double elapsedTimeInMS = std::chrono::duration<double, std::milli>(currentTime - roadTimer).count();

	const double roadScollingSpeedInMS = 200;

	//Scroll roadway
	if (elapsedTimeInMS >= roadScollingSpeedInMS) {
		roadTimer = currentTime;

		//Shift squares
		//Loop bottom - 1 to top, left to right
		for (int row = (gb->boardHeight - 1) - 1; row >= 0; row--)
		{
			for (int column = 0; column < gb->boardWidth; column++)
			{
				//Free squares on the bottom row
				if (row == gb->boardHeight - 1) {
					delete gb->squares[column][row + 1];
				}

				//Move square down
				Square* thisSquare = gb->squares[column][row];
				thisSquare->SetY(thisSquare->GetY() + 1);
				gb->squares[column][row + 1] = thisSquare; //Shift pointer

				//Clear squares on top row
				if (row == 0) {
					gb->squares[column][row] = nullptr; //Warning: this may cause errors elsewhere.
				}
			}
		}

		//Randomly shift road
		const int roadShiftStability = 5;
		const int shiftLeft = 1;
		const int shiftRight = 2;

		//TODO: move these to class members (static?)
		unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> dice_roadShift(1, roadShiftStability);
		switch (dice_roadShift(generator))
		{
		case shiftLeft:
			this->roadShift += -1;
			break;
		case shiftRight:
			this->roadShift += 1;
			break;
		default:
			break;
		}

		/*
			Math for the road shift:
			-------------------------
			rLS = 3 + rS
			if rLS < 1
				3 + rS < 1
				rS < 1 - 3

			rRS = 10 - 3 + rS
			if rRS > 10 - 1
				10 - 3 + rS > 10 - 1
				rS > 10 - 1 - 10 + 3
				rs > 3 - 1
		*/
		int roadLeftSide = (gb->boardWidth / 3) + roadShift; //TODO: move these constants
		int roadRightSide = (gb->boardWidth - (gb->boardWidth / 3)) + roadShift;
		//int roadWidth = roadRightSide - roadLeftSide;
		if (roadLeftSide < 1)
		{
			this->roadShift = 1 - (gb->boardWidth / 3);
		}
		else if (roadRightSide > gb->boardWidth - 1)
		{
			this->roadShift = (gb->boardWidth / 3) - 1;
		}
		else
		{
			//roadShift is in bounds.
		}

		//Generate new top row
		for (int column = 0, row = 0; column < gb->boardWidth; column++)
		{
			gb->squares[column][row] = gb->CreateSquare(column, row);
			Square* thisSquare = gb->squares[column][row];


			//Place road
			int roadLeftSide = (gb->boardWidth / 3) + roadShift; //TODO: move these constants
			int roadRightSide = (gb->boardWidth - (gb->boardWidth / 3)) + roadShift; //TODO: move these constants
			if (column + 1 <= roadLeftSide || column + 1 > roadRightSide)
			{
				thisSquare->SetTerrain(gb->_grassTerrain);
			}
			else
			{
				thisSquare->SetTerrain(gb->_roadTerrain);
			}

		}

		//Generate new terrain on top row
		placePlants(gb, 0);
	}
}

void Level1::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.0f);

	for (int column = 0; column < gb->boardWidth; column++)
	{
		for (int row = 0; row < gb->boardHeight; row++)
		{
			Square* thisSquare = gb->squares[column][row];

			thisSquare->Draw();

			//DEBUG
			gfx->DrawRect((float)column * gb->squareWidth, (float)row * gb->squareHeight, (float)gb->squareWidth, (float)gb->squareHeight, 0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	player->Draw();

	int sX = 0;
	int sY = 0;
	Square* test = gb->FindSquare(player->GetXPos() + (player->GetWidth() / 2), player->GetYPos());
	if (test != nullptr) {
		sX = test->GetX();
		sY = test->GetY();
	}

	//DEBUG
	char msg[500] = "";
	gfx->WriteDebugText(msg, sprintf_s(msg, 500, "x: %d\n"
		"y: %d\n sX: %d\n sY: %d\n mX: %d\n mY: %d\n", (int)player->GetXPos(), (int)player->GetYPos(), sX, sY, gfx->MouseX, gfx->MouseY));

}