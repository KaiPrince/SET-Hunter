#pragma once

class GameObject; //Forward declare

/*
Class Name: CollisionResolutionStrategy
Purpose: This abstract class is defines the interface for CollisionResolution classes.
	Collision Resolution Strategies are given to PhysicsComponents, to be called on by the GameWorld after a collision 
	is detected.
	Their responsibility is to define the new X/Y position and velocity; as well as cause state changes or update object
	properties.
	NOTE: This class family uses the Strategy and Visitor design patterns.
*/
class CollisionResolutionStrategy
{
protected:
	GameObject* _obj;

public:
	CollisionResolutionStrategy(GameObject* obj) { this->_obj = obj; }
	virtual ~CollisionResolutionStrategy() {}

	virtual void CollideWith(GameObject* collidedObj);


	// Inherited via Visitor
	virtual void visit(class DeathTouchCollisionStrategy* component) = 0;
	virtual void visit(class PlayerCollisionStrategy* component) = 0;
	virtual void visit(class NullCollisionStrategy* component) = 0;
	virtual void visit(class CoinCollisionStrategy* component) {}
	//virtual void visit(class CollisionResolutionStrategy* component) = 0;


	// Inherited via VisitorComponent
	virtual void accept(CollisionResolutionStrategy* visitor) = 0;

};

/*
Class Name: NullCollisionStrategy
Purpose: This class adheres to the Null Object Pattern.
*/
class NullCollisionStrategy : public CollisionResolutionStrategy
{
	static NullCollisionStrategy* _singleton;
public:
	NullCollisionStrategy();
	~NullCollisionStrategy() {}

	// Inherited via CollisionResolutionStrategy
	virtual void CollideWith(GameObject* collidedObj) override {}

	// Inherited via Visitor
	//virtual void visit(CollisionResolutionStrategy* component) override {}
	virtual void visit(DeathTouchCollisionStrategy* component) override {}
	virtual void visit(NullCollisionStrategy* component) override {}
	virtual void visit(PlayerCollisionStrategy* component) override {}

	// Inherited via VisitorComponent
	virtual void accept(CollisionResolutionStrategy* visitor) override { visitor->visit(this); }

	static NullCollisionStrategy* GetInstance() {
		if (!_singleton) {
			_singleton = new NullCollisionStrategy();
		}
		return _singleton;
	}

private:



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


	// Inherited via Visitor
	virtual void visit(DeathTouchCollisionStrategy* component);
	virtual void visit(PlayerCollisionStrategy* component) override {}
	virtual void visit(NullCollisionStrategy* component) override {}
	virtual void visit(CoinCollisionStrategy* component) override;


	// Inherited via VisitorComponent
	virtual void accept(CollisionResolutionStrategy* visitor) override { visitor->visit(this); }

private:

};

/*
Class Name: DeathTouchCollisionStrategy
Purpose: Anything that touches me dies.
*/
class DeathTouchCollisionStrategy : public CollisionResolutionStrategy
{
public:
	DeathTouchCollisionStrategy(GameObject* obj) : CollisionResolutionStrategy(obj) {}
	~DeathTouchCollisionStrategy() {}



	// Inherited via Visitor
	virtual void visit(DeathTouchCollisionStrategy* component) override {}
	virtual void visit(NullCollisionStrategy* component) override {}
	virtual void visit(PlayerCollisionStrategy* component) override {}

	// Inherited via VisitorComponent
	virtual void accept(CollisionResolutionStrategy* visitor) override { visitor->visit(this); }
private:

};



class RocketCollisionStrategy : public DeathTouchCollisionStrategy
{
public:
	RocketCollisionStrategy(GameObject* obj) : DeathTouchCollisionStrategy(obj) {}
	~RocketCollisionStrategy() {}



	// Inherited via Visitor
	virtual void visit(DeathTouchCollisionStrategy* component) override {}
	virtual void visit(NullCollisionStrategy* component) override {}
	virtual void visit(PlayerCollisionStrategy* component) override;

	// Inherited via VisitorComponent
	virtual void accept(CollisionResolutionStrategy* visitor) override { visitor->visit(this); }
};

class CoinCollisionStrategy : public CollisionResolutionStrategy {

public:
	CoinCollisionStrategy(GameObject* obj) : CollisionResolutionStrategy(obj) {}
	~CoinCollisionStrategy() {}



	// Inherited via Visitor
	virtual void visit(DeathTouchCollisionStrategy* component) override {}
	virtual void visit(NullCollisionStrategy* component) override {}
	virtual void visit(PlayerCollisionStrategy* component) override;

	// Inherited via VisitorComponent
	virtual void accept(CollisionResolutionStrategy* visitor) override { visitor->visit(this); }
};