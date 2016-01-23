//-----------------------------------------------------------------------------------------------
// XboxControllerHandler.hpp - Unfinished handler for xinput controller
// 
//-----------------------------------------------------------------------------------------------
#pragma once
#ifndef _INCLUDED_XBOXCONTROLLERHANDLER_
#define _INCLUDED_XBOXCONTROLLERHANDLER_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include "Engine/Math/GameMath.hpp"
#include "Engine/Math/Vec2.hpp"


/////////////////////////////////////////////////////////////////////////////
enum Button
{
	BUTTON_DPAD_UP					/* = 0x0001 */ = 0,
	BUTTON_DPAD_DOWN				/* = 0x0002 */ = 1,
	BUTTON_DPAD_LEFT				/* = 0x0004 */ = 2,
	BUTTON_DPAD_RIGHT				/* = 0x0008 */ = 3,
	BUTTON_GAMEPAD_START			/* = 0x0010 */ = 4,
	BUTTON_GAMEPAD_BACK				/* = 0x0020 */ = 5,
	BUTTON_GAMEPAD_LEFT_THUMB		/* = 0x0040 */ = 6,
	BUTTON_GAMEPAD_RIGHT_THUMB		/* = 0x0080 */ = 7,
	BUTTON_GAMEPAD_LEFT_SHOULDER	/* = 0x0100 */ = 8,
	BUTTON_GAMEPAD_RIGHT_SHOULDER	/* = 0x0200 */ = 9,
	BUTTON_GAMEPAD_A				/* = 0x1000 */ = 10,
	BUTTON_GAMEPAD_B				/* = 0x2000 */ = 11,
	BUTTON_GAMEPAD_X				/* = 0x4000 */ = 12,
	BUTTON_GAMEPAD_Y				/* = 0x8000 */ = 13,
};


/////////////////////////////////////////////////////////////////////////////
struct ButtonState
{
	bool	m_isButtonDown;
	bool	m_hasButtonBeenPressed;

	ButtonState() : m_isButtonDown( false ), m_hasButtonBeenPressed( false ) {}
};


/////////////////////////////////////////////////////////////////////////////
class XboxControllerHandler
{
public:
	Degrees				m_stickDegreesLeft;
	Degrees				m_stickDegreesRight;
	float				m_stickMagnitudeLeft;
	float				m_stickMagnitudeRight;
	bool				m_hasLeftThumbMoved;
	bool				m_hasRightThumbMoved;

public:
	XboxControllerHandler();
	XboxControllerHandler( int controllerNumber );
	~XboxControllerHandler();

	bool				IsConnected();
	bool				IsButtonDown( WORD button );
	bool				IsButtonDown( const Button button );
	bool				HasButtonBeenPressed( const Button button );
	void				MagnitudeVibration();
	void				SetVibration( WORD vibration );
	void				UpdateControllerState();
	int					GetControllerNumber();

private:
	static const int	MAX_THUMB_VALUE;

	XINPUT_STATE		m_xboxControllerState;
	XINPUT_VIBRATION	m_xboxVibrationState;
	Vec2f				m_normalLeftStick;
	Vec2f				m_normalRightStick;
	int					m_controllerNumber;
	ButtonState			m_buttonStates[ 14 ];

	enum THUMBSTICK
	{
		RIGHT,
		LEFT
	};

	void				UpdateThumbValues( THUMBSTICK leftOrRight );
	float				NormalizeComponent( float thumbstickValue, float deadzone );
	void				CalculateStickDegrees( THUMBSTICK leftOrRight );
	void				CalculateStickMagnitude( THUMBSTICK leftOrRight );
	void				SetButtonDownState( const Button button );
	void				SetButtonUpState( const Button button );

	void				ResetButtonStates();
	void				UpdateButtonStates();
};

extern XboxControllerHandler	g_xboxHandlerOne;

#endif