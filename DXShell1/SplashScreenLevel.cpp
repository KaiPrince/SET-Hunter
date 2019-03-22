#include "SplashScreenLevel.h"
#include "GameController.h"
#include "MainMenuLevel.h"

#include "Audio.h"
#include "Graphics.h"



SplashScreenLevel::SplashScreenLevel()
{
}


SplashScreenLevel::~SplashScreenLevel()
{
}

void SplashScreenLevel::Load()
{
	opacity = 1.0f;
	startupTime = std::chrono::steady_clock::now();

	artwork =  new SpriteSheet(L"Assets\\spy_hunter_artwork.bmp", gfx); //_assetFactory->CreateDrawableAsset(DrawableAsset::SPY_HUNTER_ART);
	

	//Play Theme Song
	AudioLocator::GetAudio()->PlayThemeSong();
}

void SplashScreenLevel::Unload()
{
}

void SplashScreenLevel::Update()
{
}

void SplashScreenLevel::Render()
{
	using namespace std::chrono;

	duration<double> time_span = duration_cast<duration<double>>(steady_clock::now() - startupTime);

	double elapsedTimeInS = time_span.count();

	if (elapsedTimeInS >= 0 && elapsedTimeInS <= 3.0) {
		//fade in
		opacity = 1.0 - ((1.0 / 3) * elapsedTimeInS);
	}
	else if (elapsedTimeInS >= 3.1 && elapsedTimeInS <= 4.0) {
		//hold
		opacity = 0;
	}
	else if (elapsedTimeInS >= 4.1 && elapsedTimeInS <= 6.0)
	{
		//fade out
		opacity = (1.0 / 2) * (elapsedTimeInS - 4.1);
	}
	else if (elapsedTimeInS >= 6.1)
	{
		//switch to main menu.
		GameController::QueuedNextLevel = new MainMenuLevel();
	}
	else
	{
		//Do nothing.
	}
	const float screenH = GraphicsLocator::GetGraphics()->Window_Height;
	const float screenW = GraphicsLocator::GetGraphics()->Window_Width;
	//artwork->Draw(0.0f, 0.0f, screenW, screenH);
	artwork->Draw();
	

	//Draw white filter
	GraphicsLocator::GetGraphics()->FillRect(0.0f, 0.0f, screenW, screenH, 1.0f, 1.0f, 1.0f, opacity);
	

}


void SplashScreenLevel::HandleInput()
{
}
