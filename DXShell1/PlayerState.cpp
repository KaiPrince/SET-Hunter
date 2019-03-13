#include "PlayerState.h"
#include "Player.h"



PlayerState::PlayerState(Player* player)
{
	this->_player = player;
}

PlayerState::~PlayerState()
{
}

PlayerState * NoMoveState::handleInput(int key)
{
	PlayerState* state = nullptr;
	switch (key)
	{
	case VK_UP:
		state = new MoveUpState(_player);
		break;
	case VK_DOWN:
		state = new MoveDownState(_player);
		break;
	case VK_RIGHT:
		state = new MoveRightState(_player);
		break;
	case VK_LEFT:
		state = new MoveLeftState(_player);
		break;
	case 0: //No key pressed.
		state = new NoMoveState(_player);
		break;
	default: //Unhandled key pressed.
		break;
	}

	return state;
}

void NoMoveState::enter()
{
	_player->SetXVelocity(0.0f);
	_player->SetYVelocity(0.0f);
}

void NoMoveState::update()
{
}

PlayerState * MoveUpState::handleInput(int key)
{
	PlayerState* state = nullptr;
	switch (key)
	{
	case VK_UP:
		if (_player->GetYPos() > (_player->GetGameBoard()->boardHeight * _player->GetGameBoard()->squareHeight) / 2) {
			state = this;
		}
		else {
			state = new NoMoveState(_player);
		}
		break;
	case VK_DOWN:
		break;
	case VK_RIGHT:
		break;
	case VK_LEFT:
		break;
	case 0: //No key pressed.
		state = new NoMoveState(_player);
		break;
	default: //Unhandled key pressed.
		break;
	}
	return state;
}

void MoveUpState::enter()
{
	//_player->SetXVelocity(0.0f);
	if (_player->GetYPos() > (_player->GetGameBoard()->boardHeight * _player->GetGameBoard()->squareHeight) / 2) {
		_player->SetYVelocity(_player->GetGameBoard()->squareHeight / 20);
	}
	else {
		//Do nothing. Will transition back to NoMoveState;
	}
	
}

void MoveUpState::update()
{
	_player->SetYPos(_player->GetYPos() - _player->GetYVelocity());
}

PlayerState * MoveDownState::handleInput(int key)
{
	PlayerState* state = nullptr;
	switch (key)
	{
	case VK_UP:
		//state = new MoveUpState(_player);
		break;
	case VK_DOWN:
		if (_player->GetYPos() > 0 + _player->GetHeight()) {
			state = this;
		}
		else {
			state = new NoMoveState(_player);
		}
		break;
	case VK_RIGHT:
		break;
	case VK_LEFT:
		break;
	case 0: //No key pressed.
		state = new NoMoveState(_player);
		break;
	default: //Unhandled key pressed.
		break;
	}
	return state;
}

void MoveDownState::enter()
{
	//_player->SetXVelocity(0.0f);
	_player->SetYVelocity(-(_player->GetGameBoard()->squareHeight / 20));
}

void MoveDownState::update()
{
	_player->SetYPos(_player->GetYPos() - _player->GetYVelocity());
}

PlayerState * MoveLeftState::handleInput(int key)
{
	PlayerState* state = nullptr;
	switch (key)
	{
	case VK_UP:
		//state = new MoveUpState(_player);
		break;
	case VK_DOWN:
		break;
	case VK_RIGHT:
		break;
	case VK_LEFT:
		if (_player->GetXPos() > 0 + _player->GetWidth()) {
			state = this;
		}
		else {
			state = new NoMoveState(_player);
		}
		break;
	case 0: //No key pressed.
		state = new NoMoveState(_player);
		break;
	default: //Unhandled key pressed.
		break;
	}
	return state;
}

void MoveLeftState::enter()
{
	_player->SetXVelocity(_player->GetGameBoard()->squareWidth / 20);
	//_player->SetYVelocity(20.0f); //gb->squareHeight / 20
}

void MoveLeftState::update()
{
}

PlayerState * MoveRightState::handleInput(int key)
{
	PlayerState* state = nullptr;
	switch (key)
	{
	case VK_UP:
		//state = new MoveUpState(_player);
		break;
	case VK_DOWN:
		break;
	case VK_RIGHT:
		if (_player->GetXPos() < _player->GetGameBoard()->boardWidth - _player->GetWidth()) {
			state = this;
		}
		else {
			state = new NoMoveState(_player);
		}
		break;
	case VK_LEFT:
		break;
	case 0: //No key pressed.
		state = new NoMoveState(_player);
		break;
	default: //Unhandled key pressed.
		break;
	}
	return state;
}

void MoveRightState::enter()
{
	_player->SetXVelocity(-(_player->GetGameBoard()->squareWidth / 20));
	//_player->SetYVelocity(20.0f); //gb->squareHeight / 20
}

void MoveRightState::update()
{
}
