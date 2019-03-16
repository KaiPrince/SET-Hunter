#include "PlayerState.h"
#include "Player.h"



PlayerState::PlayerState(Player* player)
{
	this->_player = player;
}

PlayerState::~PlayerState()
{
}

PlayerState * AliveState::handleInput()
{
	PlayerState* nextState = nullptr;

	float xVelocity = 0;
	float yVelocity = 0;
	if (GetKeyState(VK_UP) & 0x8000)
	{
		// UP arrow key is down.
		yVelocity += _player->GetGameBoard()->squareHeight / 10;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		// DOWN arrow key is down.
		yVelocity += -(_player->GetGameBoard()->squareHeight / 10);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		// RIGHT arrow key is down.
		xVelocity += -(_player->GetGameBoard()->squareWidth / 20); //TODO: make this accelerrate rather than constant speed.
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// LEFT arrow key is down.
		xVelocity += _player->GetGameBoard()->squareWidth / 20; //TODO: make this accelerrate rather than constant speed.
	}
	this->_player->SetXVelocity(xVelocity);
	this->_player->SetYVelocity(yVelocity);


	//Collision detection
	float newXPos = _player->GetXPos() - _player->GetXVelocity();
	float newYPos = _player->GetYPos() - _player->GetYVelocity();

	Square* nextSquare = _player->GetGameBoard()->FindSquare(newXPos + (_player->GetWidth() / 2), newYPos);
	if (nextSquare != nullptr && nextSquare->IsCollidable()) {
		nextState = new DeadState(_player);
	}

	return nextState;
}

void AliveState::enter()
{
}

void AliveState::update()
{
	float newXPos = _player->GetXPos() - _player->GetXVelocity();
	float newYPos = _player->GetYPos() - _player->GetYVelocity();

	//Advance position
	_player->SetXPos(newXPos);
	_player->SetYPos(newYPos);
}

DeadState::~DeadState()
{
}

PlayerState * DeadState::handleInput()
{
	Sleep(3000);
	return new AliveState(_player);
}

void DeadState::enter()
{
}

void DeadState::update()
{
}
