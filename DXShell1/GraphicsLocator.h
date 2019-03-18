#pragma once
#include "Graphics.h"

/*
Class Name: GraphicsLocator
Purpose: This class implements the Service Locator pattern.
	This class allows the graphics service to be accessed globally.
	Note: don't overuse this. Keep things SOLID.
*/
class GraphicsLocator
{
	GraphicsLocator();

	static Graphics* _graphics;
public:
	~GraphicsLocator();

	static Graphics* GetGraphics() { return _graphics; }
	static void Provide(Graphics* graphics) { _graphics = graphics; }
};

