#include "ActorState.h"
#include "Actor.h"
#include "GameController.h"



ActorState::ActorState(Actor* player)
{
	this->_actor = player;
}

ActorState::~ActorState()
{
}

ActorState * AliveState::HandleInput()
{
	ActorState* nextState = nullptr;

	_actor->GetInputComponent()->HandleInput();


	return nextState;
}

void AliveState::enter()
{
	//Reset position
	float centerOfBoardX = _actor->GetGameBoard()->boardWidth * _actor->GetGameBoard()->squareWidth / 2;
	float bottomOfBoardY = _actor->GetGameBoard()->boardHeight * _actor->GetGameBoard()->squareHeight;

	_actor->SetXPos(centerOfBoardX - _actor->GetWidth());
	_actor->SetYPos(bottomOfBoardY);

	//Reset Sprite
	_actor->SetSprite(_actor->GetGameBoard()->GetAssetFactory()->CreateDrawableAsset(DrawableAsset::CAR_SPRITE));

	//Notify observers
	_actor->Notify();
}

ActorState* AliveState::update()
{
	ActorState* nextState = nullptr;

	_actor->GetPhysicsComponent()->Update();

	//Transition States
	if (_actor->GetPhysicsComponent()->IsCollisionDetected()) {
		nextState = new DeadState(_actor);
	}
	else {

		//Accumulate Score
		Square* currentSquare = _actor->GetGameBoard()->FindSquare(_actor->GetXPos(), _actor->GetYPos());
		if (currentSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {
			GameController::SetScore(GameController::GetScore() + 1);
		}
	}

	return nextState;
}

DeadState::~DeadState()
{
}

ActorState * DeadState::HandleInput()
{
	return nullptr;
}

void DeadState::enter()
{
	timeOfDeath = clock();

	//-1 life
	GameController::SetLives(GameController::GetLives() - 1);

	//Death sprite
	_actor->SetSprite(_actor->GetGameBoard()->GetAssetFactory()->CreateDrawableAsset(DrawableAsset::EXPLOSION_SPRITE));

	//Notify observers (Road stops scrolling.)
	_actor->Notify();
}

ActorState* DeadState::update()
{
	ActorState* nextState = nullptr;

	std::clock_t currentTime = clock();
	double elapsedTimeInMS = std::chrono::duration<double, std::milli>(currentTime - timeOfDeath).count();

	//Revive player after 3 seconds
	const int timeUntilRevivalInMS = 3000;
	if (elapsedTimeInMS >= timeUntilRevivalInMS) {
		nextState = new AliveState(_actor);
	}

	//TODO: add blinking before revivial
	//TODO: don't revive if no lives left.

	return nextState;
}
