#include "MainMenuLevel.h"
#include "Graphics.h"
#include "GameController.h"
#include "Level1.h"
#include "CollisionResolutionStrategy.h"
#include "Audio.h"



MainMenuLevel::MainMenuLevel()
{
	StartButton = new NullGameObject();
	ExitButton = new NullGameObject();

	isPlayerDead = false; //TODO: remove
}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::Load()
{
	//Create World
	world = new GameWorld(_assetFactory);

	//Play main theme
	AudioLocator::GetAudio()->PlayThemeSong();

	{
		//Player starts at bottom middle of screen.
		GameBoard* gb = world->GetGameBoard();
		float player_StartX = (float)((gb->boardWidth * gb->squareWidth / 2) - gb->squareWidth);
		float player_StartY = ((float)gb->boardHeight * gb->squareHeight) - (gb->squareHeight * 3); //bring it a little off the bottom

		GameObject* player = new GameObject(player_StartX, player_StartY,
			gb->squareWidth / 2, gb->squareHeight, _assetFactory->GetAsset(DrawableAsset::CAR_SPRITE), world);
		player->UpdateState(new AliveState(player));
		player->SetPhysicsComponent(new PlayerPhysicsComponent(player, world));
		player->SetInputComponent(new StayOnRoadInputComponent(player));
		player->GetPhysicsComponent()->SetCollisionStrategy(new PlayerCollisionStrategy(player));

		player->AddObserver(this);

		world->SetPlayer(player);
	}


	GameBoard* gb = world->GetGameBoard();
	const float ScreenWidth = (float)GraphicsLocator::GetGraphics()->Window_Width;
	const float ScreenHeight = (float)GraphicsLocator::GetGraphics()->Window_Height;

	const float pseudoPixelWidth = ScreenWidth / 10;
	const float pseudoPixelHeight = ScreenHeight / 10;

	//Background
	DrawableAsset* UIsprite = _assetFactory->GetAsset(DrawableAsset::MAIN_MENU_BACKGROUND_ASSET);

	GameObject* background = new GameObject(0.0f, 0.0f, ScreenWidth, ScreenHeight,
		UIsprite, world);


	//Logo
	UIsprite = new AssetRoundedOutlineDecorator(_assetFactory->GetAsset(DrawableAsset::NULL_ASSET));
	GameObject* Logo = new GameObject(0 + pseudoPixelWidth, 0 + pseudoPixelHeight,
		pseudoPixelWidth * 3, pseudoPixelHeight * 4,
		UIsprite,
		world);


	//Title
	TextAsset* textAsset = (TextAsset*)_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("SET HUNTER");
	textAsset->SetUseFancyFont(true);
	textAsset->SetFontSize(40.0f);

	UIsprite = textAsset;

	GameObject* Title = new GameObject(ScreenWidth - (5 * pseudoPixelWidth), 0 + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, world);


	//PlayerName
	textAsset = (TextAsset*)_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("By Agent Kai Prince"); //TODO: global constant

	UIsprite = textAsset;

	GameObject* PlayerName = new GameObject(Title->GetXPos(), Title->GetYPos() + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, world);


	//Previous Score
	textAsset = (TextAsset*)_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET);
	{
		char labelText[500];
		sprintf_s(labelText, 500, "Previous Score: %d", GameController::GetScore());
		textAsset->SetText(labelText);
	}

	UIsprite = textAsset;

	GameObject* PreviousScoreLabel = new GameObject(PlayerName->GetXPos(), PlayerName->GetYPos() + pseudoPixelHeight,
		4 * pseudoPixelWidth, 1 * pseudoPixelHeight,
		UIsprite, world);

	//Start Button
	UIsprite = new AssetRoundedOutlineDecorator(AssetFactory::GetNullAsset());

	this->StartButton = new GameObject(0 + (2 * pseudoPixelWidth), ScreenHeight - (4 * pseudoPixelHeight),
		6 * pseudoPixelWidth, 1 * pseudoPixelHeight, UIsprite, world);
	this->StartButton->SetInputComponent(new ClickableInputComponent(this->StartButton));
	this->StartButton->AddObserver(this);


	//Start Button Label
	textAsset = (TextAsset*)_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("Start Game");
	textAsset->SetUseFancyFont(true);
	textAsset->SetFontSize(70.0f);

	const float originX_startButton = StartButton->GetXPos() + (StartButton->GetWidth() / 2);
	const float originY_startButton = StartButton->GetYPos();
	GameObject* StartButtonLabel = new GameObject(originX_startButton - (1 * pseudoPixelWidth),
		originY_startButton - (0.4f * pseudoPixelWidth), StartButton->GetWidth(), StartButton->GetHeight(), textAsset, world);

	//Exit Button
	UIsprite = new AssetRoundedOutlineDecorator(AssetFactory::GetNullAsset());

	this->ExitButton = new GameObject(0 + (2 * pseudoPixelWidth), ScreenHeight - (2 * pseudoPixelHeight),
		6 * pseudoPixelWidth, 1 * pseudoPixelHeight, UIsprite, world);
	this->ExitButton->SetInputComponent(new ClickableInputComponent(this->ExitButton));
	this->ExitButton->AddObserver(this);

	//Exit Button Label
	textAsset = (TextAsset*)_assetFactory->GetAsset(DrawableAsset::TEXT_ASSET);
	textAsset->SetText("Exit Game");
	textAsset->SetUseFancyFont(true);
	textAsset->SetFontSize(70.0f);

	const float originX_exitButton = ExitButton->GetXPos() + (ExitButton->GetWidth() / 2);
	const float originY_exitButton = ExitButton->GetYPos();
	GameObject* ExitButtonLabel = new GameObject(originX_exitButton - (1 * pseudoPixelWidth),
		originY_exitButton - (0.4f * pseudoPixelWidth), ExitButton->GetWidth(), ExitButton->GetHeight(), textAsset, world);

	world->AddUIObject(background);
	world->AddUIObject(Logo);
	world->AddUIObject(Title);
	world->AddUIObject(PlayerName);
	world->AddUIObject(PreviousScoreLabel);
	world->AddUIObject(this->StartButton);
	world->AddUIObject(StartButtonLabel);
	world->AddUIObject(this->ExitButton);
	world->AddUIObject(ExitButtonLabel);

	mainMenu.push_back(background);
	mainMenu.push_back(Logo);
	mainMenu.push_back(Title);
	mainMenu.push_back(PlayerName);
	mainMenu.push_back(PreviousScoreLabel);
	mainMenu.push_back(StartButton);
	mainMenu.push_back(StartButtonLabel);
	mainMenu.push_back(ExitButton);
	mainMenu.push_back(ExitButtonLabel);

}

void MainMenuLevel::Unload()
{
	StartButton->RemoveObserver(this);
	ExitButton->RemoveObserver(this);

	GameController::SetLives(3);
	GameController::SetScore(0);

	//Delete all menu items.
	for (GameObject* obj : mainMenu) {

		world->RemoveUIObject(obj);

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
	else if (subject == world->GetPlayer()) {
		//TODO: use visitor pattern on playerstate?

		if (!isPlayerDead) {
			isPlayerDead = true;

			//Player probably died.
			//continue road scrolling. Wait 2 seconds and then create a new car.
			//hide player
			world->GetPlayer()->SetPhysicsComponent(new NullPhysicsComponent()); //disable collision detection
			world->GetPlayer()->SetYPos(static_cast<float>(GraphicsLocator::GetGraphics()->Window_Height * 2)); //move off screen
		}
		else {
			isPlayerDead = false;
			//Player probably revived.

			world->GetPlayer()->SetYVelocity(-(world->GetGameBoard()->squareHeight / 4));
			world->GetPlayer()->SetPhysicsComponent(new PlayerPhysicsComponent(world->GetPlayer(), world));

		}

	}
}
