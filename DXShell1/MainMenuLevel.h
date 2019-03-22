#pragma once
#include "GameLevel.h"
#include "Observer.h"
#include <vector>

/*
Class Name: MainMenu
Purpose: This GameLevel class will display a starting menu for the user to select actions from.
*/
class MainMenuLevel :
	public GameLevel, public Observer
{
	std::vector<GameObject*> mainMenu; //TODO: replace with composition object.
	Actor* StartButton; //TODO: move to MainMenu Type object or something.
	Actor* ExitButton;
	
	bool isPlayerDead; //TODO: this is terrible, just ask the state somehow.

public:
	MainMenuLevel();
	virtual ~MainMenuLevel();


	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;
	void HandleInput() override;

	void Notify(Observable* subject);
};

