#pragma once

#include "Graphics.h"
#include "GameBoard.h"
#include "Player.h"

class GameLevel
{
protected:
	static Graphics* gfx;
	static GameBoard* gb;
	static AssetFactory* _assetFactory;

	static Player* player;


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

		//TODO: create #defines for player width and height
		player = new Player((float) ((gb->boardWidth * gb->squareWidth / 2) - gb->squareWidth), (float) gb->boardHeight * gb->squareHeight, //Bottom middle
			gb->squareWidth, gb->squareHeight, _assetFactory->CreateDrawableAsset(DrawableAsset::CAR_SPRITE));
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetGameBoard(GameBoard* gameBoard) {
		gb = gameBoard;
	}

	GameBoard* GetGameBoard() {
		return gb;
	}
};