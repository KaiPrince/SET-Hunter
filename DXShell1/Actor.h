#pragma once
#include "GameObject.h"
#include "ActorState.h"
#include "InputComponent.h"
//#include "Visitor.h" //Note: Also inherits from GameObject.


class ActorState;
class InputComponent;

/*
Class Name: Actor
Purpose: This class is used to represent an actor in the game.
	It will manage position and state, handle input, update its position and state,
	and draw itself to the screen.
*/
class Actor :
	public GameObject
	//public VisitorComponent //Note: Also inherits from GameObject
{
protected:
	InputComponent* _inputComponent;

	ActorState* _state;
public:
	Actor(float x, float y, float width, float height, DrawableAsset* sprite, GameBoard* gameboard,
		float xVelocity = 0.0f, float yVelocity = 0.0f);
	virtual ~Actor();

	void UpdateState(ActorState * nextState);

	InputComponent* GetInputComponent() { return _inputComponent; }
	void SetInputComponent(InputComponent* input) { _inputComponent = input; }

	ActorState* GetState() { return _state; }
	//void ChangeState(ActorState* newState) {
	//	if (newState != nullptr) {
	//		//Change state

	//		delete _state;
	//		_state = newState;

	//		_state->enter();
	//	}
	//}

	virtual void HandleInput();

	virtual void Update();

	void Draw() override;

	// Inherited via VisitorComponent
	virtual void accept(Visitor & visitor) override;
};

class NullActor : public Actor
{
public:
	NullActor() : Actor(0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr) {}
	~NullActor() {}

	virtual void HandleInput() override {}
	virtual void Update() override {}
	virtual void Draw() override {}

	virtual void accept(Visitor & visitor) override {}


private:

};
