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
	GetGraphics()->WriteText(x, y, width, height, 64.0f, Text, (int)strlen(Text));
}

void AssetOutlineDecorator::Draw(float x, float y, float width, float height) {
	_asset->Draw(x, y, width, height);

	GetGraphics()->DrawRect(x, y, width, height, 1.0f, 0.0f, 0.0f, 1.0f);
}

void AssetRoundedOutlineDecorator::Draw(float x, float y, float width, float height)
{
	_asset->Draw(x, y, width, height);

	GetGraphics()->DrawRoundRect(x, y, width, height, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
