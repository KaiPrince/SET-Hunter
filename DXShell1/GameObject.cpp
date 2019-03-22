#include "GameObject.h"
#include "AssetFactory.h"
#include "GameObjectState.h"



GameObject::GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
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

	this->gb = gameboard; //TODO: null check

	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;

	this->_physicsComponent = new NullPhysicsComponent();

	this->_state = new NullState(this);
	this->_inputComponent = new NullInputComponent();
}


GameObject::~GameObject()
{
	delete _state;
}

void GameObject::Draw() {

	GameObjectState* nextState = _state->Draw();
	UpdateState(nextState);
	//sprite->Draw(x, y, width, height);
}

void GameObject::accept(Visitor & visitor)
{
	visitor.visit(this);
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

void GameObject::UpdateState(GameObjectState* nextState) {
	if (nextState != nullptr && nextState != _state) {
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