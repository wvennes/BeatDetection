//---------------------------------------------------------------------------
// InputWrapper.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_INPUTWRAPPER_
#define _INCLUDED_INPUTWRAPPER_

//#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Math/Vec2.hpp"

//---------------------------------------------------------------------------
extern enum InputKey
{
	INPUT_1 = '1',
	INPUT_2 = '2',
	INPUT_3 = '3',
	INPUT_4 = '4',
	INPUT_8 = '8',
	INPUT_9 = '9',
	INPUT_0 = '0',
	INPUT_A = 'A',
	INPUT_C = 'C',
	INPUT_D = 'D',
	INPUT_E = 'E',
	INPUT_O = 'O',
	INPUT_R = 'R',
	INPUT_S = 'S',
	INPUT_W = 'W',
	INPUT_X = 'X',
	INPUT_F1 = VK_F1,
	INPUT_F2 = VK_F2,
	INPUT_F3 = VK_F3,
	INPUT_F4 = VK_F4,
	INPUT_F5 = VK_F5,
	INPUT_F6 = VK_F6,
	INPUT_RETURN = VK_RETURN,
	INPUT_BACKSPACE = VK_BACK,
	INPUT_SPACE = VK_SPACE,
	INPUT_SHIFT = VK_SHIFT,
	INPUT_LEFT = VK_LEFT,
	INPUT_RIGHT = VK_RIGHT,
	INPUT_UP = VK_UP,
	INPUT_DOWN = VK_DOWN,
	INPUT_END = VK_END,
	INPUT_HOME = VK_HOME,
	INPUT_DELETE = VK_DELETE,
	INPUT_TILDE = 192,
	INPUT_ESCAPE = VK_ESCAPE
};


//---------------------------------------------------------------------------
int WrappedShowCursor( bool bShow );
Vec2f WrappedGetMouseMovementSinceLastChecked( long centerX, long centerY );

//#endif

#endif