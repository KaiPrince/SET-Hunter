#include "Square.h"



Square::Square()
{
	terrain = nullptr;
	assets = nullptr;

	isCollidable = false;
}


Square::~Square()
{
}

void Square::SetTerrain(TerrainAsset * terrain)
{
	this->terrain = terrain;
}

TerrainAsset * Square::GetTerrain()
{
	return terrain;
}

void Square::SetAssets(DrawableAsset * assets)
{
	this->assets = assets;
}

DrawableAsset * Square::GetAssets()
{
	return assets;
}


void Square::SetRectBoundary(int x, int y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

bool Square::ContainsPoint(float x, float y)
{
	if (x >= this->GetXPos() && x <= this->GetXPos() + this->GetWidth() &&
		y >= this->GetYPos() && y <= this->GetYPos() + this->GetHeight()) {
		return true;
	}
	else
	{
		return false;
	}
}

void Square::Draw()
{
	terrain->Draw(x * width, y * height, width, height);
	if (assets != nullptr)
	assets->Draw(x * width, y * height, width, height);
}

