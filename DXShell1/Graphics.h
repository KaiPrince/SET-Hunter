#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <dwrite_3.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")

//#include<Windows.h>
#include <d2d1_1.h>
//#include <d2d1.h>
#include <d3d11_1.h>
#include <d2d1effects.h>
#include <d2d1effects_2.h>
#include <d2d1effecthelpers.h>

class Graphics
{
	//Below, these are all COM interfaces we're using to create D2D resources.
	//We release them as part of the ~Graphics deconstructor... or bad things can happen
	ID2D1Factory* factory; //The factory allows us to create many other types of D2D resources
	ID2D1HwndRenderTarget* rendertarget; //this is typically an area in our GPU memory.. like a back buffer 
	ID2D1SolidColorBrush* brush; //Note this COM interface! Remember to release it!

	IDWriteFactory* textFactory;
	IDWriteTextFormat* textFormat;
	IDWriteTextFormat* HeaderTextFormat;
	IDWriteTypography* HeaderTypography;

	
public:
	Graphics();

	~Graphics();

	int MouseX;
	int MouseY;

	int Window_Width;
	int Window_Height;

	bool Init(HWND windowHandle);

	ID2D1RenderTarget* GetRenderTarget()
	{
		return rendertarget;
	}

	ID2D1DeviceContext* GetDeviceContext()
	{
		ID2D1DeviceContext *rtDC;
		rendertarget->QueryInterface(&rtDC);
		return rtDC;
	}

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawRect(float x, float y, float width, float height, float r, float g, float b, float a);
	void DrawRoundRect(float x, float y, float width, float height, float radiusX, float radiusY, float r, float g, float b, float a);
	void DrawLine(float x1, float y1, float x2, float y2, float width, float r, float g, float b, float a);
	void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a);
	void FillRect(float x, float y, float width, float height, float r, float g, float b, float a);
	void WriteDebugText(char * text, int length);
	void WriteText(float xPos, float yPos, float width, float height, float fontSize, char * text, int length);
	void WriteFancyText(float xPos, float yPos, float width, float height, float fontSize, char * text, int length);
};



/*
Class Name: GraphicsLocator
Purpose: This class implements the Service Locator pattern.
	This class allows the graphics service to be accessed globally.
	Note: don't overuse this. Keep things SOLID.
*/
class GraphicsLocator
{
	GraphicsLocator() {}

	static Graphics* _graphics;
public:
	~GraphicsLocator() {}

	static Graphics* GetGraphics() { return _graphics; }
	static void Provide(Graphics* graphics) { _graphics = graphics; }
};

