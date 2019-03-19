#include "GameBoard.h"
#include "GraphicsLocator.h"

#include <random>
#include <chrono>
#include <ctime>



GameBoard::GameBoard()
{
	for each (Square* square in squares)
	{
		square = nullptr;
	};
	_squareFactory = nullptr;
	_assetFactory = nullptr;
	_grassTerrain = nullptr;
	_roadTerrain = nullptr;
}


GameBoard::~GameBoard()
{
}

void GameBoard::Init()
{
	_squareFactory = new SquareFactory(this, _assetFactory);
	_grassTerrain = (TerrainAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::GRASS_TERRAIN);
	_roadTerrain = (TerrainAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::ROAD_TERRAIN);

	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			squares[column][row] = CreateSquare(column, row);
			Square* thisSquare = squares[column][row];

			//Left third or right third
			if (column + 1 <= boardWidth / 3 || column + 1 > boardWidth - (boardWidth / 3))
			{
				thisSquare->SetTerrain(_grassTerrain);
			}
			else
			{
				thisSquare->SetTerrain(_roadTerrain);
			}

		}
	}

	placePlants();

	roadTimer = clock(); //std::chrono::high_resolution_clock::now();
	roadShift = 0;
	verticalOffset = 0.0f;
}

Square * GameBoard::FindSquare(float xPos, float yPos)
{
	Square* output = nullptr;

	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			Square* thisSquare = squares[column][row];
			if (thisSquare->ContainsPoint(xPos, yPos)) {
				output = thisSquare;
			}
		}
	}

	return output;
}

void GameBoard::SetAssetFactory(AssetFactory * assetFactory)
{
	_assetFactory = assetFactory;
}

Square * GameBoard::CreateSquare(int column, int row)
{
	Square* output = nullptr;

	//input validation
	if (column >= 0 && column <= this->boardWidth &&
		row >= 0 && row <= this->boardHeight) {

		//Create square from factory
		output = this->_squareFactory->CreateSquare(column, row, squareWidth, squareHeight);

	}

	return output;
}

