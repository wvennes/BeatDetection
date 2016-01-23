//---------------------------------------------------------------------------
// Rgba.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_RGBA_
#define _INCLUDED_RGBA_

class Rgba
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

public:
	Rgba() : r( 0 ), g( 0 ), b( 0 ), a( 0 ) {}
	Rgba( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha ) : r( red ), g( green ), b( blue ), a( alpha ) {}
	Rgba( const Rgba& otherRgba ) : r( otherRgba.r ), g( otherRgba.g ), b( otherRgba.b ), a( otherRgba.a ) {}

	Rgba operator=( const Rgba& rightHandSide );
	bool operator==( const Rgba& rightHandSide ) const;
	bool operator!=( const Rgba& rightHandSide ) const;
};


//---------------------------------------------------------------------------
inline Rgba Rgba::operator=( const Rgba& rightHandSide )
{
	return Rgba( r = rightHandSide.r, g = rightHandSide.g, b = rightHandSide.b, a = rightHandSide.a );
}


//---------------------------------------------------------------------------
inline bool Rgba::operator==( const Rgba& rightHandSide ) const
{
	return ( ( r == rightHandSide.r ) && ( g == rightHandSide.g ) && ( b == rightHandSide.b ) && ( a == rightHandSide.a ) );
}


//---------------------------------------------------------------------------
inline bool Rgba::operator!=( const Rgba& rightHandSide ) const
{
	return !( *this == rightHandSide );
}

#endif