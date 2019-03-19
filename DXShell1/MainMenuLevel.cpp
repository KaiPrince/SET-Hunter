#include "MainMenuLevel.h"
#include "GraphicsLocator.h"
#include "GameController.h"
#include "Level1.h"



MainMenuLevel::MainMenuLevel()
{
	StartButton = new NullActor();
	ExitButton = new NullActor();
}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::Load()
{
	//Create World
	world = new GameWorld(_assetFactory);

	GameBoard* gb = world->GetGameBoard();
	const float ScreenWidth = (float) GraphicsLocator::GetGraphics()->Window_Width;
	const float ScreenHeight = (float) GraphicsLocator::GetGraphics()->Window_Height;

	const float pseudoPixelWidth = ScreenWidth / 10;
	const float pseudoPixelHeight = ScreenHeight / 10;

	//Background
	DrawableAsset* UIsprite = new AssetOutlineDecorator(_assetFactory->CreateDrawableAsset(DrawableAsset::MAIN_MENU_BACKGROUND_ASSET));

	GameObject* background = new GameObject(0.0f, 0.0f, ScreenWidth, ScreenHeight, 
		UIsprite, gb);

	world->AddGameObject(background);

	//Logo
	UIsprite = new AssetOutlineDecorator(_assetFactory->CreateDrawableAsset(DrawableAsset::GRASS_TERRAIN));
	GameObject* Logo = new GameObject(0 + pseudoPixelWidth, 0 + pseudoPixelHeight,
		pseudoPixelWidth * 3, pseudoPixelHeight * 4,
		UIsprite,
		gb);

	world->AddGameObject(Logo);

	//Title
	TextAsset* textAsset = (TextAsset*) _assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("SET HUNTER");

	UIsprite = textAsset;
	
	GameObject* Title = new GameObject(ScreenWidth - (5 * pseudoPixelWidth), 0 + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, gb);

	world->AddGameObject(Title);

	//Start Button
	textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("Start Game");

	UIsprite = new AssetRoundedOutlineDecorator(textAsset); //TODO: create Rounded Rectangle

	this->StartButton = new Actor(0 + (2 * pseudoPixelWidth), ScreenHeight - (4 * pseudoPixelHeight),
		6 * pseudoPixelWidth, 1 * pseudoPixelHeight, UIsprite, gb);
	this->StartButton->SetInputComponent(new ClickableInputComponent(this->StartButton));
	this->StartButton->AddObserver(this);

	world->AddGameObject(this->StartButton);

	//Exit Button
	textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("Exit Game");

	UIsprite = new AssetRoundedOutlineDecorator(textAsset); //TODO: create Rounded Rectangle

	this->ExitButton = new Actor(0 + (2 * pseudoPixelWidth), ScreenHeight - (2 * pseudoPixelHeight),
		6 * pseudoPixelWidth, 1 * pseudoPixelHeight, UIsprite, gb);
	this->ExitButton->SetInputComponent(new ClickableInputComponent(this->ExitButton));
	this->ExitButton->AddObserver(this);

	world->AddGameObject(this->ExitButton);


}

void MainMenuLevel::Unload()
{
	//Delete World
	delete world;
}

void MainMenuLevel::HandleInput() {
	world->HandleInput();
}

void MainMenuLevel::Update()
{
	world->Update();
}

void MainMenuLevel::Render()
{
	world->Draw();
}

void MainMenuLevel::Notify(Observable * subject)
{

	if (subject == this->StartButton) {
		//Load Level 1
		GameController::QueuedNextLevel = new Level1();
	}
	else if (subject == this->ExitButton) {
		GameController::QueueExitGame = true;
	}
}
