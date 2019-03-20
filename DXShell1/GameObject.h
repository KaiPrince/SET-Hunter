#pragma once
#include "DrawableAsset.h"
//#include "GameBoard.h"
#include "Observer.h"
#include "Visitor.h"
#include "PhysicsComponent.h"

class GameBoard;

class GameObject : public Observable, public VisitorComponent
{
	PhysicsComponent* _physicsComponent;

protected:
	DrawableAsset* sprite;
	GameBoard* gb;

	float x;
	float y;
	float width;
	float height;

	float xVelocity;
	float yVelocity;

	bool isCollidable;
public:
	GameObject(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
		float xVelocity = 0.0f, float yVelocity = 0.0f);
	virtual ~GameObject();

	//------- Getters and Setters for Properties ------------
	float GetXPos() { return x; }
	float GetYPos() { return y; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetXVelocity() { return xVelocity; }
	float GetYVelocity() { return yVelocity; }
	bool IsCollidable() { return isCollidable; }

	virtual void SetXPos(float x) { this->x = x; }
	virtual void SetYPos(float y) { this->y = y; }
	void SetWidth(float width) { this->width = width; }
	void SetHeight(float height) { this->height = height; }
	void SetXVelocity(float xVelocity) { this->xVelocity = xVelocity; }
	void SetYVelocity(float yVelocity) { this->yVelocity = yVelocity; }
	void SetCollidable(bool isCollidable) { this->isCollidable = isCollidable; }



	//-------- Getters and Setters for Components ------------
	GameBoard* GetGameBoard() { return gb; }
	PhysicsComponent* GetPhysicsComponent() { return _physicsComponent; }
	void SetGameBoard(GameBoard* gb) { this->gb = gb; }
	void SetPhysicsComponent(PhysicsComponent* physics) { _physicsComponent = physics; }
	void SetSprite(DrawableAsset* sprite) { this->sprite = sprite; }


	//-------- Utility Functions -----------------------------
	bool ContainsPoint(float x, float y);


	virtual void Update();

	virtual void Draw();

	// Inherited via VisitorComponent
	virtual void accept(Visitor & visitor) override;
};

class NullGameObject : public GameObject
{
public:
	NullGameObject() : GameObject(0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr) {}
	NullGameObject(float x, float y, float width, float height) : GameObject(x, y, width, height, nullptr, nullptr) {}
	~NullGameObject() {}

	virtual void Update() override {}
	virtual void Draw() override {}

	virtual void accept(Visitor & visitor) override {}

private:

};