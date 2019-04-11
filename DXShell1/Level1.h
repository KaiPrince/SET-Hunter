#pragma once
#include "GameLevel.h"
#include "GameObjectState.h"


class Level1 : public GameLevel, public Observer
{

public:
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;
	void HandleInput() override;

	void Notify(Observable* subject);
};
