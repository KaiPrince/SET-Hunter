#include "Graphics.h"

/***********************************************************************************
The intent of the Graphics class is to handle our DirectX calls, and to be largely responsible 
for managing the rendertarget.
******************************************************************************************/

//Constructor for Graphics class
Graphics::Graphics()
{
	factory = NULL;
	rendertarget = NULL;
	brush = NULL;

}

//Destructor for Graphics class
//Note that all COM objects we instantiate should be 'released' here 
//Look for comments on COM usage in the corresponding header file.

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (rendertarget) rendertarget->Release();
	if (brush) brush->Release();
}

//Provide some comments for each of the methods below.
//Be sure you get a sense of what is happening, and resolve any issues you have understanding these
// methods, their parameters, returns and so on.
bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect); //set the rect's right and bottom properties = the client window's size

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&rendertarget);
	if (res != S_OK) return false;
	
	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;

	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(textFactory),
		reinterpret_cast<IUnknown **>(&textFactory)
	);

	res = textFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		10,
		L"en_us", //locale
		&textFormat
	);

	return true;
}

void Graphics::ClearScreen(float r, float g, float b) 
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	rendertarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);	
}

void Graphics::DrawRect(float x, float y, float width, float height, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	rendertarget->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), brush, 1.0f);
}

void Graphics::FillRect(float x, float y, float width, float height, float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	float smudgeX = 3; //temp fix for gapping problem where a few pixels between squares are unfilled.
	float smudgeY = 3; //TODO: replace with screen size dynamic solution.

	rendertarget->FillRectangle(D2D1::RectF(x, y, x + width + smudgeX, y + height + smudgeY), brush);
}

void Graphics::WriteDebugText(char* text, int length) {
	WriteText(0.0f, 0.0f, text, length);
}

void Graphics::WriteText(float xPos, float yPos, char* text, int length) {
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));//(1.0f, 1.0f, 1.0f, 1.0f));
	char *p = text; //just for proper syntax highlighting ..."
	const WCHAR *pwcsName;
	// required size
	int nChars = MultiByteToWideChar(CP_ACP, 0, p, -1, NULL, 0);
	// allocate it
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, p, -1, (LPWSTR)pwcsName, nChars);
	// use it....

	rendertarget->DrawText(pwcsName, length, textFormat, D2D1::RectF(xPos, yPos, 50.0f, 20.0f), brush);

	// delete it
	delete[] pwcsName;
}