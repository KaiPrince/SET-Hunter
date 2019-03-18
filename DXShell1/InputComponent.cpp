#include "InputComponent.h"
#include "Actor.h"
#include "GameBoard.h"
#include "GameObject.h"
#include "GraphicsLocator.h"


void PlayerInputComponent::HandleInput() {

	float xVelocity = 0;
	float yVelocity = 0;
	if (GetKeyState(VK_UP) & 0x8000)
	{
		// UP arrow key is down.
		yVelocity += object->GetGameBoard()->squareHeight / 10;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		// DOWN arrow key is down.
		yVelocity += -(object->GetGameBoard()->squareHeight / 10);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		// RIGHT arrow key is down.
		xVelocity += -(object->GetGameBoard()->squareWidth / 20); //TODO: make this accelerrate rather than constant speed.
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// LEFT arrow key is down.
		xVelocity += object->GetGameBoard()->squareWidth / 20; //TODO: make this accelerrate rather than constant speed.
	}

	object->SetXVelocity(xVelocity);
	object->SetYVelocity(yVelocity);
}

void ClickableInputComponent::HandleInput() {

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0) {
		if (this->object->ContainsPoint(GraphicsLocator::GetGraphics()->MouseX, GraphicsLocator::GetGraphics()->MouseY))
		{
			//Notify observers
			object->Notify();

			//TODO: Save event information somewhere.
		}
	}
}