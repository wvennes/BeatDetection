//---------------------------------------------------------------------------
// InputManager.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_INPUTMANAGER_
#define _INCLUDED_INPUTMANAGER_

/////////////////////////////////////////////////////////////////////////////
namespace Input
{
	enum InputValues // for Windows
	{
		KEY_LBUTTON			= 0x01,
		KEY_RBUTTON			= 0x02,
		KEY_CANCEL			= 0x03,
		KEY_MBUTTON			= 0x04,
		KEY_XBUTTON1		= 0x05,
		KEY_XBUTTON2		= 0x06,
		KEY_BACK			= 0x08,
		KEY_TAB				= 0x09,
		KEY_CLEAR			= 0x0C,
		KEY_RETURN			= 0x0D,
		KEY_SHIFT			= 0x10,
		KEY_CONTROL			= 0x11,
		KEY_MENU			= 0x12,
		KEY_PAUSE			= 0x13,
		KEY_CAPITAL			= 0x14,
		KEY_ESCAPE			= 0x1B,
		KEY_SPACE			= 0x20,
		KEY_PAGEUP			= 0x21,
		KEY_PAGEDOWN		= 0x22,
		KEY_END				= 0x23,
		KEY_HOME			= 0x24,
		KEY_LEFT			= 0x25,
		KEY_UP				= 0x26,
		KEY_RIGHT			= 0x27,
		KEY_DOWN			= 0x28,
		KEY_SELECT			= 0x29,
		KEY_PRINT			= 0x2A,
		KEY_SNAPSHOT		= 0x2C,
		KEY_INSERT			= 0x2D,
		KEY_DELETE			= 0x2E,
		KEY_HELP			= 0x2F,
		KEY_LWIN			= 0x5B,
		KEY_SLEEP			= 0x5F,
		KEY_NUMPAD0			= 0x60,
		KEY_NUMPAD1			= 0x61,
		KEY_NUMPAD2			= 0x62,
		KEY_NUMPAD3			= 0x63,
		KEY_NUMPAD4			= 0x64,
		KEY_NUMPAD5			= 0x65,
		KEY_NUMPAD6			= 0x66,
		KEY_NUMPAD7			= 0x67,
		KEY_NUMPAD8			= 0x68,
		KEY_NUMPAD9			= 0x69,
		KEY_MULTIPLY		= 0x6A,
		KEY_ADD				= 0x6B,
		KEY_SEPARATOR		= 0x6C,
		KEY_SUBTRACT		= 0x6D,
		KEY_DECIMAL			= 0x6E,
		KEY_DIVIDE			= 0x6F,
		KEY_F1				= 0x70,
		KEY_F2				= 0x71,
		KEY_F3				= 0x72,
		KEY_F4				= 0x73,
		KEY_F5				= 0x74,
		KEY_F6				= 0x75,
		KEY_F7				= 0x76,
		KEY_F8				= 0x77,
		KEY_F9				= 0x78,
		KEY_F10				= 0x79,
		KEY_F11				= 0x7A,
		KEY_F12				= 0x7B,
		KEY_F13				= 0x7C,
		KEY_F14				= 0x7D,
		KEY_F15				= 0x7E,
		KEY_F16				= 0x7F,
		KEY_F17				= 0x80,
		KEY_F18				= 0x81,
		KEY_F19				= 0x82,
		KEY_F20				= 0x83,
		KEY_F21				= 0x84,
		KEY_F22				= 0x85,
		KEY_F23				= 0x86,
		KEY_F24				= 0x87,
		KEY_NUMLOCK			= 0x90,
		KEY_SCROLL			= 0x91,
		KEY_LSHIFT			= 0xA0,
		KEY_RSHIFT			= 0xA1,
		KEY_LCONTROL		= 0xA2,
		KEY_RCONTROL		= 0xA3,
		KEY_LMENU			= 0xA4,
		KEY_RMENU			= 0xA5,
		KEY_COLON			= 0xBA,
		KEY_PLUS			= 0xBB,
		KEY_COMMA			= 0xBC,
		KEY_MINUS			= 0xBD,
		KEY_PERIOD			= 0xBE,
		KEY_FORWARDSLASH	= 0xBF,
		KEY_TILDE			= 0xC0,
		KEY_LEFTBRACKET		= 0xDB,
		KEY_BACKSLASH		= 0xDC,
		KEY_RIGHTBRACKET	= 0xDD,
		KEY_QUOTE			= 0xDE,
		KEY_ENGINE_SHUTDOWN	= 0xFF // TODO: Find a better way to handle shutdown events
	};
}


/////////////////////////////////////////////////////////////////////////////
struct KeyState
{
	bool	m_isKeyDown;
	bool	m_hasKeyBeenPressed;
	bool	m_hasKeyBeenReleased;

	KeyState() : m_isKeyDown( false ), m_hasKeyBeenPressed( false ), m_hasKeyBeenReleased( false ) {}
};


/////////////////////////////////////////////////////////////////////////////
class InputManager
{
public:
	InputManager( void* platformHandler );
	~InputManager();

	void		Update();

	void		SetKeyDownState( const unsigned char input );
	void		SetKeyUpState( const unsigned char input );

	bool		IsKeyDown( const unsigned char input ) const;
	bool		HasKeyBeenPressedOnce( const unsigned char input ) const;
	bool		HasKeyBeenPressed( const unsigned char input ) const;
	bool		HasKeyBeenReleased( const unsigned char input ) const;

private:
	void*		m_platformHandler;
	KeyState	m_keyStates[ 256 ];

private:
	void		RunMessagePump();
	void		ResetKeyUpAndKeyDownStates();

};

extern InputManager*	g_theInputManager;

#endif