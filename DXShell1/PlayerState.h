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

	virtual PlayerState* handleInput(int key) = 0;
	virtual void enter() = 0;
	virtual void update() = 0;
};

/*
Class Name: NoMoveState
Purpose: In this state, the car will not move.
*/
class NoMoveState : public PlayerState
{
private:
public:
	NoMoveState(Player* player) : PlayerState(player) {}
	virtual ~NoMoveState() {}

	//Transition states
	virtual PlayerState* handleInput(int key);

	//Set up velocity
	virtual void enter();

	//Update player position
	virtual void update();
};

/*
Class Name: MoveUpState
Purpose: In this state, the car accelerates up its top height position, 
	and then transitions back to the NoMoveState.
*/
class MoveUpState : public PlayerState
{
private:
public:
	MoveUpState(Player* player) : PlayerState(player) {}
	virtual ~MoveUpState() {}

	//Transition states
	virtual PlayerState* handleInput(int key);

	//Set up velocity
	virtual void enter();

	virtual void update();
};

/*
Class Name: MoveDownState
Purpose: In this state, the car deccelerates back to the bottom row,
	and then transitions back to the NoMoveState.
*/
class MoveDownState : public PlayerState
{
private:
public:
	MoveDownState(Player* player) : PlayerState(player) {}
	virtual ~MoveDownState() {}

	virtual PlayerState* handleInput(int key);

	//Set up velocity
	virtual void enter();

	virtual void update();
};


/*
Class Name: MoveLeftState
Purpose: In this state, the player moves left at a constant rate until it reaches the left edge of the board.
	Note: Collision detection is handled elsewhere.
*/
class MoveLeftState : public PlayerState
{
private:
public:
	MoveLeftState(Player* player) : PlayerState(player) {}
	virtual ~MoveLeftState() {}

	virtual PlayerState* handleInput(int key);

	//Set up velocity
	virtual void enter();

	virtual void update();
};

/*
Class Name: MoveRightState
Purpose: In this state, the player moves right at a constant rate until it reaches the right edge of the board.
*/
class MoveRightState : public PlayerState
{
private:
public:
	MoveRightState(Player* player) : PlayerState(player) {}
	virtual ~MoveRightState() {}

	virtual PlayerState* handleInput(int key);

	//Set up velocity
	virtual void enter();

	virtual void update();
};