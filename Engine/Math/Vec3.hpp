//---------------------------------------------------------------------------
// ** Vec3.hpp **
// * Template for 3 dimensional vectors
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_VEC3_
#define _INCLUDED_VEC3_

#include <math.h>

template < typename T >
class Vec3
{
//-----------------------------------------------------------------------------------------------
public:
	T	x;
	T	y;
	T	z;

	Vec3();
	Vec3( T x, T y, T z );
	~Vec3();

	inline Vec3< T > operator=( const Vec3< T >& rightHandSide );
	inline Vec3< T > operator+( const Vec3< T >& rightHandSide ) const;
	inline Vec3< T > operator-( const Vec3< T >& rightHandSide ) const;
	inline Vec3< T > operator*( const T& rightHandSide ) const;
	inline Vec3< T > operator*=( const T& rightHandSide );
	inline Vec3< T > operator+=( const Vec3< T >& rightHandSide );
	inline Vec3< T > operator-=( const Vec3< T >& rightHandSide );
	inline bool operator==( const Vec3< T >& rightHandSide ) const;
	inline bool operator!=( const Vec3< T >& rightHandSide ) const;

	inline T CalculateDistance();
	inline T CalculateDistanceSquared();
	T Normalize();

	T DotProduct( const Vec3< T >& rightHandSide ) const;
	static T DotProduct( const Vec3< T >& leftHandSide, const Vec3< T >& rightHandSide );
	Vec3< T > CrossProduct( const Vec3< T >& rightHandSide );
	static Vec3< T > CrossProduct( const Vec3< T >& leftHandSide, const Vec3< T >& rightHandSide );
	//-----------------------------------------------------------------------------------------------
};
typedef Vec3< float > Vec3f;
typedef Vec3< double > Vec3d;
typedef Vec3< int > Vec3i;
typedef Vec3< unsigned char > Vec3uc;

//---------------------------------------------------------------------------
// Template Implementations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template < typename T >
Vec3< T >::Vec3()
	: x( 0 )
	, y( 0 )
	, z( 0 )
{

}

//---------------------------------------------------------------------------
template < typename T >
Vec3< T >::Vec3( T x, T y, T z )
	: x( x )
	, y( y )
	, z( z )
{

}

//---------------------------------------------------------------------------
template < typename T >
Vec3< T >::~Vec3()
{

}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator=( const Vec3< T >& rightHandSide ) 
{ 
	return Vec3( this->x = rightHandSide.x, this->y = rightHandSide.y, this->z = rightHandSide.z ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator+( const Vec3< T >& rightHandSide ) const
{
	return Vec3( this->x + rightHandSide.x, this->y + rightHandSide.y, this->z + rightHandSide.z ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator+=( const Vec3< T >& rightHandSide ) 
{
	return Vec3( this->x += rightHandSide.x, this->y += rightHandSide.y, this->z += rightHandSide.z ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator-( const Vec3< T >& rightHandSide ) const
{
	return Vec3( this->x - rightHandSide.x, this->y - rightHandSide.y, this->z - rightHandSide.z ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator-=( const Vec3< T >& rightHandSide ) 
{
	return Vec3( this->x -= rightHandSide.x, this->y -= rightHandSide.y, this->z -= rightHandSide.z ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator*( const T& rightHandSide ) const
{
	return Vec3( this->x * rightHandSide, this->y * rightHandSide, this->z * rightHandSide );
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec3< T > Vec3< T >::operator*=( const T& rightHandSide )
{
	return Vec3< T >( this->x *= rightHandSide, this->y *= rightHandSide, this->z *= rightHandSide );
}

//---------------------------------------------------------------------------
template < typename T >
inline bool Vec3< T >::operator==( const Vec3< T >& rightHandSide ) const
{
	return ( x == rightHandSide.x && y == rightHandSide.y && z == rightHandSide.z );
}

//---------------------------------------------------------------------------
template < typename T >
inline bool Vec3< T >::operator!=( const Vec3< T >& rightHandSide ) const
{
	return ( x != rightHandSide.x || y != rightHandSide.y || z != rightHandSide.z );
}

//---------------------------------------------------------------------------
template < typename T >
inline T Vec3< T >::CalculateDistance()
{
	T lengthSquared = ( x * x ) + ( y * y ) + ( z * z );
	return sqrt( lengthSquared );
}

//---------------------------------------------------------------------------
template < typename T >
inline T Vec3< T >::CalculateDistanceSquared()
{
	return ( x * x ) + ( y * y ) + ( z * z );
}

//---------------------------------------------------------------------------
template < typename T >
T Vec3< T >::Normalize()
{
	T lengthSquared = ( x * x ) + ( y * y ) + ( z * z );

	if ( lengthSquared > 0 )
	{
		T length = sqrt( lengthSquared );
		T oneOverDistance = 1 / length;
		x = x * oneOverDistance;
		y = y * oneOverDistance;
		z = z * oneOverDistance;
		return length;
	}
	else
	{
		return 0;
	}
}

//---------------------------------------------------------------------------
template < typename T >
inline T Vec3< T >::DotProduct( const Vec3< T >& rightHandSide ) const
{
	return x * rightHandSide.x + y * rightHandSide.y + z * rightHandSide.z;
}

//---------------------------------------------------------------------------
template < typename T >
inline T Vec3< T >::DotProduct( const Vec3< T >& leftHandSide, const Vec3< T >& rightHandSide )
{
	return leftHandSide.x * rightHandSide.x + leftHandSide.y * rightHandSide.y + leftHandSide.z * rightHandSide.z;
}

//---------------------------------------------------------------------------
template < typename T >
Vec3< T > Vec3< T >::CrossProduct( const Vec3< T >& rightHandSide )
{
	Vec3 result;

	result.x = this->y * rightHandSide.z - this->z * rightHandSide.y;
	result.y = this->z * rightHandSide.x - this->x * rightHandSide.z;
	result.z = this->x * rightHandSide.y - this->y * rightHandSide.x;

	return result;
}

//---------------------------------------------------------------------------
template < typename T >
Vec3< T > Vec3< T >::CrossProduct( const Vec3< T >& leftHandSide, const Vec3< T >& rightHandSide )
{
	Vec3 result;

	result.x = leftHandSide.y * rightHandSide.z - leftHandSide.z * rightHandSide.y;
	result.y = leftHandSide.z * rightHandSide.x - leftHandSide.x * rightHandSide.z;
	result.z = leftHandSide.x * rightHandSide.y - leftHandSide.y * rightHandSide.x;

	return result;
}

#endif