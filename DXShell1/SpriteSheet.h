#pragma once

#include <wincodec.h> //This is the WIC codec header - we need this to decode image files
#include "Graphics.h" //This includes both Windows and D2D libraries
//Remember to add "windowscodecs.lib" to your Linker/Input/AdditionalDependencies

/*
Class Name: SpriteSheet
Purpose: This class represents a sprite sheet (bitmap) which can be drawn to the screen.
	This class will load any bitmap file provided by path in its constructor.
*/
class SpriteSheet
{
	Graphics* gfx; //Reference to the Graphics class
	ID2D1Bitmap* bmp; //This will hold our loaded and converted Bitmap file
	ID2D1Effect* scaleEffect = NULL; //This will hold the result of our chromakey & scale effect

public:
	//Constructor
	SpriteSheet(wchar_t* filename, Graphics* gfx);

	//Destructor
	~SpriteSheet();

	//Draw bitmap to the render target
	void Draw();

	void Draw(float x, float y, float width, float height);

	void DrawWithoutChromaKey(float x, float y, float width, float height);

};