#include "Graphics.h"

/***********************************************************************************
The intent of the Graphics class is to handle our DirectX calls, and to be largely responsible 
for managing the rendertarget.
******************************************************************************************/

Graphics* GraphicsLocator::_graphics;

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
	if (HeaderTypography) HeaderTypography->Release();
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

	res = textFactory->CreateTextFormat(
		L"Gabriola",
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		10.0f,
		L"en-US", // locale 
		&HeaderTextFormat
	);

	textFactory->CreateTypography(&HeaderTypography);

	DWRITE_FONT_FEATURE fontFeature = { DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_1, 1 };
	//HeaderTypography->AddFontFeature(fontFeature);
	

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

void Graphics::DrawRoundRect(float x, float y, float width, float height, float radiusX, float radiusY, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	rendertarget->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x + width, y + height), radiusX, radiusY) ,brush, 1.0f);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, float width, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	rendertarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, width);
}

void Graphics::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	//TODO: move this somewhere it can be defined once.
	ID2D1PathGeometry* pathGeometry = nullptr;
	//TODO: error check
	factory->CreatePathGeometry(&pathGeometry);
	ID2D1GeometrySink *pSink = NULL;
	pathGeometry->Open(&pSink);

	pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

	pSink->BeginFigure(
		D2D1::Point2F(x1, y1),
		D2D1_FIGURE_BEGIN_FILLED
	);
	D2D1_POINT_2F points[2] = {
		D2D1::Point2F(x2, y2),
		D2D1::Point2F(x3, y3),
	};
	pSink->AddLines(points, ARRAYSIZE(points));
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

	pSink->Close();

	rendertarget->FillGeometry(pathGeometry, brush);
}

void Graphics::FillRect(float x, float y, float width, float height, float r, float g, float b, float a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));


	rendertarget->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush);
}

void Graphics::WriteDebugText(char* text, int length) {
	WriteText(0.0f, 0.0f, 10.0f, 50.0f, 20.0f, text, length);
}

void Graphics::WriteText(float xPos, float yPos, float width, float height, float fontSize, char* text, int length) {
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));//(1.0f, 1.0f, 1.0f, 1.0f));
	

	char *p = text; //just for proper syntax highlighting ..."
	const WCHAR *pwcsName;
	// required size
	int nChars = MultiByteToWideChar(CP_ACP, 0, p, -1, NULL, 0);
	// allocate it
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, p, -1, (LPWSTR)pwcsName, nChars);
	// use it....

	IDWriteTextLayout* textLayout;
	textFactory->CreateTextLayout(
		pwcsName,
		nChars,
		textFormat,
		width,
		height,
		&textLayout
	);

	DWRITE_TEXT_RANGE textRange = { 0, (UINT32) nChars };
	textLayout->SetFontSize(fontSize, textRange);

	rendertarget->DrawTextLayout(D2D1::Point2F(xPos, yPos), textLayout, brush);

	//rendertarget->DrawText(pwcsName, length, textFormat, D2D1::RectF(xPos, yPos, xPos + 50.0f, yPos + 20.0f), brush);
	//rendertarget->DrawRectangle(D2D1::RectF(xPos, yPos, xPos + textLayout->GetMaxWidth(), yPos + textLayout->GetMaxHeight()), brush);


	// delete it
	delete[] pwcsName;
	textLayout->Release();
}

void Graphics::WriteFancyText(float xPos, float yPos, float width, float height, float fontSize, char * text, int length) {
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));//(1.0f, 1.0f, 1.0f, 1.0f));


	char *p = text; //just for proper syntax highlighting ..."
	const WCHAR *pwcsName;
	// required size
	int nChars = MultiByteToWideChar(CP_ACP, 0, p, -1, NULL, 0);
	// allocate it
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, p, -1, (LPWSTR)pwcsName, nChars);
	// use it....

	IDWriteTextLayout* textLayout;
	textFactory->CreateTextLayout(
		pwcsName,
		nChars,
		HeaderTextFormat,
		width,
		height,
		&textLayout
	);

	DWRITE_TEXT_RANGE textRange = { 0, (UINT32)nChars };
	textLayout->SetFontSize(fontSize, textRange);

	textLayout->SetTypography(HeaderTypography, textRange);

	rendertarget->DrawTextLayout(D2D1::Point2F(xPos, yPos), textLayout, brush);
	//rendertarget->DrawRectangle(D2D1::RectF(xPos, yPos, xPos + textLayout->GetMaxWidth(), yPos + textLayout->GetMaxHeight()), brush);

	//rendertarget->DrawText(pwcsName, length, textFormat, D2D1::RectF(xPos, yPos, xPos + 50.0f, yPos + 20.0f), brush);


	// delete it
	delete[] pwcsName;
	textLayout->Release();
}