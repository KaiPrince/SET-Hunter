#pragma once
#include "GameLevel.h"
#include "PlayerState.h"


class Level1 : public GameLevel
{
	bool continueRoadScolling; //TODO: move this to GameBoard class?

public:
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;

};
