#include "PhysicsComponent.h"
#include "GameObject.h"
#include "GameBoard.h"
#include "GameObjectState.h"
#include "GameBoardTile.h"
#include "GameWorld.h"

#include "DrawableAsset.h"

#include "CollisionResolutionStrategy.h"

NullPhysicsComponent* NullPhysicsComponent::_singleton;


PhysicsComponent::PhysicsComponent(GameObject* obj, GameWorld* world)
{
	this->_obj = obj;
	this->_world = world;
	this->_collisionStrategy = new NullCollisionStrategy();

	//non-collidable by default.
	//this->_hitbox = new NullGameObject(obj->GetXPos(), obj->GetYPos(), obj->GetWidth(), obj->GetHeight());
	this->_hitbox = new GameObject(0.0f, 0.0f, 0.0f, 0.0f, new AssetOutlineDecorator(AssetFactory::GetNullAsset()), world);
	world->AddUIObject(this->_hitbox);
}

PhysicsComponent::PhysicsComponent(GameObject* obj, GameWorld* world, GameObject* hitbox, CollisionResolutionStrategy* collisionStrategy)
{
	this->_obj = obj;
	this->_world = world;
	this->_hitbox = hitbox;
	this->_collisionStrategy = collisionStrategy;
}


PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::DetectCollisions()
{
	DetectCollisionsAs(_obj);
}

/*
This function checks all GameBoard squares and GameObjects for a collision with a given object.
Sets the class collision flags if collision is detected.
*/
void PhysicsComponent::DetectCollisionsAs(GameObject* obj) {
	//Reset Flags
	this->_collisionObjects.clear();

	//Collision detection

	//GameBoard* gb = _world->GetGameBoard();
	//for (int column = 0; column < gb->boardWidth; column++)
	//{
	//	for (int row = 0; row < gb->boardHeight; row++)
	//	{
	//		GameBoardTile* thisSquare = gb->squares[column][row];
	//		if (thisSquare->GetPhysicsComponent()->IsCollidable() && CheckIntersection(obj, thisSquare)) {

	//			//Set collision flag
	//			this->_collisionObjects.push_back(thisSquare);
	//		}
	//	}
	//}

	for each (GameObject* objectInGameWorld in _world->GetGameObjects())
	{
		if (objectInGameWorld != _obj && objectInGameWorld->GetPhysicsComponent()->IsCollidable() && CheckIntersection(obj, objectInGameWorld)) {
			//Set collision flag
			this->_collisionObjects.push_back(objectInGameWorld);

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

PlayerPhysicsComponent::PlayerPhysicsComponent(GameObject * obj, GameWorld * world) : CollidablePhysicsComponent(obj, world)
{
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

void PlayerPhysicsComponent::Update() {

	float newXPos = _obj->GetXPos() + _obj->GetXVelocity();
	float newYPos = _obj->GetYPos() - _obj->GetYVelocity(); // Because y axis is inverted.


	//Create clone gameObject with altered hitbox.
	//float oneThirdWidth = _obj->GetWidth() / 3;
	//float heightDifference = _obj->GetHeight() / 5;
	//ChangeHitbox(_obj->GetXPos() + oneThirdWidth, _obj->GetYPos() + heightDifference, oneThirdWidth, _obj->GetHeight() - heightDifference);

	//CollidablePhysicsComponent::Update();
	ResetHitbox();
	DetectCollisionsAs(_hitbox);


	//... In this case, we advance the object regardless of its collision status.

	//Check X Bounds
	if (newXPos <= (_world->GetGameBoard()->boardWidth * _world->GetGameBoard()->squareWidth) - _obj->GetWidth() && newXPos >= 0) {
		//Advance position
		_obj->SetXPos(newXPos);
	}

	//Check Y Bounds
	const float maxheight = (_world->GetGameBoard()->boardHeight * _world->GetGameBoard()->squareHeight) - (_obj->GetHeight() * 2);
	const float minheight = (_world->GetGameBoard()->boardHeight * _world->GetGameBoard()->squareHeight) / 2;

	if (newYPos <= maxheight && newYPos >= minheight) {
		_obj->SetYPos(newYPos);
	}
	else if (newYPos > maxheight) {
		_obj->SetYPos(maxheight);
	}
	else if (newYPos < minheight)
	{
		_obj->SetYPos(minheight);
	}
	else
	{
		//Out of bounds. Refuse to set.
	}



}

CollidablePhysicsComponent::CollidablePhysicsComponent(GameObject * obj, GameWorld * world) : PhysicsComponent(obj, world)
{
	this->ChangeHitbox(obj->GetXPos(), obj->GetYPos(), obj->GetWidth(), obj->GetHeight());
}

void PhysicsComponent::ChangeHitbox(float x, float y, float width, float height)
{
	_hitbox->SetXPos(x);
	_hitbox->SetYPos(y);
	_hitbox->SetWidth(width);
	_hitbox->SetHeight(height);
}

void CollidablePhysicsComponent::Update()
{
	ResetHitbox();
	DetectCollisionsAs(_hitbox);

	float newXPos = _obj->GetXPos() + _obj->GetXVelocity();
	float newYPos = _obj->GetYPos() - _obj->GetYVelocity(); // Because y axis is inverted.

	_obj->SetXPos(newXPos);
	_obj->SetYPos(newYPos);
}

void CollidablePhysicsComponent::ResetHitbox()
{
	this->ChangeHitbox(
		_obj->GetXPos(), 
		_obj->GetYPos(), 
		_obj->GetWidth(), 
		_obj->GetHeight()
	);
}

Hitbox::Hitbox(GameObject& obj)
{
	this->x = obj.GetXPos();
	this->y = obj.GetYPos();
	this->width = obj.GetWidth();
	this->height = obj.GetHeight();

}

NullPhysicsComponent::NullPhysicsComponent() : 
	PhysicsComponent(nullptr, nullptr, new GameObject(0,0,0,0,nullptr,nullptr,0,0,nullptr,nullptr,nullptr), 
		NullCollisionStrategy::GetInstance())
{
}
