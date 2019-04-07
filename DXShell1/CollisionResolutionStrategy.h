#pragma once

class GameObject; //Forward declare

/*
Class Name: CollisionResolutionStrategy
Purpose: This abstract class is defines the interface for CollisionResolution classes.
	Collision Resolution Strategies are given to PhysicsComponents, to be called on by the GameWorld after a collision 
	is detected.
	Their responsibility is to define the new X/Y position and velocity; as well as cause state changes or update object
	properties.
	NOTE: This class family uses the Strategy design pattern.
*/
class CollisionResolutionStrategy
{
	GameObject* _obj;
public:
	CollisionResolutionStrategy(GameObject* obj);
	virtual ~CollisionResolutionStrategy();

	virtual void CollideWith(GameObject* obj) = 0;
};

/*
Class Name: NullCollisionStrategy
Purpose: This class adheres to the Null Object Pattern.
*/
class NullCollisionStrategy : public CollisionResolutionStrategy
{
public:
	NullCollisionStrategy();
	~NullCollisionStrategy() {}

private:


	// Inherited via CollisionResolutionStrategy
	virtual void CollideWith(GameObject* obj) override {}

};

/*
Class Name: PlayerCollisionStrategy
Purpose: This class defines what should happen when the Player collides with an object.
*/
class PlayerCollisionStrategy : public CollisionResolutionStrategy
{
public:
	PlayerCollisionStrategy(GameObject* obj) : CollisionResolutionStrategy(obj) {}
	~PlayerCollisionStrategy() {}

	virtual void CollideWith(GameObject* obj) override;

private:

};