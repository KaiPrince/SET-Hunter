#include "GameLevel.h"
#include "GameWorld.h"
#include "GameController.h"
#include "MainMenuLevel.h"
#include "Constants.h"
#include "Audio.h"

Graphics* GameLevel::gfx;
AssetFactory* GameLevel::_assetFactory;
GameWorld* GameLevel::world;
GameObject* GameLevel::player;


GameLevel::GameLevel()
{
	this->continueRoadScolling = true;

	this->_returnToMainMenu = false;
	this->_returnToMainMenuCountdown = std::chrono::duration<float, std::milli>(0.0f);
}

void GameLevel::InitScoreHUD()
{

	scoreAsset = static_cast<TextAsset*>(_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET));
	DrawableAsset* backgroundDecoratedAsset = new AssetBackgroundRectangleDecorator(scoreAsset);

	this->scoreHUD = new GameObject(0, 0, 100.0f, 50.0f, backgroundDecoratedAsset, world);
	world->AddUIObject(this->scoreHUD);
}

void GameLevel::ReturnToMainMenu_Update()
{

	if (_returnToMainMenu) {

		if (_returnToMainMenuCountdown.count() > 0) {

			//Decrement countdown
			using namespace std::chrono;
			float elapsedTimeInMS = GameController::GetDeltaTime();
			_returnToMainMenuCountdown -= duration<float, std::milli>(elapsedTimeInMS);

		}
		else {
			//Transition to main menu.
			GameController::QueuedNextLevel = new MainMenuLevel();
		}

	}
}

void GameLevel::ReturnToMainMenu_Detect()
{

	if (GameController::GetLives() <= 0 && _returnToMainMenu == false) {

		AudioLocator::GetAudio()->playSound(Audio::GAME_OVER_THEME);
		AudioLocator::GetAudio()->playSound(Audio::GAME_OVER_EFFECT);

		_returnToMainMenu = true;

		using namespace std::chrono;
		_returnToMainMenuCountdown = duration<float, std::milli>(kReturnToMainMenuCountdownTime);

	}
}


void GameLevel::ReturnToMainMenu_Render()
{
	if (_returnToMainMenuCountdown.count() > 0) {


		const float screenH = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Height);
		const float screenW = static_cast<float>(GraphicsLocator::GetGraphics()->Window_Width);

		float opacity = (1.0f - 0.0f) * ((kReturnToMainMenuCountdownTime - _returnToMainMenuCountdown.count()) / kReturnToMainMenuCountdownTime);

		//Draw white filter
		GraphicsLocator::GetGraphics()->FillRect(0.0f, 0.0f, screenW, screenH, 1.0f, 1.0f, 1.0f, opacity);

	}
}

void GameLevel::RefreshScoreHUDText()
{

	char ScoreMessage[500] = "";
	sprintf_s(ScoreMessage, 500, "Score: %u\nLives %d\n",
		GameController::GetScore(), GameController::GetLives());

	this->scoreAsset->SetText(ScoreMessage);
}