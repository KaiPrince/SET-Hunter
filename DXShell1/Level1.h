#pragma once
#include "GameLevel.h"
#include "GameObjectState.h"

class GameObject;
class TextAsset;

class Level1 : public GameLevel, public Observer
{

	std::chrono::steady_clock::time_point playerLostLastLife_Time;

	bool continueRoadScolling; //TODO: move this to GameBoard class?
	GameObject* scoreHUD;
	TextAsset* scoreAsset;

public:
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;
	void HandleInput() override;

	void Notify(Observable* subject);
};
