#include "GameObject.h"
#include "AssetFactory.h"
#include "GameObjectState.h"
#include "GameWorld.h"



GameObject::GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameWorld* gameworld,
	float xVelocity, float yVelocity)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	if (sprite != nullptr) {
		this->sprite = sprite;
	}
	else {
		this->sprite = AssetFactory::GetNullAsset();
	}

	this->world = gameworld; //TODO: null check

	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;

	this->_physicsComponent = NullPhysicsComponent::GetSingleton();

	this->_state = new NullState(this);
	this->_inputComponent = new NullInputComponent(); //Note: if this is ever changed to a singleton class, add delete safeguard in destructor.
}

GameObject::GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameWorld* gameworld, float xVelocity, float yVelocity, PhysicsComponent* physics, InputComponent* input, GameObjectState* state)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	if (sprite != nullptr) {
		this->sprite = sprite;
	}
	else {
		this->sprite = AssetFactory::GetNullAsset();
	}

	this->world = gameworld; //TODO: null check

	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;

	this->_physicsComponent = physics;

	this->_state = state;
	this->_inputComponent = input;
}


GameObject::~GameObject()
{
	delete _state;
	if (_physicsComponent != NullPhysicsComponent::GetSingleton()) delete _physicsComponent;
	delete _inputComponent;
}

void GameObject::Draw() {

	GameObjectState* nextState = _state->Draw();
	UpdateState(nextState);
	//sprite->Draw(x, y, width, height);
}

void GameObject::Update() {
	GameObjectState* nextState = _state->Update();
	UpdateState(nextState);
}



bool GameObject::ContainsPoint(float x, float y)
{
	if (x >= this->GetXPos() && x <= this->GetXPos() + this->GetWidth() &&
		y >= this->GetYPos() && y <= this->GetYPos() + this->GetHeight()) {
		return true;
	}
	else
	{
		return false;
	}
}

GameBoard* GameObject::GetGameBoard()
{
	return world->GetGameBoard();
}

void GameObject::UpdateState(GameObjectState* nextState) {
	if (nextState != nullptr && nextState != _state && nextState->GetType() != _state->GetType()) {
		//Change state
		_state->Leave();

		delete _state;
		_state = nextState;

		_state->Enter();
	}
}

void GameObject::HandleInput() {

	GameObjectState* nextState = _state->HandleInput();
	UpdateState(nextState);
}