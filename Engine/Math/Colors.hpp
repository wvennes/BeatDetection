//---------------------------------------------------------------------------
// Colors.hpp
// * Predefined color values
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_COLORS_
#define _INCLUDED_COLORS_

#include "Engine/Math/Rgba.hpp"

namespace Colors
{
	const static Rgba	WHITE	( 255,		255,	255,	255 );	// 1
	const static Rgba	RED		( 255,		0,		0,		255 );	// 2
	const static Rgba	GREEN	( 0,		255,	0,		255 );	// 3
	const static Rgba	BLUE	( 0,		0,		255,	255 );	// 4
	const static Rgba	YELLOW	( 255,		255,	0,		255 );	// 5
	const static Rgba	PURPLE	( 255,		0,		255,	255 );	// 6
	const static Rgba	CYAN	( 0,		255,	255,	255 );	// 7
	const static Rgba	GREY	( 178,		178,	178,	255 );
	const static Rgba	BLACK	( 0,		0,		0,		255 );
}

#endif