#pragma once
#include "GameObject.h"
#include "ActorState.h"

class ActorState;

/*
Class Name: Actor
Purpose: This class is used to represent an actor in the game.
	It will manage position and state, handle input, update its position and state,
	and draw itself to the screen.
*/
class Actor :
	public GameObject
{
protected:

	ActorState* _state;
public:
	Actor(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
		float xVelocity = 0.0f, float yVelocity = 0.0f);
	virtual ~Actor();


	ActorState* GetState() { return _state; }
	void ChangeState(ActorState* newState) {
		if (newState != nullptr) {
			//Change state

			delete _state;
			_state = newState;

			_state->enter();
		}
	}

	virtual void HandleInput();

	virtual void Update();
};

