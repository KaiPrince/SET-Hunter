#include <Windows.h>
#include <WinUser.h>
#include <Windowsx.h>
#include "Graphics.h"
#include "Audio.h"
#include "SplashScreenLevel.h"
#include "GameController.h"
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <chrono>

Graphics* graphics;
Audio* audio;

/*
Your Mission: Document the following code. What do all these components do?
*/
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }

	//BIG CHANGE! We are not Drawing/rendering here! 
	// We've moved this down to the message handling loop below

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Have you seen a 'main' for a Win32 app before? Please comment this.
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPWSTR cmd,
	int nCmdShow
)
{
	HRESULT errorStatus = CoInitialize(NULL);
	if (errorStatus != S_OK) return -1;

	//Do a bit of reading - What is this Window Class used for? 
	// What are the major parameters below?
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = "MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW; //Alert - This is useful here... what does it do?

	RegisterClassEx(&windowclass);

	//TODO: move these
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };//Do these numbers look significant to you? What are they?
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	// The factory returns the current system DPI. This is also the value it will use
	// to create its own windows.
	FLOAT dpiX, dpiY;
	ID2D1Factory * m_pDirect2dFactory;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	m_pDirect2dFactory->Release();

	const int originX = CW_USEDEFAULT; //100;
	const int originY = CW_USEDEFAULT; //100;
	const int width = static_cast<UINT>(ceil(WINDOW_WIDTH * dpiX / 96.f)); //rect.right - rect.left;
	const int height = static_cast<UINT>(ceil(WINDOW_HEIGHT * dpiY / 96.f)); // rect.bottom - rect.top

//Below is another important process to understand... what are we doing?
//Why is this connected to rect we just defined above?
	HWND windowhandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "SET Hunter", WS_OVERLAPPEDWINDOW, originX, originY,
		width, height, NULL, NULL, hInstance, 0);
	if (!windowhandle) return -1;

	graphics = new Graphics();
	graphics->Window_Width = WINDOW_WIDTH;
	graphics->Window_Height = WINDOW_HEIGHT;
	if (!graphics->Init(windowhandle))
	{
		delete graphics;
		return -1;
	}
	GraphicsLocator::Provide(graphics);

	audio = new DirectXAudio(windowhandle);
	AudioLocator::Provide(audio);


	GameController::Init();
	GameLevel::Init(graphics);


	ShowWindow(windowhandle, nCmdShow);
	GameController::LoadInitialLevel(new SplashScreenLevel());

#pragma region GameLoop
	//Below, we have essentially an infinite loop that will keep the window running and will dispatch/show messages
	//As many people will tell you, most Windows you see are just infinite loops waiting for some kind of work-flow or 
	//system-based interuption.

	//clock_t previousTime = clock();
	auto previousTime = std::chrono::high_resolution_clock::now();
	double lag = 0.0;

	//Note - Our message handling has changed from the first demo code.
	//Here, we use a PeekMessage to avoid locking the graphics/windoProc
	//when a message has to be dispatched.
	MSG message;
	message.message = WM_NULL; //Do not have this set to WM_QUIT, which has a specific context
	while (message.message != WM_QUIT && !GameController::QueueExitGame)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_MOUSEMOVE)
			{
				//DEBUG TODO: REMOVE
				int xPos = GET_X_LPARAM(message.lParam);
				int yPos = GET_Y_LPARAM(message.lParam);
				graphics->MouseX = xPos;
				graphics->MouseY = yPos;
			}
			//This allows us to send a message to the WindowProc IF there is one
			DispatchMessage(&message);
		}
		else
		{
			//clock_t currentTime = clock();
			auto currentTime = std::chrono::high_resolution_clock::now();

			double elapsedTimeInMS = std::chrono::duration<double, std::milli>(currentTime - previousTime).count();

			previousTime = currentTime;
			lag += elapsedTimeInMS;

			//Process Input
			GameController::HandleInput();

			const unsigned int maxUpdateLoopsPerFrame = 1000; //Don't let the game get stuck updating
			unsigned int updateLoopCounter = 0;
#define MS_PER_UPDATE (1000 / 120) //120 / sec
			while (lag >= MS_PER_UPDATE && updateLoopCounter < maxUpdateLoopsPerFrame)
			{
				//Update Routine... we've moved the code for handling updates to GameController
				GameController::Update();

				lag -= MS_PER_UPDATE;
				++updateLoopCounter;

				//Switch level if queued.
				if (GameController::QueuedNextLevel != nullptr) {
					GameController::SwitchLevel(GameController::QueuedNextLevel);
					GameController::QueuedNextLevel = nullptr;
				}
			}

			//Render Routine... This is very modular. GameController now handles the rendering
			graphics->BeginDraw();
			GameController::Render();
			graphics->EndDraw();

		}
	}

#pragma endregion
	delete graphics;
	return 0;
}