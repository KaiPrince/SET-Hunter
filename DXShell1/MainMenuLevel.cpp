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
	const float ScreenWidth = GraphicsLocator::GetGraphics()->Window_Width;
	const float ScreenHeight = GraphicsLocator::GetGraphics()->Window_Height;

	const float pseudoPixelWidth = ScreenWidth / 10;
	const float pseudoPixelHeight = ScreenHeight / 10;

	GameObject* background = new GameObject(0.0f, 0.0f, ScreenWidth, ScreenHeight, 
		_assetFactory->CreateDrawableAsset(DrawableAsset::MAIN_MENU_BACKGROUND_ASSET), gb);
	background->SetPhysicsComponent(new NullPhysicsComponent(background, world));

	mainMenu.push_back(background);

	GameObject* Logo = new GameObject(0 + pseudoPixelWidth, 0 + pseudoPixelHeight,
		pseudoPixelWidth * 3, pseudoPixelHeight * 4,
		_assetFactory->CreateDrawableAsset(DrawableAsset::GRASS_TERRAIN),
		gb);
	Logo->SetPhysicsComponent(new NullPhysicsComponent(background, world));

	mainMenu.push_back(Logo);

	//TODO: Text, Buttons
	TextAsset* textAsset = (TextAsset*) _assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("SET HUNTER");
	
	GameObject* Title = new GameObject(ScreenWidth - (5 * pseudoPixelWidth), 0 + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		textAsset, gb);
	Title->SetPhysicsComponent(new NullPhysicsComponent(Title, world));

	mainMenu.push_back(Title);



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
}
