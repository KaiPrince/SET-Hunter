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
	Actor* actor;
public:
	InputComponent(Actor* actor);
	virtual ~InputComponent();

	virtual void HandleInput() = 0;
};

class Actor;
/*
Class Name: PlayerInputComponent
Purpose: This class models the input handling strategy of a player.
	The player's velocity will be adjusted based on inputs from the user.
*/
class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(Actor* actor);
	~PlayerInputComponent();

	virtual void HandleInput();

private:

};


