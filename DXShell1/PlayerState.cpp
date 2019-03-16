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
	return this;
}

void AliveState::enter()
{
}

void AliveState::update()
{
}

DeadState::~DeadState()
{
}

PlayerState * DeadState::handleInput()
{
	return this;
}

void DeadState::enter()
{
}

void DeadState::update()
{
}
