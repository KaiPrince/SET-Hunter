#include "GameObjectState.h"
#include "GameObject.h"
#include "GameController.h"



GameObjectState::GameObjectState(GameObject* object)
{
	this->_object = object;
}

GameObjectState::~GameObjectState()
{
}

GameObjectState * AliveState::HandleInput()
{
	GameObjectState* nextState = this;

	_object->GetInputComponent()->HandleInput();

	return nextState;
}

void AliveState::Enter()
{
	

	//Reset Sprite
	_object->SetSprite(_object->GetGameBoard()->GetAssetFactory()->GetAsset(DrawableAsset::CAR_SPRITE));

	//Reset ScoreTimer
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	scoreTimer = currentTime;


	//Notify observers
	_object->Notify();
}

GameObjectState* AliveState::Update()
{
	GameObjectState* nextState = this;

	_object->GetPhysicsComponent()->Update();

	//Get delta time
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	double elapsedTimeInMS = duration<double, std::milli>(currentTime - scoreTimer).count();

	//Transition States
	if (dynamic_cast<CollidablePhysicsComponent*>(_object->GetPhysicsComponent()) && static_cast<CollidablePhysicsComponent*>(_object->GetPhysicsComponent())->IsCollisionDetected()) {

		nextState = new DeadState(_object);
	}
	else {

		//Object on the road?
		GameBoardTile* currentSquare = _object->GetGameBoard()->FindSquare(_object->GetXPos() + (_object->GetWidth() / 2), _object->GetYPos());
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


		//Reset Score timer
		scoreTimer = currentTime;
	}

	return nextState;
}

void AliveState::Leave()
{
}

GameObjectState * AliveState::Draw()
{
	_object->GetSprite()->Draw(_object->GetXPos(), _object->GetYPos(), _object->GetWidth(), _object->GetHeight()); //TODO: This is terrible. Use renderComponent?
	return this;
}

DeadState::~DeadState()
{
}

GameObjectState * DeadState::HandleInput()
{
	//Don't actually handle input. You're dead.
	return this;
}

void DeadState::Enter()
{
	timeOfDeath = clock();

	//-1 life
	GameController::SetLives(GameController::GetLives() - 1);

	//Death sprite
	_object->SetSprite(_object->GetGameBoard()->GetAssetFactory()->GetAsset(DrawableAsset::EXPLOSION_SPRITE));

	//Notify observers (Road stops scrolling.)
	_object->Notify();
}

GameObjectState* DeadState::Update()
{
	GameObjectState* nextState = this;

	std::clock_t currentTime = clock();
	double elapsedTimeInMS = std::chrono::duration<double, std::milli>(currentTime - timeOfDeath).count();

	//Revive player after 3 seconds
	const int timeUntilRevivalInMS = 3000;
	if (elapsedTimeInMS >= timeUntilRevivalInMS) {
		nextState = new InvincibleState(_object); // AliveState(_object);
	}

	return nextState;
}

void DeadState::Leave()
{
}

GameObjectState * DeadState::Draw()
{
	_object->GetSprite()->Draw(_object->GetXPos(), _object->GetYPos(), _object->GetWidth(), _object->GetHeight()); //TODO: This is terrible. Use renderComponent?
	return this;
}

GameObjectState * NullState::HandleInput()
{
	_object->GetInputComponent()->HandleInput();
	return this;
}

GameObjectState * NullState::Update()
{
	_object->GetPhysicsComponent()->Update();
	return this;
}

GameObjectState * NullState::Draw()
{
	_object->GetSprite()->Draw(_object->GetXPos(), _object->GetYPos(), _object->GetWidth(), _object->GetHeight()); //TODO: This is terrible. Use renderComponent?
	return this;
}

GameObjectState * InvincibleState::HandleInput()
{
	_object->GetInputComponent()->HandleInput();
	return this;
}

void InvincibleState::Enter()
{
	//Reset position
	float centerOfBoardX = _object->GetGameBoard()->boardWidth * _object->GetGameBoard()->squareWidth / 2;
	float bottomOfBoardY = _object->GetGameBoard()->boardHeight * _object->GetGameBoard()->squareHeight;

	_object->SetXPos(centerOfBoardX - _object->GetWidth());
	_object->SetYPos(bottomOfBoardY);

	//Reset Sprite
	_object->SetSprite(_object->GetGameBoard()->GetAssetFactory()->GetAsset(DrawableAsset::CAR_SPRITE));

	//Reset ScoreTimer
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	timeOfRevival = currentTime;

	invincibilityCountdown = duration<float, std::milli>(3000.0f);

	//Notify observers (Road continues scrolling.)
	_object->Notify();

}

GameObjectState * InvincibleState::Update()
{
	GameObjectState* nextState = this;

	_object->GetPhysicsComponent()->Update();

	//Get delta time
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	float elapsedTimeInMS = GameController::GetDeltaTime(); //duration<float, std::milli>(currentTime - timeOfRevival).count();


	//Decrement invincibility timer
	if (invincibilityCountdown.count() > 0) {

		invincibilityCountdown -= duration<float, std::milli>(elapsedTimeInMS);
	}

	if (invincibilityCountdown.count() <= 0) {
		//Transition to Alive State
		nextState = new AliveState(_object);
	}

	//Reset Score timer
	timeOfRevival = currentTime;

	return nextState;
}

void InvincibleState::Leave()
{
}

GameObjectState * InvincibleState::Draw()
{
	if (invincibilityCountdown.count() > 0) {
		//Draw for 500 milliseconds, and then don't draw for 500 milliseconds. Repeat.

		//Draw: 3000 -> 2500, 2000 -> 1500, 1000 -> 500
		//Don't Draw: 2500 -> 2000, 1500 -> 1000

		//x / 500 = 6; when x = 3000
		//x / 500 = 5; when x = 2500
		//x / 500 = 4.2 when x = 2100
		//x / 500 = 4; when x = 2000
		//x / 500 = 3; when x = 1500
		//x / 500 = 2; when x = 1000
		//x / 500 = 1; when x = 500

		//Draw when x / 500: 5->6, 3->4, 1->2

		const float flashDuration = 500.0f; //TODO: tweak this for better feel.
		const float interval = invincibilityCountdown.count() / flashDuration;
		if ((interval < 6 && interval > 5) || (interval < 4 && interval > 3) || (interval < 2 && interval > 1)) {
			_object->GetSprite()->Draw(_object->GetXPos(), _object->GetYPos(), _object->GetWidth(), _object->GetHeight());  //TODO: This is terrible. Use renderComponent?
		}
		else {

		}

	}
	else
	{
		_object->GetSprite()->Draw(_object->GetXPos(), _object->GetYPos(), _object->GetWidth(), _object->GetHeight());  //TODO: This is terrible. Use renderComponent?
	}

	return this;
}
