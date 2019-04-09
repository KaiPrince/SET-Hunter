#pragma once
#include "GameLevel.h"
class Level2 :
	public GameLevel, public Observer
{

	bool continueRoadScolling; //TODO: move this to GameBoard class?



public:
	Level2();
	virtual ~Level2();

	// Inherited via GameLevel
	virtual void Load() override;
	virtual void Unload() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput() override;


	void Notify(Observable* subject);
};
