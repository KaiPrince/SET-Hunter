#pragma once
#include <ctime>
#include <chrono>

class Actor;

/*
Class Name: ActorState
Purpose: This class encapulates all the state-dependent behaviour of an Actor class.
*/
class ActorState
{
protected:
	Actor* _actor;


public:
	ActorState(Actor* player);
	virtual ~ActorState();

	virtual void Enter() = 0;
	virtual void Leave() = 0;

	virtual ActorState* HandleInput() = 0;
	virtual ActorState* Update() = 0;
	virtual ActorState* Draw() = 0;

	enum ActorStates
	{
		ALIVE_STATE, DEAD_STATE
	};
protected: //This is only here because the ActorStates var must be below its definition.
	ActorState::ActorStates type;

public:
	ActorState::ActorStates GetType() { return type; }
};

/*
Class Name: AliveState
Purpose: This class represents the state of the actor being alive.
	In this state, the player can move, receive damage and interact with other game objects.
*/
class AliveState : public ActorState
{
private:
	std::chrono::time_point<std::chrono::steady_clock> scoreTimer;
	std::chrono::duration<int, std::milli> offRoadDelayCountdown; //Score won't update
	std::chrono::duration<int, std::milli> invincibilityCountdown; //Can't die
public:
	AliveState(Actor* actor) : ActorState(actor) { type = ALIVE_STATE; }
	virtual ~AliveState() {}

	//Transition states
	virtual ActorState* HandleInput() override;

	//Set up
	virtual void Enter() override;

	//Update player properties
	virtual ActorState* Update() override;

	//On transition out..
	virtual void Leave() override;


	// Inherited via ActorState
	virtual ActorState * Draw() override;

};

/*
Class Name: DeadState
Purpose: This class represents the state of the actor being dead.
	In this state, the player cannot move, receive damage, or interact with other game objects.
*/
class DeadState : public ActorState
{
private:
	std::clock_t timeOfDeath;
public:
	DeadState(Actor* actor) : ActorState(actor) { type = DEAD_STATE; }
	virtual ~DeadState();

	//Transition states
	virtual ActorState* HandleInput();

	//Player died
	virtual void Enter();

	//Update player properties
	virtual ActorState* Update();

	//On transition out..
	virtual void Leave() override;


	// Inherited via ActorState
	virtual ActorState * Draw() override;

};

class NullState : public ActorState {
public:

	NullState(Actor* actor) : ActorState(actor) {}
	~NullState() {}

	// Inherited via ActorState
	virtual void Enter() override {}
	virtual void Leave() override {}
	virtual ActorState * HandleInput() override { return this; }
	virtual ActorState * Update() override { return this; }
	virtual ActorState * Draw() override { return this; }
};