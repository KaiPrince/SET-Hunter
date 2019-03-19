#include "InputComponent.h"
#include "Actor.h"
#include "GameBoard.h"
#include "GameObject.h"
#include "GraphicsLocator.h"

#include <chrono>
#include <random>
#include <math.h>


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
		xVelocity += object->GetGameBoard()->squareWidth / 20; //TODO: make this accelerrate rather than constant speed.
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// LEFT arrow key is down.
		xVelocity += -(object->GetGameBoard()->squareWidth / 20); //TODO: make this accelerrate rather than constant speed.
	}

	object->SetXVelocity(xVelocity);
	object->SetYVelocity(yVelocity);
}

void ClickableInputComponent::HandleInput() {

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0) {
		if (object->ContainsPoint((float)GraphicsLocator::GetGraphics()->MouseX, (float)GraphicsLocator::GetGraphics()->MouseY))
		{
			//Notify observers
			object->Notify();

			//TODO: Save event information somewhere.
		}
	}
}

void ClickableInputComponent::visit(GameObject * gameobject)
{

}

void ClickableInputComponent::visit(Actor * actor)
{
}

void StayOnRoadInputComponent::HandleInput()
{
	GameBoard* gb = object->GetGameBoard();
	int rowAbove = gb->FindSquare(object->GetXPos(), object->GetYPos())->GetGbY() - 3;
	Square* leftRoadSquare = gb->FindLeftRoadSquare(rowAbove);

	/*unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> dice_randomRoadOffset(0, gb->roadWidth);*/


	Square* targetSquare = gb->GetSquare(leftRoadSquare->GetGbX() + (gb->roadWidth / 2), rowAbove);

	float newXVelocity = (targetSquare->GetXPos() - object->GetXPos()) / 100.0f;
	const float movingThreshold = 0.0f; //Threshold for future tuning.

	if (fabs(newXVelocity) < movingThreshold) {
		newXVelocity = 0.0f;
	}

	object->SetXVelocity(newXVelocity);

}
