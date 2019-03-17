#pragma once

#include "Graphics.h"
#include "GameBoard.h"
#include "Actor.h"
#include "InputComponent.h"

class GameLevel
{
protected:
	static Graphics* gfx;
	static GameBoard* gb;
	static AssetFactory* _assetFactory;

	static Actor* player;

public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
		gb = new GameBoard();
		_assetFactory = new AssetFactory(gfx);
		gb->SetAssetFactory(_assetFactory); //TODO: delete in destructor
		gb->squareWidth = (float) gfx->Window_Width / gb->boardWidth; //TODO: change this to use a setter;
		gb->squareHeight = (float) gfx->Window_Height / gb->boardHeight; //TODO: change this to use a setter;
		gb->Init();

		//Player starts at bottom middle of screen.
		float player_StartX = (float)((gb->boardWidth * gb->squareWidth / 2) - gb->squareWidth);
		float player_StartY = ((float)gb->boardHeight * gb->squareHeight) - gb->squareHeight ;

		player = new Actor(player_StartX, player_StartY,
			gb->squareWidth, gb->squareHeight, _assetFactory->CreateDrawableAsset(DrawableAsset::CAR_SPRITE), gb);
		player->SetPhysicsComponent(new PlayerPhysicsComponent(player, gb));
		player->SetInputComponent(new PlayerInputComponent(player));
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput() {
		player->HandleInput();
	}

	void SetGameBoard(GameBoard* gameBoard) {
		gb = gameBoard;
	}

	GameBoard* GetGameBoard() {
		return gb;
	}

	Actor* GetPlayer() {
		return player;
	}
};