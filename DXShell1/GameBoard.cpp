#include "GameBoard.h"
#include "GameWorld.h"
#include "GraphicsLocator.h"

#include <random>
#include <chrono>
#include <ctime>



GameBoard::GameBoard(float squareWidth, float squareHeight, AssetFactory* assetFactory, GameWorld* world)
{
	this->squareWidth = squareWidth;
	this->squareHeight = squareHeight;
	this->_world = world;



	for each (Square* square in squares)
	{
		square = nullptr;
	};

	_assetFactory = assetFactory;
	_squareFactory = new SquareFactory(this, _assetFactory);
	_grassTerrain = nullptr;
	_roadTerrain = nullptr;
}


GameBoard::~GameBoard()
{
	for each (Square* square in squares)
	{
		delete square;
	};

	delete _squareFactory;
}

void GameBoard::Init()
{
	_grassTerrain = (DrawableAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::GRASS_TERRAIN);
	_roadTerrain = (DrawableAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::ROAD_TERRAIN);

	//Generate initial board layout
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

	roadTimer = std::chrono::steady_clock::now();
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
				break;
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
	using namespace std::chrono;
	time_point<std::chrono::steady_clock> currentTime = steady_clock::now();
	double elapsedTimeInMS = duration<double, std::milli>(currentTime - roadTimer).count();
	const double roadScollingDelayInMS = 1;

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
					if (row == boardHeight - 1 - 1) {
						for (Square* obstacle : _plantObstacles) {
							if (obstacle->GetGbX() == column && obstacle->GetGbY() == row + 1) {
								RemoveObstacle(obstacle);
							}
						}

						delete squares[column][row + 1];

					}

					//Move square down
					Square* thisSquare = squares[column][row];
					thisSquare->SetGbY(thisSquare->GetGbY() + 1);
					squares[column][row + 1] = thisSquare; //Shift pointer

					//Move Obstacles down
					for (Square* obstacle : _plantObstacles) {
						if (obstacle->GetGbX() == column && obstacle->GetGbY() == row) {
							obstacle->SetGbY(obstacle->GetGbY() + 1);
						}
					}

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

				//Add vertical offset to square
				thisSquare->SetYPos((thisSquare->GetGbY() * squareHeight) + verticalOffset);

				//Keep obstacles in track with their squares
				for (Square* obstacle : _plantObstacles) {
					if (obstacle->GetGbX() == column && obstacle->GetGbY() == row) {
						obstacle->SetYPos(thisSquare->GetYPos());
					}
				}
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

			for (Square* plant : _plantObstacles)
			{

				//clear any current plants
				if (plant->GetGbY() == row && plant->GetGbX() == column)  {
					RemoveObstacle(plant);
				}

			}

			//clear any current plants
			//if (thisSquare->GetAssets() != nullptr) {
			//	switch (thisSquare->GetAssets()->GetType())
			//	{
			//		//intentional fall-through
			//	case DrawableAsset::TREE_SPRITE:
			//	case DrawableAsset::TREE2_SPRITE:
			//	case DrawableAsset::SHRUB_SPRITE:
			//		thisSquare->SetAssets(AssetFactory::_emptySprite); //TODO: search through composite and replace only where necessary.
			//	default:
			//		break;
			//	}
			//}



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
				//thisSquare->SetAssets(_assetFactory->CreateDrawableAsset(plantAssetType));

				Square* newPlant = _squareFactory->CreateSquare(column, row, thisSquare->GetWidth(), thisSquare->GetHeight());
				newPlant->SetTerrain(AssetFactory::_emptySprite);
				newPlant->SetAssets(_assetFactory->CreateDrawableAsset(plantAssetType));
				newPlant->SetCollidable(true);

				AddObstacle(newPlant);

			}

		}
	}
}

void GameBoard::AddObstacle(Square * obstacle)
{
	_plantObstacles.push_back(obstacle);
	_world->AddGameObject(obstacle);
}

void GameBoard::RemoveObstacle(Square * obstacle)
{
	if (obstacle != nullptr) {
		_world->RemoveGameObject(obstacle);
		_plantObstacles.erase(std::remove(_plantObstacles.begin(), _plantObstacles.end(), obstacle), _plantObstacles.end());
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
		leftRoadSquare = FindLeftRoadSquare(row);

		//Find right Road tile
		rightRoadSquare = FindRightRoadSquare(row); //TODO: replace with left + roadwidth?

		//Link to squares above it
		if (row != 0) {

			//Left Side
			DrawLeftRoadMask(leftRoadSquare, leftRoadSquareAbove);

			//Right side
			DrawRightRoadMask(rightRoadSquare, rightRoadSquareAbove);
		}

		leftRoadSquareAbove = leftRoadSquare;
		rightRoadSquareAbove = rightRoadSquare;
	}
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

	}

	return output;
}

Square* GameBoard::FindRightRoadSquare(int row) {
	Square* output = nullptr;

	for (int column = boardWidth - 1; column >= 0; column--)
	{
		Square* thisSquare = squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
			output = thisSquare;
			break;
		}

	}

	return output;
}

void GameBoard::DrawLeftRoadMask(Square* leftRoadSquare, Square* leftRoadSquareAbove)
{
	float x1 = leftRoadSquare->GetXPos();
	float x2 = leftRoadSquareAbove->GetXPos();

	float y1 = leftRoadSquare->GetYPos();
	float y2 = y1;

	float x3 = leftRoadSquare->GetXPos() + leftRoadSquare->GetWidth();
	float y3 = y1;

	if (x1 < x2) {
		//right - Top to Top
		y1 = leftRoadSquare->GetYPos();
		y2 = leftRoadSquareAbove->GetYPos();
		x3 = leftRoadSquare->GetXPos() + leftRoadSquare->GetWidth();

		GraphicsLocator::GetGraphics()->DrawTriangle(x1, y1, x2, y2, x3, y3, 0.0f, 0.0f, 0.0f, 1.0f); //TODO: move this out of gameboard.
	}
	else if (x1 > x2)
	{
		//left - Bottom to Bottom
		y1 = leftRoadSquare->GetYPos() + leftRoadSquare->GetHeight();
		y2 = leftRoadSquareAbove->GetYPos() + leftRoadSquareAbove->GetHeight();
		x3 = leftRoadSquare->GetXPos();

		GraphicsLocator::GetGraphics()->DrawTriangle(x1, y1, x2, y2, x3, y3, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		//don't draw
	}
}

void GameBoard::DrawRightRoadMask(Square* rightRoadSquare, Square* rightRoadSquareAbove)
{
	float x1 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();
	float x2 = rightRoadSquareAbove->GetXPos() + rightRoadSquareAbove->GetWidth();

	float y1 = rightRoadSquare->GetYPos();
	float y2 = y1;

	float x3 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();
	float y3 = y1;

	if (x1 < x2) {
		//right - Bottom to Bottom
		y1 = rightRoadSquare->GetYPos() + rightRoadSquare->GetHeight();
		y2 = rightRoadSquareAbove->GetYPos() + rightRoadSquareAbove->GetHeight();
		x3 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();

		GraphicsLocator::GetGraphics()->DrawTriangle(x1, y1, x2, y2, x3, y3, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (x1 > x2)
	{
		//left - Top to Top
		y1 = rightRoadSquare->GetYPos();
		y2 = rightRoadSquareAbove->GetYPos();
		x3 = rightRoadSquare->GetXPos();

		GraphicsLocator::GetGraphics()->DrawTriangle(x1, y1, x2, y2, x3, y3, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		//don't draw
	}

}