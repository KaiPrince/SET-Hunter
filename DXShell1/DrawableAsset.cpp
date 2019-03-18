#include "DrawableAsset.h"


Graphics* DrawableAsset::gfx;



DrawableAsset::DrawableAsset()
{
}

DrawableAsset::~DrawableAsset()
{
}

EmptyAsset::EmptyAsset()
{
}

EmptyAsset::~EmptyAsset()
{
}

void MainMenuBackgroundAsset::Draw(float x, float y, float width, float height)
{
	GetGraphics()->FillRect(x, y, width, height, 1.0f, 1.0f, 1.0f, 0.5f);
}

void TextAsset::Draw(float x, float y, float width, float height) {
	GetGraphics()->WriteText(x, y, Text, strlen(Text));
}
