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

	artwork =  new SpriteSheet(L"Assets\\spy_hunter_artwork.bmp", gfx); //_assetFactory->GetAsset(DrawableAsset::SPY_HUNTER_ART);
	

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
	const double fadeInTime = 3.0;
	const double holdTime = 2.0;
	const double fadeOutTime = 2.0;

	const double minOpacity = 0.0;
	const double maxOpacity = 1.0;



	using namespace std::chrono;

	duration<double> time_span = duration_cast<duration<double>>(steady_clock::now() - startupTime);

	double elapsedTimeInS = time_span.count();

	if (elapsedTimeInS >= minOpacity && elapsedTimeInS <= fadeInTime) {
		//fade in
		opacity = maxOpacity - ((maxOpacity / fadeInTime) * elapsedTimeInS);
	}
	else if (elapsedTimeInS > fadeInTime && elapsedTimeInS <= fadeInTime + holdTime) {
		//hold
		opacity = minOpacity;
	}
	else if (elapsedTimeInS > fadeInTime + holdTime && elapsedTimeInS <= fadeInTime + holdTime + fadeOutTime)
	{
		//fade out
		opacity = (maxOpacity / fadeOutTime) * (elapsedTimeInS - (fadeInTime + holdTime));
	}
	else if (elapsedTimeInS > fadeInTime + holdTime + fadeOutTime)
	{
		//switch to main menu.
		GameController::QueuedNextLevel = new MainMenuLevel();
	}
	else
	{
		//Do nothing.
	}

	const float screenH = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Height);
	const float screenW = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Width);
	//artwork->Draw(0.0f, 0.0f, screenW, screenH);
	artwork->Draw();
	

	//Draw white filter
	GraphicsLocator::GetGraphics()->FillRect(0.0f, 0.0f, screenW, screenH, 1.0f, 1.0f, 1.0f, static_cast<float>(opacity));
	

}


void SplashScreenLevel::HandleInput()
{
}
