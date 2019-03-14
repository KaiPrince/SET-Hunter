#include "Player.h"
#include "PlayerState.h"




Player::Player(float x, float y, float width, float height, DrawableAsset * sprite, GameBoard* gameboard, float xVelocity, float yVelocity)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->sprite = sprite;
	this->gb = gameboard;

	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;

}

Player::~Player()
{
}

void Player::Update()
{
	//_state->update(); //Update velocities

	//TODO: collision detection.
	//..

	//Advance position
	this->SetYPos(this->GetYPos() - this->GetYVelocity());
	this->SetXPos(this->GetXPos() - this->GetXVelocity());
	
}

void Player::Draw()
{
	sprite->Draw(x, y, width, height);
}

void Player::handleInput()
{
	this->xVelocity = 0;
	this->yVelocity = 0;
	if (GetKeyState(VK_UP) & 0x8000)
	{
		// UP arrow key is down.
		this->yVelocity += this->gb->squareHeight / 10;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		// DOWN arrow key is down.
		this->yVelocity += -(this->gb->squareHeight / 10);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		// RIGHT arrow key is down.
		this->xVelocity += -(this->gb->squareWidth / 10);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// LEFT arrow key is down.
		this->xVelocity += this->gb->squareWidth / 10;
	}

	//PlayerState* state = _state->handleInput(key);
	//if (state != NULL && state != _state) {
	//	//Change state

	//	delete _state;
	//	_state = state;

	//	_state->enter();
	//}

}
