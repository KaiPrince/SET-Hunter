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

void PhysicsComponent::visit(GameObject * gameobject)
{
}

void PhysicsComponent::visit(Actor * actor)
{
}

/*
This function checks all GameBoard squares and GameObjects for a collision with a given point.
*/
void PhysicsComponent::DetectCollisions() {
	//Reset Flags
	this->collisionObjects.clear();

	//Collision detection
	GameBoard* gb = world->GetGameBoard();
	for (int column = 0; column < gb->boardWidth; column++)
	{
		for (int row = 0; row < gb->boardHeight; row++)
		{
			Square* thisSquare = gb->squares[column][row];
			if (thisSquare->IsCollidable() && CheckIntersection(obj, thisSquare)) {

				//Set collision flag
				this->collisionObjects.push_back(thisSquare);
			}
		}
	}

	for each (GameObject* objectInGameWorld in world->GetGameObjects())
	{
		if (objectInGameWorld->IsCollidable() && CheckIntersection(obj, objectInGameWorld)) {
			//Set collision flag
			this->collisionObjects.push_back(objectInGameWorld);

		}
	}
}

bool PhysicsComponent::CheckIntersection(GameObject * obj1, GameObject * obj2)
{
	if (obj1->GetXPos() < obj2->GetXPos() + obj2->GetWidth() &&
		obj1->GetXPos() + obj1->GetWidth() > obj2->GetXPos() &&
		obj1->GetYPos() < obj2->GetYPos() + obj2->GetHeight() &&
		obj1->GetYPos() + obj1->GetHeight() > obj2->GetYPos()) {
		// collision detected!
		return true;
	}
	else {
		return false;
	}
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

void PlayerPhysicsComponent::Update() {

	float newXPos = obj->GetXPos() + obj->GetXVelocity();
	float newYPos = obj->GetYPos() - obj->GetYVelocity(); // Because y axis is inverted.


	//... In this case, we advance the object regardless of its collision status.

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

	//Set collision flags.
	DetectCollisions(); 

}

void CollidablePhysicsComponent::Update()
{
	DetectCollisions();
}
