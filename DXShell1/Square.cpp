#include "Square.h"



Square::Square()
{
	terrain = nullptr;
	assets = nullptr;
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


void Square::SetRectBoundary(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Square::Draw()
{
	terrain->Draw(x * width, y * height, width, height);
	if (assets != nullptr)
	assets->Draw(x * width, y * height, width, height);
}

