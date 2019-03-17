#include "Square.h"
#include "GameBoard.h"
#include "GameObject.h"


Square::Square(float x, float y, float width, float height, TerrainAsset* terrain, GameBoard* gb)
	: GameObject(x, y, width, height, (DrawableAsset*)terrain, gb)
{
	assets = nullptr;
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

void Square::SetTerrain(TerrainAsset * terrain)
{
	this->sprite = terrain;
}

TerrainAsset * Square::GetTerrain()
{
	return (TerrainAsset*) sprite;
}

void Square::SetAssets(DrawableAsset * assets)
{
	this->assets = assets;
}

DrawableAsset * Square::GetAssets()
{
	return assets;
}

int Square::GetGbY() {
	return this->GetYPos() / this->GetGameBoard()->squareHeight; //TODO: fix floating-point error.
}
void Square::SetGbX(int x) {
	this->SetXPos(x * this->GetGameBoard()->squareWidth);
}

void Square::SetGbY(int y) {
	this->SetYPos(y * this->GetGameBoard()->squareHeight);
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

