#include "DrawableAsset.h"


Graphics* DrawableAsset::gfx;



DrawableAsset::DrawableAsset()
{
}

DrawableAsset::~DrawableAsset()
{
}

NullAsset::NullAsset()
{
}

NullAsset::~NullAsset()
{
}

void MainMenuBackgroundAsset::Draw(float x, float y, float width, float height)
{
	GetGraphics()->FillRect(x, y, width, height, 1.0f, 1.0f, 1.0f, 0.5f);
}

void TextAsset::Draw(float x, float y, float width, float height) {
	if (UseFancyFont)
	{
		GetGraphics()->WriteFancyText(x, y, width, height, FontSize, Text, (int)strlen(Text));
	}
	else
	{
		GetGraphics()->WriteText(x, y, width, height, FontSize, Text, (int)strlen(Text));
	}
	
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
