#include "PhysicsComponent.h"
#include "GameObject.h"
#include "GameBoard.h"
#include "ActorState.h"
#include "Square.h"



PhysicsComponent::PhysicsComponent(GameObject* obj, GameBoard* world)
{
	this->obj = obj;
	this->world = world;
}


PhysicsComponent::~PhysicsComponent()
{
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

void PlayerPhysicsComponent::Update() {
	//Reset Flags
	this->collisionObject = nullptr;

	//Collision detection
	float newXPos = obj->GetXPos() - obj->GetXVelocity();
	float newYPos = obj->GetYPos() - obj->GetYVelocity();

	Square* nextSquare = world->FindSquare(newXPos + (obj->GetWidth() / 2), newYPos);
	if (nextSquare != nullptr && nextSquare->IsCollidable()) {
		//Set collision flag
		this->collisionObject = nextSquare;
	}

	//Check X Bounds
	if (newXPos <= (world->boardWidth * world->squareWidth) - obj->GetWidth() && newXPos >= 0) {
		//Advance position
		obj->SetXPos(newXPos);
	}


	//Check Y Bounds
	const float maxheight = (world->boardHeight * world->squareHeight) - (obj->GetHeight() * 2);
	const float minheight = (world->boardHeight * world->squareHeight) / 2;

	if (newYPos <= maxheight && newYPos >= minheight) {
		obj->SetYPos(newYPos);
	}
	else if (newYPos > maxheight) {
		obj->SetYPos(maxheight);
	}
	else if (newYPos < minheight)
	{
		obj->SetYPos(minheight);
	}
	else
	{
		//Out of bounds. Refuse to set.
	}

}