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
	this->_collisionStrategy = NullCollisionStrategy::GetInstance();

	//non-collidable by default.
	this->_hitbox = new NullGameObject(0, 0, obj->GetWidth(), obj->GetHeight());

	this->_hitboxInWorld = new NullGameObject(
		_obj->GetXPos() + _hitbox->GetXPos(),
		_obj->GetYPos() + _hitbox->GetYPos(),
		_hitbox->GetWidth(),
		_hitbox->GetHeight()
	);
	this->_hitboxInWorld->SetSprite(new AssetOutlineDecorator(_hitboxInWorld->GetSprite())); //DEBUG
}

PhysicsComponent::PhysicsComponent(GameObject* obj, GameWorld* world, GameObject* hitbox, CollisionResolutionStrategy* collisionStrategy)
{
	this->_obj = obj;
	this->_world = world;
	this->_hitbox = hitbox;
	this->_collisionStrategy = collisionStrategy;

	this->_hitboxInWorld = nullptr;
}


PhysicsComponent::~PhysicsComponent()
{

	delete _hitbox;
	delete _hitboxInWorld;

}

void PhysicsComponent::DetectCollisions()
{
	GetHitboxInWorld()->Draw(); //DEBUG

	DetectCollisionsAs(GetHitboxInWorld());
}

/*
This function checks all GameBoard squares and GameObjects for a collision with a given object.
Sets the class collision flags if collision is detected.
*/
void PhysicsComponent::DetectCollisionsAs(GameObject* obj) {
	//Reset Flags
	this->_collisionObjects.clear();

	//Collision detection

	for each (GameObject* objectInGameWorld in _world->GetGameObjects())
	{
		if (objectInGameWorld != _obj && objectInGameWorld->GetPhysicsComponent()->IsCollidable() && Intersects(obj, objectInGameWorld->GetPhysicsComponent()->GetHitboxInWorld())) {
			//Set collision flag
			this->_collisionObjects.push_back(objectInGameWorld);

		}
	}
}

bool PhysicsComponent::Intersects(GameObject * obj1, GameObject * obj2)
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

	CollidablePhysicsComponent::Update();


	//... In this case, we advance the object regardless of its collision status.

	float newXPos = _obj->GetXPos();
	float newYPos = _obj->GetYPos();

	float smudgeX = 3.0f; //These numbers account for edge case errors with pixel math.
	float smudgeY = 3.0f;

	//Check X Bounds
	const float maxXPos = GraphicsLocator::GetGraphics()->Window_Width - _obj->GetWidth() - smudgeX;
	const float minXPos = 0 + smudgeX;

	if (newXPos > maxXPos) {
		_obj->SetXPos(maxXPos);
	}
	else if (newXPos < minXPos) {
		_obj->SetXPos(minXPos);
	}
	else {
		//Object is within acceptable range.
	}

	//Check Y Bounds
	const float maxheight = GraphicsLocator::GetGraphics()->Window_Height - (_obj->GetHeight() * 2) - smudgeY;
	const float minheight = GraphicsLocator::GetGraphics()->Window_Height / 2;

	if (newYPos > maxheight) {
		_obj->SetYPos(maxheight);
	}
	else if (newYPos < minheight)
	{
		_obj->SetYPos(minheight);
	}
	else
	{
		//Object is within acceptable range.
	}



}

CollidablePhysicsComponent::CollidablePhysicsComponent(GameObject * obj, GameWorld * world) : PhysicsComponent(obj, world)
{
	this->ChangeHitbox(0, 0, obj->GetWidth(), obj->GetHeight());
}

GameObject* PhysicsComponent::GetHitboxInWorld()
{
	if (_hitboxInWorld == nullptr) {
		//Lazy initialization.
		this->_hitboxInWorld = new NullGameObject(
			_obj->GetXPos() + _hitbox->GetXPos(),
			_obj->GetYPos() + _hitbox->GetYPos(),
			_hitbox->GetWidth(),
			_hitbox->GetHeight()
		);

	}
	_hitboxInWorld->SetXPos(_obj->GetXPos() + _hitbox->GetXPos());
	_hitboxInWorld->SetYPos(_obj->GetYPos() + _hitbox->GetYPos());
	_hitboxInWorld->SetWidth(_hitbox->GetWidth());
	_hitboxInWorld->SetHeight(_hitbox->GetHeight());

	return _hitboxInWorld;
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
	DetectCollisions();

	float newXPos = _obj->GetXPos() + _obj->GetXVelocity();
	float newYPos = _obj->GetYPos() - _obj->GetYVelocity(); // Because y axis is inverted.

	_obj->SetXPos(newXPos);
	_obj->SetYPos(newYPos);

	//Remove myself if I leave the screen.
	if (ObjectOffScreen(_obj))
	{
		_world->QueueCommand(new RemoveGameObjectFromWorldCommand(_obj, _world));
	}
}

bool CollidablePhysicsComponent::ObjectOffScreen(GameObject* obj)
{
	return obj->GetXPos() < 0 - obj->GetWidth() || obj->GetXPos() > GraphicsLocator::GetGraphics()->Window_Width
		|| obj->GetYPos() < 0 - obj->GetHeight() || obj->GetYPos() > GraphicsLocator::GetGraphics()->Window_Height;
}

void CollidablePhysicsComponent::ResetHitbox()
{
	this->ChangeHitbox(
		0, 
		0, 
		_obj->GetWidth(), 
		_obj->GetHeight()
	);
}

NullPhysicsComponent::NullPhysicsComponent() : 
	PhysicsComponent(nullptr, nullptr, new GameObject(0,0,0,0,nullptr,nullptr,0,0,nullptr,nullptr,nullptr), 
		NullCollisionStrategy::GetInstance())
{
}
