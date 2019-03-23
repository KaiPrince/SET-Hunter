#include "InputComponent.h"
#include "GameObject.h"
#include "GameBoard.h"
#include "GameObject.h"
#include "Graphics.h"

#include <chrono>
#include <random>
#include <math.h>


void PlayerInputComponent::HandleInput() {

	const float MaxXVelocity = object->GetGameBoard()->squareHeight / 25;
	const float MaxYVelocity = MaxXVelocity;
	const float AccelerationSpeed = MaxXVelocity * 0.5f;
	const float DeccelerationSpeed = MaxXVelocity * 0.1f;

	float xVelocity = object->GetXVelocity(); //0;
	float yVelocity = object->GetYVelocity(); //0;

	//Deacceleration. Velocities work their way back to 0.
	if (abs(xVelocity) <= DeccelerationSpeed) xVelocity = 0;
	if (abs(yVelocity) <= DeccelerationSpeed) yVelocity = 0;
	if (xVelocity < 0) xVelocity += DeccelerationSpeed; else if (xVelocity > 0) xVelocity -= DeccelerationSpeed;
	if (yVelocity < 0) yVelocity += DeccelerationSpeed; else if (yVelocity > 0) yVelocity -= DeccelerationSpeed;

	//Accelerration 
	if (GetKeyState(VK_UP) & 0x8000)
	{
		// UP arrow key is down.
		yVelocity += AccelerationSpeed;
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		// DOWN arrow key is down.
		yVelocity += -AccelerationSpeed;
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		// RIGHT arrow key is down.
		xVelocity += AccelerationSpeed;
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		// LEFT arrow key is down.
		xVelocity += -AccelerationSpeed;
	}

	//Enforce Max Velocity cap.
	if (xVelocity < -MaxXVelocity) xVelocity = -MaxXVelocity; else if (xVelocity > MaxXVelocity) xVelocity = MaxXVelocity;
	if (yVelocity < -MaxXVelocity) yVelocity = -MaxYVelocity; else if (yVelocity > MaxYVelocity) yVelocity = MaxYVelocity;

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

void StayOnRoadInputComponent::HandleInput()
{
	GameBoard* gb = object->GetGameBoard();
	int rowAbove = gb->FindSquare(object->GetXPos(), object->GetYPos())->GetGbY() - 3;
	GameBoardTile* leftRoadSquare = gb->FindLeftRoadSquare(rowAbove);

	/*unsigned seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> dice_randomRoadOffset(0, gb->roadWidth);*/


	GameBoardTile* targetSquare = gb->GetSquare(leftRoadSquare->GetGbX() + (gb->roadWidth / 2), rowAbove);

	float newXVelocity = (targetSquare->GetXPos() - object->GetXPos()) / 100.0f;
	const float movingThreshold = 0.0f; //Threshold for future tuning.

	if (fabs(newXVelocity) < movingThreshold) {
		newXVelocity = 0.0f;
	}

	object->SetXVelocity(newXVelocity);

}
