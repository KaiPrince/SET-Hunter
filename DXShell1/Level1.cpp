#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"
#include <string.h>
#include <stdio.h>
#include "GameBoard.h"
#include "SpriteSheetAsset.h"

#include <random>
#include <chrono>

void Level1::placePlants(GameBoard * gb)
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
		for (int row = 0; row < gb->boardHeight; row++)
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
}


void Level1::Unload()
{
}


void Level1::Update()
{

	player->SetYPos(player->GetYPos() - (gb->squareHeight / 20));

	if (player->GetYPos() <= 0) {
		player->SetYPos(gfx->Window_Height - player->GetHeight());

		placePlants(gb);
	}
}

void Level1::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 1.0f); //TODO: change to white or black

	for (int column = 0; column < gb->boardWidth; column++)
	{
		for (int row = 0; row < gb->boardHeight; row++)
		{
			Square* thisSquare = gb->squares[column][row];

			thisSquare->Draw();

			//DEBUG
			//gfx->DrawRect((float)column * gb->squareWidth, (float)row * gb->squareHeight, (float)gb->squareWidth, (float)gb->squareHeight, 0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	player->Draw();

	//DEBUG
	/*char msg[50] = "";
	gfx->WriteDebugText(msg, sprintf_s(msg, 50, "x: %d\n"
		"y : %d\n", gfx->MouseX, gfx->MouseY));*/

}