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

	//Reset ScoreTimer
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	scoreTimer = currentTime;

	//Start invincibility countdown 
	invincibilityCountdown = duration<int, std::milli>(3000);

	//Notify observers
	_actor->Notify();
}

ActorState* AliveState::update()
{
	ActorState* nextState = nullptr;

	_actor->GetPhysicsComponent()->Update();

	//Get delta time
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	double elapsedTimeInMS = duration<double, std::milli>(currentTime - scoreTimer).count();

	//Transition States
	if (_actor->IsCollidable() && ((CollidablePhysicsComponent*)_actor->GetPhysicsComponent())->IsCollisionDetected()) {

		if (invincibilityCountdown.count() <= 0) {
		nextState = new DeadState(_actor);
		}
	}
	else {

		//Object on the road?
		Square* currentSquare = _actor->GetGameBoard()->FindSquare(_actor->GetXPos() + (_actor->GetWidth() / 2), _actor->GetYPos());
		if (currentSquare != nullptr && currentSquare->GetTerrain()->GetType() == DrawableAsset::ROAD_TERRAIN) {


		//Accumulate Score
			if (offRoadDelayCountdown.count() <= 0) {
				const double pointsPerMS = 1;
				GameController::SetScore(GameController::GetScore() + static_cast<unsigned int>(pointsPerMS * elapsedTimeInMS));
			}
			else {
				offRoadDelayCountdown -= duration<int, std::milli>(static_cast<int>(elapsedTimeInMS));
			}




		}
		else {

			//Start off-road delay.
			offRoadDelayCountdown = duration<int, std::milli>(3000);
		}

		//Decrement invincibility timer
		if (invincibilityCountdown.count() > 0) {

			invincibilityCountdown -= duration<int, std::milli>(static_cast<int>(elapsedTimeInMS));
		}

		//Reset Score timer
		scoreTimer = currentTime;
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
