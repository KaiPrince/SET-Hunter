#include "GameBoard.h"
#include "GameWorld.h"
#include "Graphics.h"
#include "Constants.h"
#include "CollisionResolutionStrategy.h"

#include <random>
#include <chrono>
#include <ctime>



GameBoard::GameBoard(float squareWidth, float squareHeight, AssetFactory* assetFactory, GameWorld* world)
{
	this->squareWidth = squareWidth;
	this->squareHeight = squareHeight;
	this->_world = world;


	for each (GameBoardTile* square in squares)
	{
		square = nullptr;
	};

	_assetFactory = assetFactory;
	_squareFactory = new TileFactory(world, _assetFactory);
	_grassTerrain = nullptr;
	_roadTerrain = nullptr;

	roadTimer = std::chrono::steady_clock::now();
	roadShift = 0;
	verticalOffset = 0.0f;
}


GameBoard::~GameBoard()
{
	for each (GameBoardTile* square in squares)
	{
		delete square;
	};

	delete _squareFactory;
}

void GameBoard::Init()
{
	_grassTerrain = (DrawableAsset*)_assetFactory->GetAsset(DrawableAsset::GRASS_TERRAIN);
	_roadTerrain = (DrawableAsset*)_assetFactory->GetAsset(DrawableAsset::ROAD_TERRAIN);

	//Generate initial board layout
	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			squares[column][row] = CreateSquare(column, row);
			GameBoardTile* thisSquare = squares[column][row];

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

}

GameBoardTile * GameBoard::FindSquare(float xPos, float yPos)
{
	GameBoardTile* output = squares[0][0];

	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			GameBoardTile* thisSquare = squares[column][row];
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

GameBoardTile * GameBoard::CreateSquare(int column, int row)
{
	GameBoardTile* output = nullptr;

	//input validation
	if (column >= 0 && column <= this->boardWidth &&
		row >= 0 && row <= this->boardHeight) {

		//Create square from factory
		output = this->_squareFactory->CreateSquare(column, row, squareWidth, squareHeight);
	}

	return output;
}

/*
Function Name: ScrollBoard
Description: This method is used to make the gameboard appear to scroll downwards.
Params: None.
Returns: None.
*/
void GameBoard::ScrollBoard() {
	//Set up timers, define constants.
	using namespace std::chrono;
	time_point<std::chrono::steady_clock> currentTime = steady_clock::now();
	double elapsedTimeInMS = duration<double, std::milli>(currentTime - roadTimer).count();

	//Scroll roadway
	if (elapsedTimeInMS >= roadScrollingDelayInMS) {
		roadTimer = currentTime;

		verticalOffset += roadScrollingSpeed;
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

						for (GameBoardTile* obstacle : _plantObstacles) {
							if (obstacle->GetGbX() == column && obstacle->GetGbY() == row + 1) {
								RemoveObstacle(obstacle);
							}
						}

						delete squares[column][row + 1];

					}

					//Move square down
					GameBoardTile* thisSquare = squares[column][row];
					thisSquare->SetGbY(thisSquare->GetGbY() + 1);
					squares[column][row + 1] = thisSquare; //Shift pointer

					//Move Obstacles down
					for (GameBoardTile* obstacle : _plantObstacles) {
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
				GameBoardTile* thisSquare = squares[column][row];


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
				GameBoardTile* thisSquare = squares[column][row];

				//Add vertical offset to square
				thisSquare->SetYPos((thisSquare->GetGbY() * squareHeight) + verticalOffset);

				//Keep obstacles in track with their squares
				for (GameBoardTile* obstacle : _plantObstacles) {
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
		GameBoardTile* thisSquare = squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::GRASS_TERRAIN) {

			for (GameBoardTile* plant : _plantObstacles)
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
			//		thisSquare->SetAssets(AssetFactory::GetNullAsset()); //TODO: search through composite and replace only where necessary.
			//	default:
			//		break;
			//	}
			//}



			//random 1 in 20 chance to have plant asset
			if (dice_containsPlantAsset(generator) == 1) {
				DrawableAsset::AssetTypes plantAssetType;

				GameBoardTile* newPlant = _squareFactory->CreateSquare(column, row, thisSquare->GetWidth(), thisSquare->GetHeight());
				newPlant->SetTerrain(AssetFactory::GetNullAsset());

				newPlant->SetPhysicsComponent(new CollidablePhysicsComponent(newPlant, _world));
				newPlant->GetPhysicsComponent()->SetCollisionStrategy(new DeathTouchCollisionStrategy(newPlant));

				switch (dice_typeOfPlantAsset(generator))
				{
				case 1:
					plantAssetType = DrawableAsset::TREE_SPRITE;
					newPlant->GetPhysicsComponent()->ChangeHitbox(newPlant->GetWidth() / 3, newPlant->GetHeight() / 5, newPlant->GetWidth() / 3, newPlant->GetHeight() / 3);
					break;
				case 2:
					plantAssetType = DrawableAsset::TREE2_SPRITE;
					break;
				case 3:
					plantAssetType = DrawableAsset::SHRUB_SPRITE;
					break;
				default:
					//ERROR! random number out of expected range!
					throw;
					break;
				}

				newPlant->GetPhysicsComponent()->ChangeHitbox(newPlant->GetWidth() / 3, newPlant->GetHeight() / 5, newPlant->GetWidth() / 3, newPlant->GetHeight() / 3);
				newPlant->SetAssets(_assetFactory->GetAsset(plantAssetType));


				AddObstacle(newPlant);

			}

		}
		else if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
			//Place coin with 1 in 20 chance

			//random 1 in 20 chance to have plant asset
			if (dice_containsPlantAsset(generator) == 1) {
				DrawableAsset::AssetTypes coinAssetType = DrawableAsset::COIN_SPRITE;

				GameBoardTile* newCoin = _squareFactory->CreateSquare(column, row, thisSquare->GetWidth(), thisSquare->GetHeight());
				newCoin->SetTerrain(AssetFactory::GetNullAsset());

				newCoin->SetPhysicsComponent(new CollidablePhysicsComponent(newCoin, _world));
				newCoin->GetPhysicsComponent()->SetCollisionStrategy(new CoinCollisionStrategy(newCoin));


				newCoin->GetPhysicsComponent()->ChangeHitbox(newCoin->GetWidth() / 3, newCoin->GetHeight() / 5, newCoin->GetWidth() / 3, newCoin->GetHeight() / 3);
				newCoin->SetAssets(_assetFactory->GetAsset(coinAssetType));


				AddObstacle(newCoin);

			}

		}
	}
}

