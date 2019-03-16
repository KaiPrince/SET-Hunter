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

	this->_state = new AliveState(this);
}

Player::~Player()
{
}

void Player::Update()
{
	PlayerState* newState = _state->update(); //Update position
	if (newState != nullptr) {
		//Change state

		delete _state;
		_state = newState;

		_state->enter();
	}
}

void Player::Draw()
{
	sprite->Draw(x, y, width, height);
}

void Player::handleInput()
{

	PlayerState* newState = _state->handleInput();
	if (newState != nullptr) {
		//Change state

		delete _state;
		_state = newState;

		_state->enter();
	}

}
