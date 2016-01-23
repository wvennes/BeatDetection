//---------------------------------------------------------------------------
// InputManager.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "InputManager.hpp"


InputManager*	g_theInputManager = nullptr;


//---------------------------------------------------------------------------
LRESULT CALLBACK InputManager_WindowsProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		// these are various ways to close a program, we want to pick them up
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		//g_isQuitting = true;
		g_theInputManager->SetKeyDownState( Input::KEY_ENGINE_SHUTDOWN );
		// we want this action, return 0
		return 0;

	case WM_KEYDOWN:
		g_theInputManager->SetKeyDownState( asKey );
		return 0;

	case WM_KEYUP:
		g_theInputManager->SetKeyUpState( asKey );
		return 0;

	case WM_CHAR:

		return 0;

		break;
	}

	// we need to call this to pass the message along if nothing happened that we want
	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}


//---------------------------------------------------------------------------
InputManager::InputManager( void* platformHandler )
	: m_platformHandler( platformHandler )
{
	HWND hwnd = ( HWND ) m_platformHandler;
	SetWindowLongPtr( hwnd, GWLP_WNDPROC, ( LONG ) InputManager_WindowsProcedure );
}


//---------------------------------------------------------------------------
InputManager::~InputManager()
{

}


//---------------------------------------------------------------------------
void InputManager::SetKeyDownState( const unsigned char input )
{
	KeyState& state = m_keyStates[ input ];

	//state.m_isKeyDown = true;
	state.m_hasKeyBeenPressed = true;
	state.m_hasKeyBeenReleased = false;
}


//---------------------------------------------------------------------------
void InputManager::SetKeyUpState( const unsigned char input )
{
	KeyState& state = m_keyStates[ input ];

	//state.m_isKeyDown = false;
	state.m_hasKeyBeenPressed = false;
	state.m_hasKeyBeenReleased = true;
}


//---------------------------------------------------------------------------
void InputManager::Update()
{
	ResetKeyUpAndKeyDownStates();
	RunMessagePump();
}


//-----------------------------------------------------------------------------------------------
void InputManager::RunMessagePump()
{
	MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}


//---------------------------------------------------------------------------
void InputManager::ResetKeyUpAndKeyDownStates()
{
	for ( int index = 0; index <= 255; ++ index )
	{
		KeyState& state = m_keyStates[ index ];

		if ( state.m_hasKeyBeenPressed ) state.m_isKeyDown = true;
		else if ( state.m_hasKeyBeenReleased ) state.m_isKeyDown = false;

		state.m_hasKeyBeenPressed = false;
		state.m_hasKeyBeenReleased = false;
	}
}


//---------------------------------------------------------------------------
bool InputManager::IsKeyDown( const unsigned char input ) const
{
	return m_keyStates[ input ].m_isKeyDown;
}


//---------------------------------------------------------------------------
bool InputManager::HasKeyBeenPressedOnce( const unsigned char input ) const
{
	return ( HasKeyBeenPressed( input ) && !( IsKeyDown( input ) ) );
}


//---------------------------------------------------------------------------
bool InputManager::HasKeyBeenPressed( const unsigned char input ) const
{
	return m_keyStates[ input ].m_hasKeyBeenPressed;
}

//---------------------------------------------------------------------------
bool InputManager::HasKeyBeenReleased( const unsigned char input ) const
{
	return m_keyStates[ input ].m_hasKeyBeenReleased;
}