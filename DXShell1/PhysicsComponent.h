#pragma once
#include <vector>

class GameObject;
class GameBoard;
class GameWorld;
class CollisionResolutionStrategy;

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

	GameObject* _hitbox;
	CollisionResolutionStrategy* _collisionStrategy;
	
	std::vector<GameObject*> _collisionObjects;

	void DetectCollisions();
	void DetectCollisionsAs(GameObject* obj);
	bool CheckIntersection(GameObject* obj1, GameObject* obj2);

public:
	PhysicsComponent(GameObject* obj, GameWorld* world);
	PhysicsComponent(GameObject* obj, GameWorld* world, GameObject* hitbox, CollisionResolutionStrategy* collisionStrategy);
	virtual ~PhysicsComponent();

	virtual CollisionResolutionStrategy* GetCollisionStrategy() { return _collisionStrategy; }
	virtual void SetCollisionStrategy(CollisionResolutionStrategy* collisionStrategy) { this->_collisionStrategy = collisionStrategy; }

	virtual std::vector<GameObject*> GetCollisionObjects() {
		return _collisionObjects;
	}

	virtual bool IsCollidable() = 0;
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

	virtual void Update() = 0; //NOTE: all derived classes must call on their decorated component.


};

/*
Class Name: NullPhysicsComponent
Purpose: This class adheres to the Null Object pattern.
*/
class NullPhysicsComponent : public PhysicsComponent
{
	static NullPhysicsComponent* _singleton;
public:
	NullPhysicsComponent();
	~NullPhysicsComponent() {}

	static NullPhysicsComponent* GetSingleton() {
		if (!_singleton) {
			_singleton = new NullPhysicsComponent();
		}
		return _singleton;
	}

	virtual void Update() override { /*Do nothing*/ }
	virtual bool IsCollidable() override { return false; }

private:

};

/*
Class Name: Collidable PhysicsComponent
Purpose: This component will detect collisions with all gameObjects in the world when Update() is called.
*/
class CollidablePhysicsComponent : public PhysicsComponent
{

public:
	CollidablePhysicsComponent(GameObject* obj, GameWorld* world);
	virtual ~CollidablePhysicsComponent() {}


	virtual bool IsCollidable() override { return true; }

	virtual void Update();

protected:
	virtual void ResetHitbox();
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

/*
Class Name: Hitbox
Purpose: This class literally is just a rectangle. 
GameObject could no longer be reused for this purpose, because of a circular instantiation that developed between
NullPhysicsComponent and NullGameObject.
*/
class Hitbox
{

	float x;
	float y;
	float width;
	float height;
public:
	Hitbox(float x, float y, float width, float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	Hitbox(GameObject& obj);
	~Hitbox() {}


	//------- Getters and Setters for Properties ------------
	float GetXPos() { return x; }
	float GetYPos() { return y; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }

	virtual void SetXPos(float x) { this->x = x; }
	virtual void SetYPos(float y) { this->y = y; }
	void SetWidth(float width) { this->width = width; }
	void SetHeight(float height) { this->height = height; }

private:

};