void GameBoard::ScrollBoard() {

	std::clock_t currentTime = clock();
	double elapsedTimeInMS = std::chrono::duration<double, std::milli>(currentTime - roadTimer).count();
	const double roadScollingDelayInMS = 20;

	//Scroll roadway
	if (elapsedTimeInMS >= roadScollingDelayInMS) {
		roadTimer = currentTime;

		verticalOffset += squareHeight / 20;
		if (verticalOffset >= 0) {
			verticalOffset = -squareHeight;

			//Shift squares
			//Loop bottom - 1 to top, left to right
			for (int row = (boardHeight - 1) - 1; row >= 0; row--)
			{
				for (int column = 0; column < boardWidth; column++)
				{
					//Free squares on the bottom row
					if (row == boardHeight - 1) {
						delete squares[column][row + 1];
					}

					//Move square down
					Square* thisSquare = squares[column][row];
					thisSquare->SetGbY(thisSquare->GetGbY() + 1);
					squares[column][row + 1] = thisSquare; //Shift pointer

					//Clear squares on top row
					if (row == 0) {
						squares[column][row] = nullptr; //Warning: this may cause errors elsewhere.
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
			int roadLeftSide = (boardWidth / 3) + roadShift; //TODO: move these constants
			int roadRightSide = (boardWidth - (boardWidth / 3)) + roadShift;
			//int roadWidth = roadRightSide - roadLeftSide;
			if (roadLeftSide < 1)
			{
				this->roadShift = 1 - (boardWidth / 3);
			}
			else if (roadRightSide > boardWidth - 1)
			{
				this->roadShift = (boardWidth / 3) - 1;
			}
			else
			{
				//roadShift is in bounds.
			}

			//Generate new top row
			for (int column = 0, row = 0; column < boardWidth; column++)
			{
				squares[column][row] = CreateSquare(column, row);
				Square* thisSquare = squares[column][row];


				//Place road
				int roadLeftSide = (boardWidth / 3) + roadShift; //TODO: move these constants
				int roadRightSide = (boardWidth - (boardWidth / 3)) + roadShift;
				//int roadWidth = roadRightSide - roadLeftSide;
				if (column + 1 <= roadLeftSide || column + 1 > roadRightSide)
				{
					thisSquare->SetTerrain(_grassTerrain);
				}
				else
				{
					thisSquare->SetTerrain(_roadTerrain);
				}

			}

			//Generate new terrain on top row
			placePlants(0);

		}

		for (int column = 0; column < boardWidth; column++)
		{
			for (int row = 0; row < boardHeight; row++)
			{
				Square* thisSquare = squares[column][row];

				thisSquare->SetYPos((thisSquare->GetGbY() * squareHeight) + verticalOffset);
			}
		}

	}
}

void GameBoard::placePlants()
{

	for (int row = 0; row < boardHeight; row++)
	{
		placePlants(row);
	}

}

void GameBoard::placePlants(int row)
{
	//Random number generators
	//std::default_random_engine generator;
	unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	const int unlikelyhoodOfContainingPlantAsset = 20;
	const int numberOfPlantAssetTypes = 3; //TODO: make this dynamically linked to the DrawableAsset::AssetTypes enum
	std::uniform_int_distribution<int> dice_containsPlantAsset(1, unlikelyhoodOfContainingPlantAsset);
	std::uniform_int_distribution<int> dice_typeOfPlantAsset(1, numberOfPlantAssetTypes);

	for (int column = 0; column < boardWidth; column++)
	{
		Square* thisSquare = squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::GRASS_TERRAIN) {
			//clear any current plants
			if (thisSquare->GetAssets() != nullptr) {
				switch (thisSquare->GetAssets()->GetType())
				{
					//intentional fall-through
				case DrawableAsset::TREE_SPRITE:
				case DrawableAsset::TREE2_SPRITE:
				case DrawableAsset::SHRUB_SPRITE:
					thisSquare->SetAssets(nullptr);
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
				thisSquare->SetAssets(_assetFactory->CreateDrawableAsset(plantAssetType));

				thisSquare->SetCollidable(true);
			}

		}
	}
}


void GameBoard::Draw() {
	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			Square* thisSquare = squares[column][row];

			thisSquare->Draw();



		}
	}

	DrawRoadMask();
}

//TODO: don't leave this here.
//Blend the outer edges
void GameBoard::DrawRoadMask()
{

	Square* leftRoadSquare = nullptr;
	Square* leftRoadSquareAbove = nullptr;
	Square* rightRoadSquare = nullptr;
	Square* rightRoadSquareAbove = nullptr;
	for (int row = 0; row < boardHeight; row++)
	{
		//Find left Road tile
		for (int column = 0; column < boardWidth; column++)
		{
			Square* thisSquare = squares[column][row];

			if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
				leftRoadSquare = thisSquare;
				break;
			}

Square* GameBoard::FindLeftRoadSquare(int row)
{
	Square* output = nullptr;
	//Find left Road tile
	for (int column = 0; column < boardWidth; column++)
	{
		Square* thisSquare = squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
			output = thisSquare;
			break;
		}

		//Find right Road tile
		for (int column = boardWidth - 1; column >= 0; column--)
		{
			Square* thisSquare = squares[column][row];

			if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
				rightRoadSquare = thisSquare;
				break;
			}

		}

		//Link to squares above it
		if (row != 0) {

			//Left Side
			{
				float x1 = leftRoadSquare->GetXPos();
				float x2 = leftRoadSquareAbove->GetXPos();

				float y1 = leftRoadSquare->GetYPos();
				float y2 = y1;// = leftRoadSquareAbove->GetYPos() + leftRoadSquareAbove->GetHeight();

				if (x1 < x2) {
					//right - Top to Top
					y1 = leftRoadSquare->GetYPos();
					y2 = leftRoadSquareAbove->GetYPos();
				}
				else if (x1 > x2)
				{
					//left - Bottom to Bottom
					y1 = leftRoadSquare->GetYPos() + leftRoadSquare->GetHeight();
					y2 = leftRoadSquareAbove->GetYPos() + leftRoadSquareAbove->GetHeight();
				}
				else {
					//don't draw
				}
				GraphicsLocator::GetGraphics()->DrawLine(x1, y1, x2, y2, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f);
				GraphicsLocator::GetGraphics()->DrawTriangle(x1, y1, x2, y2, leftRoadSquare->GetXPos(), leftRoadSquare->GetYPos(), 1.0f, 1.0f, 0.0f, 1.0f);
			}

			//Right side
			{
				float x1 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();
				float x2 = rightRoadSquareAbove->GetXPos() + rightRoadSquareAbove->GetWidth();

				float y1 = rightRoadSquare->GetYPos();
				float y2 = y1;// = leftRoadSquareAbove->GetYPos() + leftRoadSquareAbove->GetHeight();

				if (x1 < x2) {
					//right - Bottom to Bottom
					y1 = rightRoadSquare->GetYPos() + rightRoadSquare->GetHeight();
					y2 = rightRoadSquareAbove->GetYPos() + rightRoadSquareAbove->GetHeight();
				}
				else if (x1 > x2)
				{
					//left - Top to Top
					y1 = rightRoadSquare->GetYPos();
					y2 = rightRoadSquareAbove->GetYPos();
				}
				else {
					//don't draw
				}
				GraphicsLocator::GetGraphics()->DrawLine(x1, y1, x2, y2, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		leftRoadSquareAbove = leftRoadSquare;
		rightRoadSquareAbove = rightRoadSquare;
	}

	return output;
}

}
