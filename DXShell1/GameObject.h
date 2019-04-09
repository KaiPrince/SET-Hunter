#pragma once
#include "DrawableAsset.h"
//#include "GameBoard.h"
#include "Observer.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"
#include "GameObjectState.h"

class GameBoard;
class GameWorld;

/*
Class Name: GameObject
Purpose: This class represents any object with a position, and height and width.
	GameObjects can also have an InputComponent, PhysicsComponent, and State.
	GameObjects also have a sprite which represents them visually on the screen.
*/
class GameObject : public Observable
{
	PhysicsComponent* _physicsComponent;
	InputComponent* _inputComponent;

	GameObjectState* _state;

protected:
	DrawableAsset* sprite;
	GameWorld* world;

	float x;
	float y;
	float width;
	float height;

	float xVelocity;
	float yVelocity;

public:
	GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameWorld* gameworld,
		float xVelocity = 0.0f, float yVelocity = 0.0f);
	GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameWorld* gameworld,
		float xVelocity, float yVelocity, PhysicsComponent* physics, InputComponent* input, GameObjectState* state);
	virtual ~GameObject();

	//------- Getters and Setters for Properties ------------
	float GetXPos() { return x; }
	float GetYPos() { return y; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetXVelocity() { return xVelocity; }
	float GetYVelocity() { return yVelocity; }

	DrawableAsset* GetSprite() { return sprite; }

	virtual void SetXPos(float x) { this->x = x; }
	virtual void SetYPos(float y) { this->y = y; }
	void SetWidth(float width) { this->width = width; }
	void SetHeight(float height) { this->height = height; }
	void SetXVelocity(float xVelocity) { this->xVelocity = xVelocity; }
	void SetYVelocity(float yVelocity) { this->yVelocity = yVelocity; }



	//-------- Getters and Setters for Components ------------
	GameBoard* GetGameBoard();
	GameWorld* GetGameWorld() { return world; }
	PhysicsComponent* GetPhysicsComponent() { return _physicsComponent; }
	void SetGameWorld(GameWorld* world) { this->world = world; }
	void SetPhysicsComponent(PhysicsComponent* physics) { _physicsComponent = physics; }
	void SetSprite(DrawableAsset* sprite) { this->sprite = sprite; }


	void UpdateState(GameObjectState * nextState);

	InputComponent* GetInputComponent() { return _inputComponent; }
	void SetInputComponent(InputComponent* input) { _inputComponent = input; }

	GameObjectState* GetState() { return _state; }

	//-------- Utility Functions -----------------------------
	bool ContainsPoint(float x, float y);


	virtual void HandleInput();

	virtual void Update();

	virtual void Draw();
};

class NullGameObject : public GameObject
{
	static NullGameObject* _singleton;
public:
	NullGameObject() : GameObject(0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr) {}
	NullGameObject(float x, float y, float width, float height) : GameObject(x, y, width, height, nullptr, nullptr) {}
	~NullGameObject() {}

	virtual void Update() override {}
	virtual void Draw() override {}

	static NullGameObject* GetSingleton() {
		if (!_singleton) {
			_singleton = new NullGameObject();
		}
		return _singleton;
	}

private:

};