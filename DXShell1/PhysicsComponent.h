#pragma once

class GameObject;
class GameBoard;

/*
Class Name: PhysicsComponent
Purpose: This class encapsulates the physics of a game object.
	This class will be used to handle collision detection and updates to the position of a game object.
*/
class PhysicsComponent
{
protected:
	GameObject* obj; 
	GameBoard* world;

	GameObject* collisionObject; //TODO: convert to GameObject
public:
	PhysicsComponent(GameObject* obj, GameBoard* world);
	virtual ~PhysicsComponent();

	virtual GameObject* GetCollisionObject() {
		return collisionObject;
	}

	virtual bool IsCollisionDetected() {
		if (collisionObject != nullptr) {
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void Update() = 0;
};

/*
Class Name: PlayerPhysicsComponent
Purpose: This class represents the movement (X and Y position) behaviour of the player.
	It will not allow the object to move off screen.
	When it collides with a plant, a collision flag will be set.
*/
class PlayerPhysicsComponent : public PhysicsComponent
{
public:
	PlayerPhysicsComponent(GameObject* obj, GameBoard* world) : PhysicsComponent(obj, world) {}
	~PlayerPhysicsComponent();

	virtual void Update();

private:

};

