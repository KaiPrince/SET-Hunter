#include "GameBoardTile.h"
#include "GameBoard.h"
#include "GameObject.h"


GameBoardTile::GameBoardTile(float x, float y, float width, float height, DrawableAsset* terrain, GameWorld* gameWorld, int column, int row)
	: GameObject(x, y, width, height, (DrawableAsset*)terrain, gameWorld)
{
	assets = AssetFactory::GetNullAsset();
	gbX = column;
	gbY = row;
}

//GameBoardTile::GameBoardTile(int x, int y, float width, float height, TerrainAsset* terrain, GameBoard* gb) 
//	: GameObject(x * width, y * height, width, height, (DrawableAsset*) terrain, gb)
//{
//
//	assets = nullptr;
//}


GameBoardTile::~GameBoardTile()
{
}

void GameBoardTile::SetTerrain(DrawableAsset * terrain)
{
	this->sprite = terrain;
}

DrawableAsset * GameBoardTile::GetTerrain()
{
	return (DrawableAsset*) sprite;
}

void GameBoardTile::SetAssets(DrawableAsset * assets)
{
	this->assets = assets;
}

DrawableAsset * GameBoardTile::GetAssets()
{
	return assets;
}

int GameBoardTile::GetGbX() {
	return this->gbX;
}

int GameBoardTile::GetGbY() {
	return this->gbY;
}

void GameBoardTile::SetGbX(int x) {
	if (x > 0 && x < this->GetGameBoard()->boardWidth)
	{
		this->gbX = x;
		//this->SetXPos(x * this->GetGameBoard()->squareWidth);
	}
	else {
		//Out of bounds. Refuse to set.
	}
}

void GameBoardTile::SetGbY(int y) {
	if (y > 0 && y < this->GetGameBoard()->boardHeight) 
	{
		this->gbY = y;
		//this->SetYPos(y * this->GetGameBoard()->squareHeight);
	}
	else {
		//Out of bounds. Refuse to set.
	}
}


//void GameBoardTile::SetRectBoundary(int x, int y, float width, float height) {
//	this->x = x;
//	this->y = y;
//	this->width = width;
//	this->height = height;
//}
//
//bool GameBoardTile::ContainsPoint(float x, float y)
//{
//	if (x >= this->GetXPos() && x <= this->GetXPos() + this->GetWidth() &&
//		y >= this->GetYPos() && y <= this->GetYPos() + this->GetHeight()) {
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

void GameBoardTile::Draw()
{
	sprite->Draw(x, y, width, height);
	if (assets != nullptr)
	assets->Draw(x, y, width, height);
}

