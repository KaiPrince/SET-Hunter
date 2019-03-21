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

	virtual ActorState* HandleInput() = 0;
	virtual void enter() = 0;
	virtual ActorState* update() = 0;

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
	std::chrono::duration<int, std::milli> offRoadDelayCountdown;
public:
	AliveState(Actor* actor) : ActorState(actor) { type = ALIVE_STATE; }
	virtual ~AliveState() {}

	//Transition states
	virtual ActorState* HandleInput();

	//Set up
	virtual void enter();

	//Update player properties
	virtual ActorState* update();

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
	virtual void enter();

	//Update player properties
	virtual ActorState* update();


};