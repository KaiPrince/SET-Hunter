#pragma once
#include "AssetFactory.h"
#include "GameBoard.h"
#include "Observer.h"

class PlayerState;

/*
Class Name: Player
Purpose: This class is used to represent a player in the game. 
	It will manage position and state, handle input, update its position and state, 
	and draw itself to the screen.
*/
class Player : public Observable
{
private:
	DrawableAsset* sprite;
	GameBoard* gb;

	PlayerState* _state;

	float x;
	float y;
	float width;
	float height;

	float xVelocity;
	float yVelocity;
public:
	Player(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard, 
		float xVelocity = 0.0f, float yVelocity = 0.0f);
	~Player();

	float GetXPos() { return x; }
	float GetYPos() { return y; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetXVelocity() { return xVelocity; }
	float GetYVelocity() { return yVelocity; }
	GameBoard* GetGameBoard() { return gb; }
	PlayerState* GetState() { return _state; }

	void SetXPos(float x) { 
		//Check Bounds
		if (x <= (this->gb->boardWidth * this->gb->squareWidth) - this->width && x >= 0) {
			this->x = x;
		}
	
	}

	void SetYPos(float y) { 
		//Check Bounds
		const float maxheight = (this->gb->boardHeight * this->gb->squareHeight) - (this->height * 2);
		const float minheight = (this->gb->boardHeight * this->gb->squareHeight) / 2;

		if (y <= maxheight && y >= minheight) {
			this->y = y; 
		}
		else if (y > maxheight) {
			this->y = maxheight;
		}
		else if (y < minheight)
		{
			this->y = minheight;
		}
		else
		{
			//Out of bounds. Refuse to set.
		}
	}
	void SetWidth(float width) { this->width = width; }
	void SetHeight(float height) { this->height = height; }
	void SetXVelocity(float xVelocity) { this->xVelocity = xVelocity; }
	void SetYVelocity(float yVelocity) { this->yVelocity = yVelocity; }
	void SetGameBoard(GameBoard* gb) { this->gb = gb; }
	void SetSprite(DrawableAsset* sprite) { this->sprite = sprite; }

	void Update();

	void Draw();

	void handleInput();
};
