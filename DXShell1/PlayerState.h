#pragma once

class Player;

/*
Class Name: PlayerState
Purpose: This class encapulates all the behaviour and data for the state of a player class.
*/
class PlayerState
{
protected:
	Player* _player;

public:
	PlayerState(Player* player);
	virtual ~PlayerState();

	virtual PlayerState* handleInput() = 0;
	virtual void enter() = 0;
	virtual void update() = 0;
};

/*
Class Name: AliveState
Purpose: This class represents the state of the actor being alive.
	In this state, the player can move, receive damage and interact with other game objects.
*/
class AliveState : public PlayerState
{
private:
public:
	AliveState(Player* player) : PlayerState(player) {}
	virtual ~AliveState() {}

	//Transition states
	virtual PlayerState* handleInput();

	//Set up
	virtual void enter();

	//Update player properties
	virtual void update();

};

/*
Class Name: DeadState
Purpose: This class represents the state of the actor being dead.
	In this state, the player cannot move, receive damage, or interact with other game objects.
*/
class DeadState : public PlayerState
{
private:
public:
	DeadState(Player* player) : PlayerState(player) {}
	virtual ~DeadState();

	//Transition states
	virtual PlayerState* handleInput();

	//Player died
	virtual void enter();

	//Update player properties
	virtual void update();


};