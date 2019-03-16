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


	return nextState;
}

void AliveState::enter()
{
	//Reset position
	float centerOfBoardX = _player->GetGameBoard()->boardWidth * _player->GetGameBoard()->squareWidth / 2;
	float bottomOfBoardY = _player->GetGameBoard()->boardHeight * _player->GetGameBoard()->squareHeight;

	_player->SetXPos(centerOfBoardX - _player->GetWidth());
	_player->SetYPos(bottomOfBoardY);

	//Reset Sprite
	_player->SetSprite(_player->GetGameBoard()->GetAssetFactory()->CreateDrawableAsset(DrawableAsset::CAR_SPRITE));
}

PlayerState* AliveState::update()
{
	PlayerState* nextState = nullptr;

	//Collision detection
	float newXPos = _player->GetXPos() - _player->GetXVelocity();
	float newYPos = _player->GetYPos() - _player->GetYVelocity();

	Square* nextSquare = _player->GetGameBoard()->FindSquare(newXPos + (_player->GetWidth() / 2), newYPos);
	if (nextSquare != nullptr && nextSquare->IsCollidable()) {
		nextState = new DeadState(_player);
	}

	//Advance position
	_player->SetXPos(newXPos);
	_player->SetYPos(newYPos);

	return nextState;
}

DeadState::~DeadState()
{
}

PlayerState * DeadState::handleInput()
{
	return nullptr;
}

void DeadState::enter()
{
	timeOfDeath = clock();

	//Death sprite
	_player->SetSprite(_player->GetGameBoard()->GetAssetFactory()->CreateDrawableAsset(DrawableAsset::EXPLOSION_SPRITE));
}

PlayerState* DeadState::update()
{
	PlayerState* nextState = nullptr;

	std::clock_t currentTime = clock();
	double elapsedTimeInMS = std::chrono::duration<double, std::milli>(currentTime - timeOfDeath).count();

	//Revive player after 3 seconds
	const int timeUntilRevivalInMS = 3000;
	if (elapsedTimeInMS >= timeUntilRevivalInMS) {
		nextState = new AliveState(_player);
	}

	return nextState;
}
