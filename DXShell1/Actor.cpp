#include "Actor.h"



Actor::Actor(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
	float xVelocity, float yVelocity) : GameObject(x, y, width, height, sprite, gameboard,
		xVelocity, yVelocity)
{
	this->_state = new NullState(this);
	this->_inputComponent = new NullInputComponent();
}


Actor::~Actor()
{
	delete _state;
}

void Actor::UpdateState(ActorState* nextState) {
	if (nextState != nullptr && nextState != _state) {
		//Change state
		_state->Leave();

		delete _state;
		_state = nextState;

		_state->Enter();
	}
}

void Actor::HandleInput() {

	ActorState* nextState = _state->HandleInput();
	UpdateState(nextState);
}

void Actor::Update() {
	ActorState* nextState = _state->Update(); //Update position
	UpdateState(nextState);
}

void Actor::Draw() {
	ActorState* nextState = _state->Draw(); //Draw
	UpdateState(nextState);
}

void Actor::accept(Visitor & visitor)
{
	visitor.visit(this);
}
