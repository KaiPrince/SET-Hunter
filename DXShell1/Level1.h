#pragma once
#include "GameLevel.h"


class Level1 : public GameLevel
{
	void placePlants(GameBoard* gb);
public:
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;

};
