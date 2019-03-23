#pragma once
#include "Visitor.h"
#include <vector>

class GameObject;
class GameBoard;
class GameWorld;

/*
Class Name: PhysicsComponent
Purpose: This class encapsulates the physics of a game object.
	This class will be used to handle collision detection and updates to the position of a game object.
*/
class PhysicsComponent
{
protected:
	GameObject* _obj; 
	GameWorld* _world;


public:
	PhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~PhysicsComponent();

	virtual bool IsCollidable() = 0;

	virtual void Update() = 0; //NOTE: all derived classes must call on their decorated component.

};

class NullPhysicsComponent : public PhysicsComponent
{
public:
	NullPhysicsComponent() : PhysicsComponent(nullptr, nullptr) {}
	~NullPhysicsComponent() {}

	virtual void Update() override { /*Do nothing*/ }
	virtual bool IsCollidable() override { return false; }

private:

};

class CollidablePhysicsComponent : public PhysicsComponent
{
	GameObject* _hitbox;
protected:
	std::vector<GameObject*> _collisionObjects;

	void DetectCollisions();
	void DetectCollisionsAs(GameObject* obj);
	bool CheckIntersection(GameObject* obj1, GameObject* obj2);

public:
	CollidablePhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~CollidablePhysicsComponent() {}

	virtual std::vector<GameObject*> GetCollisionObjects() {
		return _collisionObjects;
	}

	virtual bool IsCollidable() override { return true; }
	virtual bool IsCollisionDetected() {
		if (!_collisionObjects.empty()) {
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual GameObject* GetHitbox() { return _hitbox; }
	virtual void ChangeHitbox(float x, float y, float width, float height);

	virtual void Update();

};

/*
Class Name: PlayerPhysicsComponent
Purpose: This class represents the movement (X and Y position) behaviour of the player.
	It will not allow the object to move off screen.
	When it collides with a plant, a collision flag will be set.
*/
class PlayerPhysicsComponent : public CollidablePhysicsComponent
{

public:
	PlayerPhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~PlayerPhysicsComponent();

	virtual void Update();

private:

};