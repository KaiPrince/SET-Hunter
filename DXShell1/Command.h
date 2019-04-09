#pragma once

#include <queue>

/*
Class Name: Command
Purpose: This interface follows the Command Pattern.
*/
class Command
{
public:
	Command();
	virtual ~Command();

	virtual void Execute() = 0;
};

/*
Class Name: CommandQueue
Purpose: This class implements a simple command queue.
	It will accept commands and execute them when called upon (possible at a later time).
*/
class CommandQueue
{

	std::queue<Command*> _commandQueue; //TODO: make std::sharedptr
public:

	void QueueCommand(Command* command) {
		_commandQueue.push(command);
	}

	void ProcessAllCommands()
	{
		while (!_commandQueue.empty()) {
			Command* nextCommand = _commandQueue.front();
			nextCommand->Execute();
			_commandQueue.pop();

			delete nextCommand; //TODO: replace with shared+ptr
		}
	}

};


class GameObject;
class GameWorld;
class AssetFactory;

class SpawnRocket : public Command
{
	GameObject* _actor;
	GameWorld* _world;
	AssetFactory* _assetFactory;
public:
	SpawnRocket(GameObject* actor, GameWorld* world, AssetFactory* assetFactory) : Command() {
		this->_actor = actor;
		this->_world = world;
		this->_assetFactory = assetFactory;
	}
	~SpawnRocket() {}

	virtual void Execute() override;

};

class RemoveGameObjectFromWorldCommand : public Command
{
	GameObject* _object;
	GameWorld* _world;

public:
	RemoveGameObjectFromWorldCommand(GameObject* object, GameWorld* world) : Command() {
		this->_object = object;
		this->_world = world;
	}
	~RemoveGameObjectFromWorldCommand() {}

	virtual void Execute() override;
};