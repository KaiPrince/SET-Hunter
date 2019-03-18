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
	if (newState != nullptr) {
		//Change state

		delete _state;
		_state = newState;

		_state->enter();
	}
}

void Actor::Update() {
	ActorState* newState = _state->update(); //Update position
	if (newState != nullptr) {
		//Change state

		delete _state;
		_state = newState;

		_state->enter();
	}
}

void Actor::accept(Visitor & visitor)
{
	visitor.visit(this);
}
