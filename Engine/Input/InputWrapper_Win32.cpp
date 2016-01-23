//---------------------------------------------------------------------------
// InputWrapper_Win32.cpp
//---------------------------------------------------------------------------
#include "InputWrapper.hpp"


//---------------------------------------------------------------------------
int WrappedShowCursor( bool bShow )
{
	return ShowCursor( bShow );
}


//---------------------------------------------------------------------------
Vec2f WrappedGetMouseMovementSinceLastChecked( long centerX, long centerY )
{
	POINT centerCursorPos = { centerX, centerY };
	POINT cursorPos;
	GetCursorPos( &cursorPos );
	SetCursorPos( centerCursorPos.x, centerCursorPos.y );
	Vec2i mouseDeltaInts( cursorPos.x - centerCursorPos.x, cursorPos.y - centerCursorPos.y );
	Vec2f mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}