#pragma once
#include <vector>
#include "Graphics.h"
class DrawableAsset
{
	static Graphics* gfx;
protected:
	Graphics* GetGraphics() {
		return gfx;
	}

public:
	DrawableAsset();
	virtual ~DrawableAsset();

	static void SetGraphics(Graphics* graphics) {
		gfx = graphics;
	}

	//TODO: move to assetFactory?
	enum AssetTypes {
		GRASS_TERRAIN, ROAD_TERRAIN, CAR_SPRITE, TREE_SPRITE, TREE2_SPRITE, SHRUB_SPRITE, 
		EXPLOSION_SPRITE, EMPTY_ASSET, MAIN_MENU_BACKGROUND_ASSET, TEXT_ASSET, SPY_HUNTER_ART
	};
protected: //This is only here because the AssetTypes var must be below its definition.
	DrawableAsset::AssetTypes type;

public:
	DrawableAsset::AssetTypes GetType() { return type; }

	virtual void Draw(float x, float y, float width, float height) = 0;
};

class EmptyAsset : public DrawableAsset
{
public:
	EmptyAsset();
	~EmptyAsset();

	virtual void Draw(float x, float y, float width, float height) override {
		//Draw nothing.
	}

private:

};

class MainMenuBackgroundAsset : public DrawableAsset
{
public:
	MainMenuBackgroundAsset() : DrawableAsset() {}
	~MainMenuBackgroundAsset() {}

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

	virtual void Draw(float x, float y, float width, float height) override;
private:

};

