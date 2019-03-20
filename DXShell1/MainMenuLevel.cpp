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

	{
		//Player starts at bottom middle of screen.
		GameBoard* gb = world->GetGameBoard();
		float player_StartX = (float)((gb->boardWidth * gb->squareWidth / 2) - gb->squareWidth);
		float player_StartY = ((float)gb->boardHeight * gb->squareHeight) - gb->squareHeight;

		Actor* player = new Actor(player_StartX, player_StartY,
			gb->squareWidth, gb->squareHeight, _assetFactory->CreateDrawableAsset(DrawableAsset::CAR_SPRITE), gb);
		player->SetCollidable(true);
		player->SetPhysicsComponent(new PlayerPhysicsComponent(player, world));
		player->SetInputComponent(new StayOnRoadInputComponent(player));

		world->SetPlayer(player);
	}


	GameBoard* gb = world->GetGameBoard();
	const float ScreenWidth = (float)GraphicsLocator::GetGraphics()->Window_Width;
	const float ScreenHeight = (float)GraphicsLocator::GetGraphics()->Window_Height;

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
	TextAsset* textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("SET HUNTER");
	textAsset->SetUseFancyFont(true);
	textAsset->SetFontSize(40.0f);

	UIsprite = textAsset;

	GameObject* Title = new GameObject(ScreenWidth - (5 * pseudoPixelWidth), 0 + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, gb);

	world->AddGameObject(Title);

	//PlayerName
	textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("By Agent Kai Prince"); //TODO: global constant

	UIsprite = textAsset;

	GameObject* PlayerName = new GameObject(Title->GetXPos(), Title->GetYPos() + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, gb);

	world->AddGameObject(PlayerName);

	//Previous Score
	textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET);
	{
		char labelText[500];
		sprintf_s(labelText, 500, "Previous Score: %d", GameController::GetScore());
		textAsset->SetText(labelText);
	}

	UIsprite = textAsset;

	GameObject* PreviousScoreLabel = new GameObject(PlayerName->GetXPos(), PlayerName->GetYPos() + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, gb);

	world->AddGameObject(PreviousScoreLabel);

	//Start Button
	textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET); //TODO: Center text
	textAsset->SetText("Start Game");
	textAsset->SetUseFancyFont(true);
	textAsset->SetFontSize(40.0f);

	UIsprite = new AssetRoundedOutlineDecorator(textAsset);

	this->StartButton = new Actor(0 + (2 * pseudoPixelWidth), ScreenHeight - (4 * pseudoPixelHeight),
		6 * pseudoPixelWidth, 1 * pseudoPixelHeight, UIsprite, gb);
	this->StartButton->SetInputComponent(new ClickableInputComponent(this->StartButton));
	this->StartButton->AddObserver(this);

	world->AddGameObject(this->StartButton);

	//Exit Button
	textAsset = (TextAsset*)_assetFactory->CreateDrawableAsset(DrawableAsset::TEXT_ASSET); //TODO: Center text
	textAsset->SetText("Exit Game");
	textAsset->SetUseFancyFont(true);
	textAsset->SetFontSize(40.0f);

	UIsprite = new AssetRoundedOutlineDecorator(textAsset);

	this->ExitButton = new Actor(0 + (2 * pseudoPixelWidth), ScreenHeight - (2 * pseudoPixelHeight),
		6 * pseudoPixelWidth, 1 * pseudoPixelHeight, UIsprite, gb);
	this->ExitButton->SetInputComponent(new ClickableInputComponent(this->ExitButton));
	this->ExitButton->AddObserver(this);

	world->AddGameObject(this->ExitButton);

	mainMenu.push_back(background);
	mainMenu.push_back(Logo);
	mainMenu.push_back(Title);
	mainMenu.push_back(PlayerName);
	mainMenu.push_back(PreviousScoreLabel);
	mainMenu.push_back(StartButton);
	mainMenu.push_back(ExitButton);
}

void MainMenuLevel::Unload()
{
	StartButton->RemoveObserver(this);
	ExitButton->RemoveObserver(this);

	GameController::SetLives(3);
	GameController::SetScore(0);

	//Delete all menu items.
	for (GameObject* obj : mainMenu) {

		if (obj != world->GetPlayer()) {
			world->RemoveGameObject(obj);
		}

	}

	world->GetPlayer()->RemoveObserver(this);
	delete world->GetPlayer()->GetInputComponent();
	world->GetPlayer()->SetInputComponent(new PlayerInputComponent(world->GetPlayer()));

	//Delete World
	//delete world;
}

void MainMenuLevel::HandleInput() {
	world->HandleInput();
}

void MainMenuLevel::Update()
{
	world->GetGameBoard()->ScrollBoard();
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
