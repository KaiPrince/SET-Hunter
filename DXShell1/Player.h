#pragma once
#include "AssetFactory.h"

class Player
{
	DrawableAsset* sprite;

	float x;
	float y;
	float width;
	float height;
public:
	Player(float x, float y, float width, float height, DrawableAsset* sprite) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->sprite = sprite;
	}
	~Player();

	float GetXPos() { return x; }
	float GetYPos() { return y; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }

	void SetXPos(float x) { this->x = x; }
	void SetYPos(float y) { this->y = y; }
	void SetWidth(float width) { this->width = width; }
	void SetHeight(float height) { this->height = height; }

	void Draw() {
		sprite->Draw(x, y, width, height);
	}
};

