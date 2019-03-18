#include "GameObject.h"
#include "AssetFactory.h"



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
		this->sprite = AssetFactory::_emptySprite;
	}

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

void GameObject::accept(Visitor & visitor)
{
	visitor.visit(this);
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