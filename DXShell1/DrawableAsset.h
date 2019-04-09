#pragma once
#include <vector>
#include "Graphics.h"

/*
Class Name: DrawableAsset
Purpose: This interface class is used to represent any asset which can draw itself to the screen.
*/
class DrawableAsset
{
	static Graphics* gfx;
protected:
	Graphics* GetGraphics() {
		return gfx;
	}

	DrawableAsset();
public:
	virtual ~DrawableAsset();

	static void SetGraphics(Graphics* graphics) {
		gfx = graphics;
	}

	//TODO: move to assetFactory?
	enum AssetTypes {
		GRASS_TERRAIN, ROAD_TERRAIN, CAR_SPRITE, TREE_SPRITE, TREE2_SPRITE, SHRUB_SPRITE, 
		EXPLOSION_SPRITE, NULL_ASSET, MAIN_MENU_BACKGROUND_ASSET, TEXT_ASSET, SPY_HUNTER_ART, ENEMY_CAR,
		ROCKET_SPRITE
	};

public:
	virtual DrawableAsset::AssetTypes GetType() = 0;

	virtual void Draw(float x, float y, float width, float height) = 0;
};

/*
Class Name: Null Asset
Purpose: This class implements the Null Object pattern.
*/
class NullAsset : public DrawableAsset
{
public:
	NullAsset();
	~NullAsset();

	virtual DrawableAsset::AssetTypes GetType() override { return DrawableAsset::NULL_ASSET; }

	virtual void Draw(float x, float y, float width, float height) override {
		//Draw nothing.
	}

private:

};

/*
Class Name: MainMenuBackgroundAsset
Purpose: This class contains instructions on how to draw a main menu background.
*/
class MainMenuBackgroundAsset : public DrawableAsset
{
public:
	MainMenuBackgroundAsset() : DrawableAsset() {}
	~MainMenuBackgroundAsset() {}

	virtual DrawableAsset::AssetTypes GetType() override { return DrawableAsset::MAIN_MENU_BACKGROUND_ASSET; }

	// Inherited via DrawableAsset
	virtual void Draw(float x, float y, float width, float height) override;
};

class TextAsset : public DrawableAsset
{
	char Text[500];
	float FontSize;
	bool UseFancyFont;
public:
	TextAsset() : DrawableAsset() { FontSize = 10.0f; UseFancyFont = false; Text[0] = '\0'; }
	~TextAsset() {}

	virtual DrawableAsset::AssetTypes GetType() override { return DrawableAsset::TEXT_ASSET; }

	virtual void Draw(float x, float y, float width, float height) override;

	void SetText(const char* text) {
		strcpy_s(Text, 500, text);
	}

	void SetFontSize(float fontsize) {
		FontSize = fontsize;
	}

	void SetUseFancyFont(bool tf) {
		UseFancyFont = tf;
	}

	

private:

};

class AssetOutlineDecorator : public DrawableAsset {
	DrawableAsset* _asset;
public:
	AssetOutlineDecorator(DrawableAsset* asset) : DrawableAsset() {
		_asset = asset;
	}
	virtual ~AssetOutlineDecorator() {}

	virtual DrawableAsset::AssetTypes GetType() override { return _asset->GetType(); } //Decorators have a transparent type.

	virtual void Draw(float x, float y, float width, float height) override;
};

class AssetRoundedOutlineDecorator : public DrawableAsset
{
	DrawableAsset* _asset;
public:
	AssetRoundedOutlineDecorator(DrawableAsset* asset) : DrawableAsset() {
		_asset = asset;
	}
	virtual ~AssetRoundedOutlineDecorator() {}

	virtual DrawableAsset::AssetTypes GetType() override { return _asset->GetType(); } //Decorators have a transparent type.

	virtual void Draw(float x, float y, float width, float height) override;
private:

};

class AssetBackgroundRectangleDecorator : public DrawableAsset
{
	DrawableAsset* _asset;
public:
	AssetBackgroundRectangleDecorator(DrawableAsset* asset) : DrawableAsset() {
		_asset = asset;
	}
	virtual ~AssetBackgroundRectangleDecorator() {}

	virtual DrawableAsset::AssetTypes GetType() override { return _asset->GetType(); } //Decorators have a transparent type.

	virtual void Draw(float x, float y, float width, float height) override;
private:
};

class RoadTerrain :
	public DrawableAsset
{
public:
	RoadTerrain() : DrawableAsset() {
	}
	virtual ~RoadTerrain() {}

	virtual DrawableAsset::AssetTypes GetType() override { return DrawableAsset::ROAD_TERRAIN; }

	void Draw(float x, float y, float width, float height) {
		GetGraphics()->FillRect(x, y, width, height, 0.0f, 0.0f, 0.0f, 1.0f);
	}
};

class GrassTerrain :
	public DrawableAsset
{
public:
	GrassTerrain() : DrawableAsset() {
	}
	virtual ~GrassTerrain() {}

	virtual DrawableAsset::AssetTypes GetType() override { return DrawableAsset::GRASS_TERRAIN; }

	void Draw(float x, float y, float width, float height) {
		GetGraphics()->FillRect(x, y, width, height, 76.0f / 255.0f, 169.0f / 255.0f, 70.0f / 255.0f, 1.0f);
	}
};

#include "SpriteSheet.h"

class SpriteSheetAsset :
	public DrawableAsset
{
	SpriteSheet* sprite;
	DrawableAsset::AssetTypes type;
	bool useChromaKey;
public:
	SpriteSheetAsset(SpriteSheet* sprite, DrawableAsset::AssetTypes assetType) : DrawableAsset() {

		this->sprite = sprite;
		this->type = assetType;
		this->useChromaKey = true;
	}
	virtual ~SpriteSheetAsset() {}

	virtual DrawableAsset::AssetTypes GetType() override { return this->type; }
	virtual void SetUseChromaKey(bool tf) { this->useChromaKey = tf; }

	virtual void Draw(float x, float y, float width, float height) {
		if (useChromaKey) {
			sprite->Draw(x, y, width, height);
		}
		else {
			sprite->DrawWithoutChromaKey(x, y, width, height);
		}
	}
};

