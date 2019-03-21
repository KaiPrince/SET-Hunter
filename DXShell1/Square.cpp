#include "Square.h"
#include "GameBoard.h"
#include "GameObject.h"


Square::Square(float x, float y, float width, float height, DrawableAsset* terrain, GameBoard* gb)
	: GameObject(x, y, width, height, (DrawableAsset*)terrain, gb)
{
	assets = AssetFactory::_emptySprite;
}

//Square::Square(int x, int y, float width, float height, TerrainAsset* terrain, GameBoard* gb) 
//	: GameObject(x * width, y * height, width, height, (DrawableAsset*) terrain, gb)
//{
//
//	assets = nullptr;
//}


Square::~Square()
{
}

void Square::SetTerrain(DrawableAsset * terrain)
{
	this->sprite = terrain;
}

DrawableAsset * Square::GetTerrain()
{
	return (DrawableAsset*) sprite;
}

void Square::SetAssets(DrawableAsset * assets)
{
	this->assets = assets;
}

DrawableAsset * Square::GetAssets()
{
	return assets;
}

int Square::GetGbX() {
	return this->gbX;
}

int Square::GetGbY() {
	return this->gbY;
}

void Square::SetGbX(int x) {
	if (x > 0 && x < this->GetGameBoard()->boardWidth)
	{
		this->gbX = x;
		//this->SetXPos(x * this->GetGameBoard()->squareWidth);
	}
	else {
		//Out of bounds. Refuse to set.
	}
}

void Square::SetGbY(int y) {
	if (y > 0 && y < this->GetGameBoard()->boardHeight) 
	{
		this->gbY = y;
		//this->SetYPos(y * this->GetGameBoard()->squareHeight);
	}
	else {
		//Out of bounds. Refuse to set.
	}
}


//void Square::SetRectBoundary(int x, int y, float width, float height) {
//	this->x = x;
//	this->y = y;
//	this->width = width;
//	this->height = height;
//}
//
//bool Square::ContainsPoint(float x, float y)
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

void Square::Draw()
{
	sprite->Draw(x, y, width, height);
	if (assets != nullptr)
	assets->Draw(x, y, width, height);
}

