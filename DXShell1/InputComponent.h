#pragma once
#include "Visitor.h"

class GameObject;
class GameObject;

/*
Class Name: InputComponent
Purpose: This class encapsulates the input handling behaviour of a game object.
	This class will respond to inputs from the player.
*/
class InputComponent
{
protected:
	GameObject* object;
public:
	InputComponent(GameObject* object) {
		this->object = object;
	}

	virtual ~InputComponent() {}

	virtual void HandleInput() = 0;

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

/*
Class Name: StayOnRoadInputComponent
Purpose: This class autopilots its actor to remain on the road.
*/
class StayOnRoadInputComponent : public InputComponent
{
public:
	StayOnRoadInputComponent(GameObject* object) : InputComponent(object) {}
	virtual ~StayOnRoadInputComponent() {}

	virtual void HandleInput() override;
};

class ClickableInputComponent : public InputComponent 
{
public:
	ClickableInputComponent(GameObject* object) : InputComponent(object) {}
	virtual ~ClickableInputComponent() {}

	virtual void HandleInput();

};

