//-----------------------------------------------------------------------------------------------
// XboxControllerHandler.cpp - unfinished xinput controller handler
// - to be finished for engine
//-----------------------------------------------------------------------------------------------
#include <iostream>
#include "Engine/Input/XboxControllerHandler.hpp"
#define STATIC

STATIC const int XboxControllerHandler::MAX_THUMB_VALUE = 32767;

XboxControllerHandler g_xboxHandlerOne = XboxControllerHandler( 0 );
WORD g_xinputButtons[] = 
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};


//-----------------------------------------------------------------------------------------------
XboxControllerHandler::XboxControllerHandler()
	: m_stickDegreesLeft( 0.f )
	, m_stickDegreesRight( 0.f )
	, m_stickMagnitudeLeft( 0.f )
	, m_stickMagnitudeRight( 0.f )
	, m_hasLeftThumbMoved( false )
	, m_hasRightThumbMoved( false )
	, m_normalLeftStick( Vec2f( 0.f, 0.f ) )
	, m_normalRightStick( Vec2f( 0.f, 0.f ) )
	, m_controllerNumber( 0 )
{

}


//-----------------------------------------------------------------------------------------------
XboxControllerHandler::XboxControllerHandler( int controllerNumber )
	: m_stickDegreesLeft( 0.f )
	, m_stickDegreesRight( 0.f )
	, m_stickMagnitudeLeft( 0.f )
	, m_stickMagnitudeRight( 0.f )
	, m_hasLeftThumbMoved( false )
	, m_hasRightThumbMoved( false )
	, m_normalLeftStick( Vec2f( 0.f, 0.f ) )
	, m_normalRightStick( Vec2f( 0.f, 0.f ) )
	, m_controllerNumber( controllerNumber )
{

}


//-----------------------------------------------------------------------------------------------
XboxControllerHandler::~XboxControllerHandler()
{

}