void GameBoard::AddObstacle(GameBoardTile * obstacle)
{
	_plantObstacles.push_back(obstacle);
	_world->AddGameObject(obstacle);
}

void GameBoard::RemoveObstacle(GameBoardTile * obstacle)
{
	if (obstacle != nullptr) {
		_world->RemoveGameObject(obstacle);
		_plantObstacles.erase(std::remove(_plantObstacles.begin(), _plantObstacles.end(), obstacle), _plantObstacles.end());

		delete obstacle;
	}
}


void GameBoard::Draw() {
	for (int column = 0; column < boardWidth; column++)
	{
		for (int row = 0; row < boardHeight; row++)
		{
			GameBoardTile* thisSquare = squares[column][row];

			thisSquare->Draw();



		}

	}

	DrawRoadMask();
}

//TODO: don't leave this here.
//Blend the outer edges
void GameBoard::DrawRoadMask()
{

	GameBoardTile* leftRoadSquare = nullptr;
	GameBoardTile* leftRoadSquareAbove = nullptr;
	GameBoardTile* rightRoadSquare = nullptr;
	GameBoardTile* rightRoadSquareAbove = nullptr;
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

GameBoardTile* GameBoard::FindLeftRoadSquare(int row)
{
	if (row < 0) row = 0; //Safeguard.

	GameBoardTile* output = nullptr;
	//Find left Road tile
	for (int column = 0; column < boardWidth; column++)
	{
		GameBoardTile* thisSquare = squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
			output = thisSquare;
			break;
		}

	}

	return output;
}

