#include "GameObject.h"



GameObject::GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
	float xVelocity, float yVelocity)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->sprite = sprite; //TODO: null check
	this->gb = gameboard; //TODO: null check

	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;
}


GameObject::~GameObject()
{
}

void GameObject::Draw() {

	sprite->Draw(x, y, width, height);
}

void GameObject::Update() {
	this->_physicsComponent->Update();
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