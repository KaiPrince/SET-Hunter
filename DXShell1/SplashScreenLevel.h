#pragma once
#include "GameLevel.h"
#include "DrawableAsset.h"

#include <chrono>

class SplashScreenLevel :
	public GameLevel
{
	std::chrono::steady_clock::time_point startupTime;
	double opacity;

	DrawableAsset* artwork;
public:
	SplashScreenLevel();
	virtual ~SplashScreenLevel();

	// Inherited via GameLevel
	virtual void Load() override;
	virtual void Unload() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput() override;
};

