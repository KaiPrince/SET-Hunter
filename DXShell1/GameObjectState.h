#pragma once
#include <ctime>
#include <chrono>

class GameObject;

/*
Class Name: GameObjectState
Purpose: This class encapulates all the state-dependent behaviour of an GameObject class.
*/
class GameObjectState
{
protected:
	GameObject* _object;


public:
	GameObjectState(GameObject* player);
	virtual ~GameObjectState();

	virtual void Enter() = 0;
	virtual void Leave() = 0;

	virtual GameObjectState* HandleInput() = 0;
	virtual GameObjectState* Update() = 0;
	virtual GameObjectState* Draw() = 0;

	enum ActorStates
	{
		ALIVE_STATE, DEAD_STATE, NULL_STATE, INVINCIBLE_STATE, SHOOT_PLAYER_STATE
	};

public:
	virtual GameObjectState::ActorStates GetType() = 0;
};

/*
Class Name: AliveState
Purpose: This class represents the state of the object being alive.
	In this state, the player can move, receive damage and interact with other game objects.
*/
class AliveState : public GameObjectState
{
private:
	std::chrono::time_point<std::chrono::steady_clock> scoreTimer;
	std::chrono::duration<int, std::milli> offRoadDelayCountdown; //Score won't update
public:
	AliveState(GameObject* object) : GameObjectState(object) {}
	virtual ~AliveState() {}

	virtual GameObjectState::ActorStates GetType() { return GameObjectState::ALIVE_STATE; }

	//Transition states
	virtual GameObjectState* HandleInput() override;

	//Set up
	virtual void Enter() override;

	//Update player properties
	virtual GameObjectState* Update() override;

	//On transition out..
	virtual void Leave() override;


	// Inherited via GameObjectState
	virtual GameObjectState * Draw() override;

};

/*
Class Name: DeadState
Purpose: This class represents the state of the object being dead.
	In this state, the player cannot move, receive damage, or interact with other game objects.
*/
class DeadState : public GameObjectState
{
private:
	std::clock_t timeOfDeath;
public:
	DeadState(GameObject* object) : GameObjectState(object) { }
	virtual ~DeadState();

	virtual GameObjectState::ActorStates GetType() { return GameObjectState::DEAD_STATE; }

	//Transition states
	virtual GameObjectState* HandleInput();

	//Player died
	virtual void Enter();

	//Update player properties
	virtual GameObjectState* Update();

	//On transition out..
	virtual void Leave() override;


	// Inherited via GameObjectState
	virtual GameObjectState * Draw() override;

};

/*
Class Name: NullState
Purpose: This class implements the Null Object pattern.
	Note that this class actually does make some standard calls to its object.
*/
class NullState : public GameObjectState {
public:

	NullState(GameObject* object) : GameObjectState(object) {}
	~NullState() {}

	virtual GameObjectState::ActorStates GetType() { return GameObjectState::NULL_STATE; }

	// Inherited via GameObjectState
	virtual void Enter() override {}
	virtual void Leave() override {}
	virtual GameObjectState * HandleInput() override;
	virtual GameObjectState * Update() override;
	virtual GameObjectState * Draw() override;
};

/*
Class Name: InvincibleState
Purpose: This class represents the state of the object being alive.
	In this state, the player can move, receive damage and interact with other game objects.
*/
class InvincibleState : public GameObjectState
{
private:
	std::chrono::time_point<std::chrono::steady_clock> timeOfRevival;
	std::chrono::duration<float, std::milli> invincibilityCountdown; //Can't die
public:
	InvincibleState(GameObject* object) : GameObjectState(object) {}
	virtual ~InvincibleState() {}

	virtual GameObjectState::ActorStates GetType() { return GameObjectState::INVINCIBLE_STATE; }

	//Transition states
	virtual GameObjectState* HandleInput() override;

	//Set up
	virtual void Enter() override;

	//Update player properties
	virtual GameObjectState* Update() override;

	//On transition out..
	virtual void Leave() override;


	// Inherited via GameObjectState
	virtual GameObjectState * Draw() override;

};

class ShootPlayerState : public GameObjectState
{
private:
	std::chrono::time_point<std::chrono::steady_clock> timeOfRevival;
	std::chrono::duration<float, std::milli> invincibilityCountdown; //Can't die
public:
	ShootPlayerState(GameObject* object) : GameObjectState(object) {}
	virtual ~ShootPlayerState() {}

	virtual GameObjectState::ActorStates GetType() { return GameObjectState::SHOOT_PLAYER_STATE; }

	//Transition states
	virtual GameObjectState* HandleInput() override;

	//Set up
	virtual void Enter() override;

	//Update player properties
	virtual GameObjectState* Update() override;

	//On transition out..
	virtual void Leave() override;


	// Inherited via GameObjectState
	virtual GameObjectState* Draw() override;

private:

};
