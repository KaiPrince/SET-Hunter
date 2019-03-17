#pragma once
#include "AssetFactory.h"
#include "GameBoard.h"
#include "Observer.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

class GameObject : public Observable
{
	PhysicsComponent* _physicsComponent;
	InputComponent* _inputComponent;

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
	InputComponent* GetInputComponent() { return _inputComponent; }
	void SetGameBoard(GameBoard* gb) { this->gb = gb; }
	void SetPhysicsComponent(PhysicsComponent* physics) { _physicsComponent = physics; }
	void SetInputComponent(InputComponent* input) { _inputComponent = input; }
	void SetSprite(DrawableAsset* sprite) { this->sprite = sprite; }


	//-------- Utility Functions -----------------------------
	bool ContainsPoint(float x, float y);


	virtual void Update();

	virtual void Draw();
};