//-----------------------------------------------------------------------------------------------
bool XboxControllerHandler::IsConnected()
{
	memset(&m_xboxControllerState, 0, sizeof(XINPUT_STATE));
	if ( XInputGetState( m_controllerNumber, &m_xboxControllerState ) == ERROR_SUCCESS)
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
void XboxControllerHandler::UpdateControllerState()
{
	if ( !IsConnected() ) return;
	//ResetButtonStates();
	UpdateButtonStates();
	// update normalized values for L Thumbstick - adding R thumbstick and triggers later
	UpdateThumbValues( LEFT );
	UpdateThumbValues( RIGHT );
}


//-----------------------------------------------------------------------------------------------
void XboxControllerHandler::UpdateThumbValues( THUMBSTICK leftOrRight )
{
	int stickX = 0;
	int stickY = 0;
	Vec2f* normalStick = nullptr;
	float deadzone = 0.f;

	switch ( leftOrRight )
	{
	case LEFT:
		normalStick = &m_normalLeftStick;
		stickX = m_xboxControllerState.Gamepad.sThumbLX;
		stickY = m_xboxControllerState.Gamepad.sThumbLY;
		deadzone = ( float ) XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;

	case RIGHT:
		normalStick = &m_normalRightStick;
		stickX = m_xboxControllerState.Gamepad.sThumbRX;
		stickY = m_xboxControllerState.Gamepad.sThumbRY;
		deadzone = ( float ) XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	}

	normalStick->x = NormalizeComponent( ( float ) stickX, ( float ) deadzone );
	normalStick->y = NormalizeComponent( ( float ) stickY, ( float ) deadzone );

	CalculateStickDegrees( leftOrRight );
	CalculateStickMagnitude( leftOrRight );
}


//-----------------------------------------------------------------------------------------------
float XboxControllerHandler::NormalizeComponent( float thumbstickValue, float deadzone )
{
	float newThumbMaxValue = ( float ) MAX_THUMB_VALUE - deadzone;
	float result;

	thumbstickValue = Clamp( thumbstickValue, ( float ) -MAX_THUMB_VALUE, ( float ) MAX_THUMB_VALUE );

	if ( abs( thumbstickValue ) > deadzone )
	{
		result = ( float ) ( ( abs( thumbstickValue ) - deadzone ) / newThumbMaxValue );
	}
	else
	{
		result = 0.f;
	}

	if ( thumbstickValue < 0 )
	{
		result = result * ( -1.f );
	}

	return result;
}


//-----------------------------------------------------------------------------------------------
bool XboxControllerHandler::IsButtonDown( WORD button )
{
	if ( ( m_xboxControllerState.Gamepad.wButtons & button ) != 0 )
	{
		return true;
	}
	else return false;
}


//---------------------------------------------------------------------------
bool XboxControllerHandler::IsButtonDown( const Button button )
{
	return m_buttonStates[ button ].m_isButtonDown;
}


//---------------------------------------------------------------------------
bool XboxControllerHandler::HasButtonBeenPressed( const Button button )
{
	return ( !m_buttonStates[ button ].m_hasButtonBeenPressed && m_buttonStates[ button ].m_isButtonDown );
}


//-----------------------------------------------------------------------------------------------
void XboxControllerHandler::CalculateStickDegrees( THUMBSTICK leftOrRight )
{
	Degrees angle = 0.f;
	switch ( leftOrRight )
	{
	case LEFT:
		angle = atan2( m_normalLeftStick.y, m_normalLeftStick.x );
		m_stickDegreesLeft = RadiansToDegrees( angle );
		break;

	case RIGHT:
		angle = atan2( m_normalRightStick.y, m_normalRightStick.x );
		m_stickDegreesRight = RadiansToDegrees( angle );
		break;
	}
}


//-----------------------------------------------------------------------------------------------
void XboxControllerHandler::CalculateStickMagnitude( THUMBSTICK leftOrRight )
{
	float magnitudeSquared = 0.f;
	switch ( leftOrRight )
	{
	case LEFT:
		magnitudeSquared = 
			m_normalLeftStick.x * m_normalLeftStick.x + 
			m_normalLeftStick.y * m_normalLeftStick.y;

		m_stickMagnitudeLeft = sqrt( magnitudeSquared );
		Clamp( m_stickMagnitudeLeft, 0.f, 1.f );
		break;

	case RIGHT:
		magnitudeSquared =
			m_normalRightStick.x * m_normalRightStick.x + 
			m_normalRightStick.y * m_normalRightStick.y;

		m_stickMagnitudeRight = sqrt( magnitudeSquared );
		Clamp( m_stickMagnitudeRight, 0.f, 1.f );
		break;
	}
}


//---------------------------------------------------------------------------
void XboxControllerHandler::ResetButtonStates()
{
	for ( int index = 0; index < 14; ++ index )
	{
		ButtonState& state = m_buttonStates[ index ];

		//if ( state.m_hasButtonBeenPressed ) state.m_isButtonDown = true;

		state.m_hasButtonBeenPressed = state.m_isButtonDown;
		state.m_isButtonDown = false;
	}
}


//---------------------------------------------------------------------------
void XboxControllerHandler::UpdateButtonStates()
{
	for ( int index = 0; index < 14; ++ index )
	{
		ButtonState& state = m_buttonStates[ index ];

		state.m_hasButtonBeenPressed = state.m_isButtonDown;
		state.m_isButtonDown = ( g_xinputButtons[ index ] & m_xboxControllerState.Gamepad.wButtons ) == g_xinputButtons[ index ];

// 		if ( state.m_isButtonDown && !state.m_hasButtonBeenPressed )
// 		{
// 			state.m_hasButtonBeenPressed = true;
// 		}
// 		if ( ( g_xinputButtons[ index ] & m_xboxControllerState.Gamepad.wButtons ) == g_xinputButtons[ index ] )
// 		{
// 			if ( !HasButtonBeenPressed( ( Button ) index ) ) SetButtonDownState( ( Button ) index );
// 			else m_buttonStates[ index ].m_hasButtonBeenPressed = false;
// 		}
	}
}


//-----------------------------------------------------------------------------------------------
void XboxControllerHandler::MagnitudeVibration()
{
	memset(&m_xboxVibrationState, 0, sizeof( XINPUT_VIBRATION ) );
	m_xboxVibrationState.wLeftMotorSpeed = ( WORD ) ( m_stickMagnitudeLeft * 60000 );
	m_xboxVibrationState.wRightMotorSpeed = ( WORD ) ( m_stickMagnitudeLeft * 60000 );
	XInputSetState( m_controllerNumber, &m_xboxVibrationState );
}


//-----------------------------------------------------------------------------------------------
void XboxControllerHandler::SetVibration( WORD vibrate )
{
	memset( &m_xboxVibrationState, 0, sizeof( XINPUT_VIBRATION ));
	m_xboxVibrationState.wLeftMotorSpeed = vibrate;
	m_xboxVibrationState.wRightMotorSpeed = vibrate;
	XInputSetState( m_controllerNumber, &m_xboxVibrationState );
}


//---------------------------------------------------------------------------
int XboxControllerHandler::GetControllerNumber()
{
	return m_controllerNumber;
}


//---------------------------------------------------------------------------
void XboxControllerHandler::SetButtonDownState( const Button button )
{
	ButtonState& state = m_buttonStates[ ( unsigned int ) button ];

	state.m_hasButtonBeenPressed = true;
}