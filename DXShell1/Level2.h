#pragma once
#include "GameLevel.h"
#include "Command.h"
#include <chrono>

class Level2 : public GameLevel, public Observer, public CommandQueue
{
	std::vector<GameObject*> _enemies;

	std::chrono::duration<float, std::milli> _spawnEnemyCountdown;

public:
	Level2() : GameLevel() {}
	virtual ~Level2() {}

	// Inherited via GameLevel
	virtual void Load() override;
	virtual void Unload() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput() override;


	void Notify(Observable* subject);
};

