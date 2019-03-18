#include "PhysicsComponent.h"
#include "GameObject.h"
#include "GameBoard.h"
#include "ActorState.h"
#include "Square.h"
#include "GameWorld.h"



PhysicsComponent::PhysicsComponent(GameObject* obj, GameWorld* world)
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

	GameObject* nextSquare = world->GetGameBoard()->FindSquare(newXPos + (obj->GetWidth() / 2), newYPos);
	if (nextSquare != nullptr && nextSquare->IsCollidable()) {
		//Set collision flag
		this->collisionObject = nextSquare;
	}

	for each (GameObject* objectInGameWorld in world->GetGameObjects())
	{
		if (objectInGameWorld->ContainsPoint(newXPos, newYPos) && objectInGameWorld->IsCollidable()) {
		//Set collision flag
		this->collisionObject = objectInGameWorld;

		}
	}

	//Check X Bounds
	if (newXPos <= (world->GetGameBoard()->boardWidth * world->GetGameBoard()->squareWidth) - obj->GetWidth() && newXPos >= 0) {
		//Advance position
		obj->SetXPos(newXPos);
	}


	//Check Y Bounds
	const float maxheight = (world->GetGameBoard()->boardHeight * world->GetGameBoard()->squareHeight) - (obj->GetHeight() * 2);
	const float minheight = (world->GetGameBoard()->boardHeight * world->GetGameBoard()->squareHeight) / 2;

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