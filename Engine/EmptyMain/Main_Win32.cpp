// Main_Win32.cpp - Author: Squirrel Eiserloh
// Modified for personal use by Will Vennes

//#include "OpenGL.hpp"
#define _CRTDBG_MAP_ALLOC
#include "Engine/Rendering/VennRenderer.hpp"
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
#include <Xinput.h>
#include <cassert>
#include <stdlib.h>
#include <crtdbg.h>
#include <string>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "glu32" )
#pragma comment( lib, "xinput" )
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Time.hpp"
#include "TheApplication.hpp"
//#include "TheProject.hpp"

//-----------------------------------------------------------------------------------------------
// Intentionally not using whatever is here.
#define UNUSED(x) (void)(x);


//-----------------------------------------------------------------------------------------------
bool g_isQuitting = false;
bool g_isGameOver = false;

// const float GAME_WINDOW_X = 1600.f;
// const float GAME_WINDOW_Y = 900.f;

HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Untitled";

// boolean buffer for movement
bool g_keys[256];

TheApplication* g_theApplication = nullptr;
//TheProject* g_theProject = nullptr;
//VennRenderer* g_theRenderer = nullptr;
//XboxControllerHandler* g_controller = nullptr;

//---------------------------------------------------------------------------
// void HandleControllerInput( XboxControllerHandler& controller )
// {
// 	//IsConnected updates the XINPUT_STATE stored in controller
// 	if ( controller.IsConnected() )
// 	{
// 		// update controller state if connected (magnitudes, analog stick normals, etc)
// 		controller.UpdateControllerState();
// 
// 		// Analog input for movement
// 		if ( controller.m_stickMagnitudeLeft > 0.f )
// 		{
// 			g_TheProject->InputAnalog( controller.m_stickDegreesLeft, controller.m_stickMagnitudeLeft );
// 		}
// 	}
// }


//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		// these are various ways to close a program, we want to pick them up
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			g_isQuitting = true;
			// we want this action, return 0
			return 0;

		case WM_KEYDOWN:
			if( asKey == VK_ESCAPE )
			{
				g_isQuitting = true;
				// we want this action, return 0, don't return window proc
			}

			g_keys[asKey] = true;
			return 0;
			
		case WM_KEYUP:
			g_keys[asKey] = false;

			return 0;

		case WM_CHAR:

			return 0;

			break;
	}

	// we need to call this to pass the message along if nothing happened that we want
	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}

//-----------------------------------------------------------------------------------------------
void CreateApplicationWindow( HINSTANCE applicationInstanceHandle )
{
	SetProcessDPIAware();

	// Define a window class
	WNDCLASSEX windowClassDescription;
	// clear to 0
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	// fill out the structure
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	// telling windows that this is the function we want to use for windows messages.
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	// register as a window class
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { 50 + 0, 50 + 0, 50 + ( long ) GAME_WINDOW_X, 50 + ( long ) GAME_WINDOW_Y };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle)/sizeof(windowTitle[0]) );
	// Real meat of the window - creates a window. everything before this are properties of the window
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );

	g_displayDeviceContext = GetDC( g_hWnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	// setting buffers
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;	// color framebuffer
	pixelFormatDescriptor.cDepthBits	= 32;	// depth framebuffer
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	// once we create the window, we need to create an opengl context
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	// makes this context current
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );
// 
// 	// make our coordinate system here - change to orthographic projection

	g_theApplication = new TheApplication( g_hWnd, g_displayDeviceContext );
}


//-----------------------------------------------------------------------------------------------
void Update()
{
	// Xbox controller movement
/*	HandleControllerInput( *g_controller );*/

//	g_isGameOver = g_theApplication->Update();
	//g_isGameOver = g_theProject->Update();
}


//-----------------------------------------------------------------------------------------------
void Render()
{
//	g_theApplication->Render();


}


//-----------------------------------------------------------------------------------------------
void RunFrame()
{
// 	RunMessagePump();
// 	//g_theApplication->RunFrame();
// 	Update();
// 	Render();
}


//-----------------------------------------------------------------------------------------------
void Initialize(HINSTANCE applicationInstanceHandle)
{
	CreateApplicationWindow( applicationInstanceHandle );

//  	g_theProject = new TheProject();
// 	g_controller = new XboxControllerHandler( 0 );

	//g_theApplication = new TheApplication();


// 	while( !g_isQuitting && !g_isGameOver )
// 	{
// 		RunFrame();
// 	}
}


// /
// struct AtExit
// {
// 	~AtExit() { _CrtDumpMemoryLeaks(); }
// } doAtExit;


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	// set debug flags for memory leaks here to avoid false positives (static map from texture)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF);
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 16016;

	{
		UNUSED( commandLineString );

		Initialize( applicationInstanceHandle );

		g_theApplication->Run();

		delete g_theApplication;
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}