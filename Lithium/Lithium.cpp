// Lithium.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#pragma warning( push )

#include "Game.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"


// Global Variables:

// current instance
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE	hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    gDebugConsole.Construct();
    gDebugConsole.SetMinMessageLevel(cTkDebugConsole::eDebugConsoleMode::eDebugConsoleMode_Verbose);
    gMemoryManager.Construct();

    Game& lGame = Game::GetInstance();
    

	// Initialize global strings
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	// If the app can't start, exit
	if (!lGame.Construct())
	{
		return FALSE;
	}

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
        
    ImGui_ImplWin32_Init(GetActiveWindow());
    ImGui_ImplDX11_Init(Game::GetInstance().mRenderManager.device, Game::GetInstance().mRenderManager.ctx);
    ImGui::StyleColorsDark();

	// Main message loop:
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	while (msg.message != WM_QUIT ||lGame.ShouldFinishGame()) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
        
        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

		lGame.Update();
		lGame.Render();
	}

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	lGame.Release();
	return (int)msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM
MyRegisterClass(
	HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0; // MAKEINTRESOURCE(IDC_ENGINE);
	wcex.lpszClassName = L"Lithium";
	wcex.hIconSm = 0; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL 
InitInstance(
	HINSTANCE	hInstance, 
	int			nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable
	
	// Define the client area
	RECT rc = { 0, 0, 800, 600 };

	// We need to tell windows the size of the full windows, including border
	// so the rect is bigger
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	if (FALSE)
	{
		//Initializando parametros de ventana a Fullscreen
		DEVMODE screen;
		DWORD dwExStyle;
		dwStyle = WS_OVERLAPPED | WS_POPUP;
		memset(&screen, 0, sizeof(screen));
		screen.dmSize = sizeof(screen);
		screen.dmPelsWidth = rc.right - rc.left;
		screen.dmPelsHeight = rc.bottom - rc.top;
		screen.dmBitsPerPel = 32;
		height = (int)GetSystemMetrics(SM_CYSCREEN);
		width = (int)GetSystemMetrics(SM_CXSCREEN);
		screen.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&screen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) 
		{
			dwExStyle = 0; // <-- YOU DON'T NEED AN EXTENDED STYLE WHEN IN FULLSCREEN      
			ShowCursor(TRUE);
		}
	}

	// Create the actual window

	hWnd = CreateWindowEx(
        WS_EX_ACCEPTFILES
        , L"Lithium"
        , L"Lithium"
        , dwStyle
        , CW_USEDEFAULT, CW_USEDEFAULT		// Position
        , width				// Width
        , height			    // Height
        , NULL, NULL
        , hInstance
        , NULL);

	if (!hWnd)
		return FALSE;

	Game::GetInstance().mRenderManager.hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK 
WndProc(
	HWND	hWnd, 
	UINT	message, 
	WPARAM	wParam, 
	LPARAM	lParam )
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//SetCursor(NULL);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}