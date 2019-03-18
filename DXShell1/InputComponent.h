#pragma once

class Actor;

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

	virtual ~InputComponent();

	virtual void HandleInput() = 0;
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
	~PlayerInputComponent() {}

	virtual void HandleInput();

private:

};

class ClickableInputComponent : public InputComponent 
{
public:
	ClickableInputComponent(GameObject* object) : InputComponent(object) {}
	~ClickableInputComponent() {}

	virtual void HandleInput();
};