GameBoardTile* GameBoard::FindRightRoadSquare(int row) {
	GameBoardTile* output = nullptr;

	for (int column = boardWidth - 1; column >= 0; column--)
	{
		GameBoardTile* thisSquare = squares[column][row];

		if (thisSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
			output = thisSquare;
			break;
		}

	}

	return output;
}

void GameBoard::DrawLeftRoadMask(GameBoardTile* leftRoadSquare, GameBoardTile* leftRoadSquareAbove)
{
	float x1 = leftRoadSquare->GetXPos();
	float x2 = leftRoadSquareAbove->GetXPos();

	float y1 = leftRoadSquare->GetYPos();
	float y2 = y1;

	float x3 = leftRoadSquare->GetXPos() + leftRoadSquare->GetWidth();
	float y3 = y1;

	//This smudge factor here is a quick fix for a misalignment problem between the triangles and the tiles.
	const float smudgeX = 1.0f;
	const float smudgeY = -1.0f;

	if (x1 < x2) {
		//right - Top to Top
		y1 = leftRoadSquare->GetYPos();
		y2 = leftRoadSquareAbove->GetYPos();
		x3 = leftRoadSquare->GetXPos() + leftRoadSquare->GetWidth();

		const float smudgeY = 1.0f; //smudge down

		GraphicsLocator::GetGraphics()->DrawTriangle(x1 + smudgeX, y1 + smudgeY, x2 + smudgeX, y2 + smudgeY, x3 + smudgeX, y3 + smudgeY, 0.14f, 0.14f, 0.14f, 1.0f); //TODO: move this out of gameboard.
	}
	else if (x1 > x2)
	{
		//left - Bottom to Bottom
		y1 = leftRoadSquare->GetYPos() + leftRoadSquare->GetHeight();
		y2 = leftRoadSquareAbove->GetYPos() + leftRoadSquareAbove->GetHeight();
		x3 = leftRoadSquare->GetXPos();

		const float smudgeY = -1.0f; //smudge up

		GraphicsLocator::GetGraphics()->DrawTriangle(x1 + smudgeX, y1 + smudgeY, x2 + smudgeX, y2 + smudgeY, x3 + smudgeX, y3 + smudgeY, 0.14f, 0.14f, 0.14f, 1.0f);
	}
	else {
		//don't draw
	}
}

void GameBoard::DrawRightRoadMask(GameBoardTile* rightRoadSquare, GameBoardTile* rightRoadSquareAbove)
{
	float x1 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();
	float x2 = rightRoadSquareAbove->GetXPos() + rightRoadSquareAbove->GetWidth();

	float y1 = rightRoadSquare->GetYPos();
	float y2 = y1;

	float x3 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();
	float y3 = y1;

	//This smudge factor here is a quick fix for a misalignment problem between the triangles and the tiles.
	const float smudgeX = -1.0f;

	if (x1 < x2) {
		//right - Bottom to Bottom
		y1 = rightRoadSquare->GetYPos() + rightRoadSquare->GetHeight();
		y2 = rightRoadSquareAbove->GetYPos() + rightRoadSquareAbove->GetHeight();
		x3 = rightRoadSquare->GetXPos() + rightRoadSquare->GetWidth();

		const float smudgeY = -1.0f; //smudge up

		GraphicsLocator::GetGraphics()->DrawTriangle(x1 + smudgeX, y1 + smudgeY, x2 + smudgeX, y2 + smudgeY, x3 + smudgeX, y3 + smudgeY, 0.14f, 0.14f, 0.14f, 1.0f); //TODO: move this out of gameboard.
	}
	else if (x1 > x2)
	{
		//left - Top to Top
		y1 = rightRoadSquare->GetYPos();
		y2 = rightRoadSquareAbove->GetYPos();
		x3 = rightRoadSquare->GetXPos();

		const float smudgeY = 1.0f; //smudge down

		GraphicsLocator::GetGraphics()->DrawTriangle(x1 + smudgeX, y1 + smudgeY, x2 + smudgeX, y2 + smudgeY, x3 + smudgeX, y3 + smudgeY, 0.14f, 0.14f, 0.14f, 1.0f); //TODO: move this out of gameboard.
	}
	else {
		//don't draw
	}

}