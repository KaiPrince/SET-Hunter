#pragma once
#include "Visitor.h"

class Actor;
class GameObject;

/*
Class Name: InputComponent
Purpose: This class encapsulates the input handling behaviour of a game object.
	This class will respond to inputs from the player.
*/
class InputComponent : public Visitor
{
protected:
	GameObject* object;
public:
	InputComponent(GameObject* object) {
		this->object = object;
	}

	virtual ~InputComponent() {}

	virtual void HandleInput() = 0;

	// Inherited via Visitor
	virtual void visit(GameObject * gameobject) override {}
	virtual void visit(Actor * actor) override {}
};

class NullInputComponent : public InputComponent
{
public:
	NullInputComponent() : InputComponent(nullptr) {}
	virtual ~NullInputComponent() {}

	void HandleInput() override { /* Do nothing. */ }
private:

};

/*
Class Name: PlayerInputComponent
Purpose: This class models the input handling strategy of a player.
	The player's velocity will be adjusted based on inputs from the user.
*/
class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(GameObject* object) : InputComponent(object) {}
	virtual ~PlayerInputComponent() {}

	virtual void HandleInput();

private:

};

class ClickableInputComponent : public InputComponent 
{
public:
	ClickableInputComponent(GameObject* object) : InputComponent(object) {}
	virtual ~ClickableInputComponent() {}

	virtual void HandleInput();

	// Inherited via InputComponent
	virtual void visit(GameObject * gameobject) override;
	virtual void visit(Actor * actor) override;
};

