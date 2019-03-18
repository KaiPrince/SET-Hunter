#include "MainMenuLevel.h"
#include "GraphicsLocator.h"



MainMenuLevel::MainMenuLevel()
{

}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::Load()
{
	const float ScreenWidth = (float) GraphicsLocator::GetGraphics()->Window_Width;
	const float ScreenHeight = (float) GraphicsLocator::GetGraphics()->Window_Height;

	const float pseudoPixelWidth = ScreenWidth / 10;
	const float pseudoPixelHeight = ScreenHeight / 10;

	//Background
	DrawableAsset* UIsprite = new AssetOutlineDecorator(_assetFactory->CreateDrawableAsset(DrawableAsset::MAIN_MENU_BACKGROUND_ASSET));

	GameObject* background = new GameObject(0.0f, 0.0f, ScreenWidth, ScreenHeight, 
		UIsprite, gb);
	background->SetPhysicsComponent(new NullPhysicsComponent(background, world));

	mainMenu.push_back(background);

	//Logo
	UIsprite = new AssetOutlineDecorator(_assetFactory->CreateDrawableAsset(DrawableAsset::GRASS_TERRAIN));
	GameObject* Logo = new GameObject(0 + pseudoPixelWidth, 0 + pseudoPixelHeight,
		pseudoPixelWidth * 3, pseudoPixelHeight * 4,
		UIsprite,
		gb);
	Logo->SetPhysicsComponent(new NullPhysicsComponent(background, world));

	mainMenu.push_back(Logo);

	//Title
	TextAsset* textAsset = (TextAsset*) _assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("SET HUNTER");

	UIsprite = new AssetOutlineDecorator(textAsset);
	
	GameObject* Title = new GameObject(ScreenWidth - (5 * pseudoPixelWidth), 0 + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, gb);
	Title->SetPhysicsComponent(new NullPhysicsComponent(Title, world));

	mainMenu.push_back(Title);

	//Start Button


	//Exit Button

}

void MainMenuLevel::Unload()
{
	//TODO..
}

void MainMenuLevel::Update()
{
}

void MainMenuLevel::Render()
{
	for each (GameObject* element in mainMenu)
	{
		element->Draw();
	}
}

void MainMenuLevel::Notify(Observable * subject)
{
	//TODO: handle object click
}
