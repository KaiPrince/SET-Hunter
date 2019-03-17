#include "InputComponent.h"
#include "Actor.h"
#include "GameBoard.h"
#include "GameObject.h"



InputComponent::InputComponent(Actor* actor) 
{
	this->actor = actor;
}


InputComponent::~InputComponent()
{
}


PlayerInputComponent::PlayerInputComponent(Actor* actor) : InputComponent(actor)
{
}

PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::HandleInput() {

	float xVelocity = 0;
	float yVelocity = 0;
	if (GetKeyState(VK_UP) & 0x8000)
	{
		// UP arrow key is down.
		yVelocity += actor->GetGameBoard()->squareHeight / 10;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		// DOWN arrow key is down.
		yVelocity += -(actor->GetGameBoard()->squareHeight / 10);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		// RIGHT arrow key is down.
		xVelocity += -(actor->GetGameBoard()->squareWidth / 20); //TODO: make this accelerrate rather than constant speed.
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// LEFT arrow key is down.
		xVelocity += actor->GetGameBoard()->squareWidth / 20; //TODO: make this accelerrate rather than constant speed.
	}

	actor->SetXVelocity(xVelocity);
	actor->SetYVelocity(yVelocity);
}