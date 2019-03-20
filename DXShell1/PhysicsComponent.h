#pragma once
#include "Visitor.h"
#include <vector>

class GameObject;
class GameBoard;
class GameWorld;
class Actor;

/*
Class Name: PhysicsComponent
Purpose: This class encapsulates the physics of a game object.
	This class will be used to handle collision detection and updates to the position of a game object.
*/
class PhysicsComponent : public Visitor
{
protected:
	GameObject* _obj; 
	GameWorld* _world;

	std::vector<GameObject*> _collisionObjects;


	void DetectCollisions();
	void DetectCollisionsAs(GameObject* obj);
	bool CheckIntersection(GameObject* obj1, GameObject* obj2);
public:
	PhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~PhysicsComponent();

	virtual std::vector<GameObject*> GetCollisionObjects() {
		return _collisionObjects;
	}

	virtual bool IsCollisionDetected() {
		if (!_collisionObjects.empty()) {
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void Update() = 0; //NOTE: all derived classes must call on their decorated component.

	// Inherited via Visitor
	virtual void visit(GameObject * gameobject) override;
	virtual void visit(Actor * actor) override;
};

class NullPhysicsComponent : public PhysicsComponent
{
public:
	NullPhysicsComponent() : PhysicsComponent(nullptr, nullptr) {}
	~NullPhysicsComponent() {}

	void Update() override { /*Do nothing*/ }


	// Inherited via Visitor
	void visit(GameObject * gameobject) override { /*Do nothing*/ }
	void visit(Actor * actor) override { /*Do nothing*/ }
private:

};

/*
Class Name: PlayerPhysicsComponent
Purpose: This class represents the movement (X and Y position) behaviour of the player.
	It will not allow the object to move off screen.
	When it collides with a plant, a collision flag will be set.
*/
class PlayerPhysicsComponent : public PhysicsComponent
{
	GameObject* _hitbox;

	virtual void ChangeHitbox(float x, float y, float width, float height);
public:
	PlayerPhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~PlayerPhysicsComponent();

	virtual void Update();

private:

};

class CollidablePhysicsComponent : public PhysicsComponent
{
	GameObject* _hitbox;
public:
	CollidablePhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~CollidablePhysicsComponent() {}

	virtual void ChangeHitbox(float x, float y, float width, float height);

	virtual void Update();

};

