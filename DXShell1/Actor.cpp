#include "Actor.h"



Actor::Actor(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
	float xVelocity, float yVelocity) : GameObject(x, y, width, height, sprite, gameboard,
		xVelocity, yVelocity)
{
	this->_state = new AliveState(this);
	this->_inputComponent = new NullInputComponent();
}


Actor::~Actor()
{
	delete _state;
}

void Actor::HandleInput() {

	ActorState* newState = _state->HandleInput();
	if (newState != nullptr && newState != _state) {
		//Change state
		_state->Leave();

		delete _state;
		_state = newState;

		_state->Enter();
	}
}

void Actor::Update() {
	ActorState* newState = _state->Update(); //Update position
	if (newState != nullptr && newState != _state) {
		//Change state
		_state->Leave();

		delete _state;
		_state = newState;

		_state->Enter();
	}
}

void Actor::Draw() {
	ActorState* newState = _state->Draw(); //Draw
	if (newState != nullptr && newState != _state) {
		//Change state
		_state->Leave();

		delete _state;
		_state = newState;

		_state->Enter();
	}
}

void Actor::accept(Visitor & visitor)
{
	visitor.visit(this);
}
