#pragma once
#include "GameLevel.h"
#include <ctime>
#include <chrono>


class Level1 : public GameLevel
{
	//std::chrono::time_point<std::chrono::steady_clock> roadTimer;
	std::clock_t roadTimer;
	int roadShift; //Shifts the road to the right

	void placePlants(GameBoard* gb);
	void placePlants(GameBoard* gb, int row);
public:
	void Load() override;
	void Unload() override;
	void Update() override;
	void Render() override;

};
