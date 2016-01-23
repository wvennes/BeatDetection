//---------------------------------------------------------------------------
// ** Vec4.hpp **
// * Template for 4 dimensional vectors
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_VEC4_
#define _INCLUDED_VEC4_

template < typename T >
class Vec4
{
	//-----------------------------------------------------------------------------------------------
public:
	T	x;
	T	y;
	T	z;
	T	w;

	Vec4();
	Vec4( T x, T y, T z, T w );
	~Vec4();

	Vec4< T > operator=( const Vec4< T >& rightHandSide );
	Vec4< T > operator+( const Vec4< T >& rightHandSide ) const;
	Vec4< T > operator-( const Vec4< T >& rightHandSide ) const;
	Vec4< T > operator*( const T& rightHandSide ) const;
	Vec4< T > operator+=( const Vec4< T >& rightHandSide );
	Vec4< T > operator-=( const Vec4< T >& rightHandSide );
	bool operator==( const Vec4< T >& rightHandSide ) const;

	T CalculateDistance() const;
	T Normalize();
 
	T DotProduct( const Vec4< T >& rightHandSide ) const;
// 	Vec4< T > CrossProduct( const Vec4< T >& rightHandSide );
	//-----------------------------------------------------------------------------------------------
};
typedef Vec4< float > Vec4f;
typedef Vec4< unsigned char > Vec4uc;
typedef Vec4f Quaternion;
// typedef Vec4< double > Vec4d;
typedef Vec4< int > Vec4i;
// typedef Vec4< unsigned char > Vec4uc;

//---------------------------------------------------------------------------
// Template Implementations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template < typename T >
Vec4< T >::Vec4() 
	: x( 0 )
	, y( 0 )
	, z( 0 )
	, w( 0 )
{

}

//---------------------------------------------------------------------------
template < typename T >
Vec4< T >::Vec4( T x, T y, T z, T w )
	: x( x )
	, y( y )
	, z( z )
	, w( w )
{

}

//---------------------------------------------------------------------------
template < typename T >
Vec4< T >::~Vec4()
{

}

//---------------------------------------------------------------------------
template < typename T >
inline Vec4< T > Vec4< T >::operator=( const Vec4< T >& rightHandSide ) 
{ 
	return Vec4( this->x = rightHandSide.x, this->y = rightHandSide.y, this->z = rightHandSide.z, this->w = rightHandSide.w ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec4< T > Vec4< T >::operator+( const Vec4< T >& rightHandSide ) const
{
	return Vec4( this->x + rightHandSide.x, this->y + rightHandSide.y, this->z + rightHandSide.z, this->w + rightHandSide.w ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec4< T > Vec4< T >::operator+=( const Vec4< T >& rightHandSide )
{
	return Vec4( this->x += rightHandSide.x, this->y += rightHandSide.y, this->z += rightHandSide.z, this->w += rightHandSide.w ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec4< T > Vec4< T >::operator-( const Vec4< T >& rightHandSide ) const
{
	return Vec4( this->x - rightHandSide.x, this->y - rightHandSide.y, this->z - rightHandSide.z, this->w - rightHandSide.w ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec4< T > Vec4< T >::operator-=( const Vec4< T >& rightHandSide ) 
{
	return Vec4( this->x -= rightHandSide.x, this->y -= rightHandSide.y, this->z -= rightHandSide.z, this->w -= rightHandSide.w ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec4< T > Vec4< T >::operator*( const T& rightHandSide ) const
{
	return Vec4( this->x * rightHandSide, this->y * rightHandSide, this->z * rightHandSide, this->w * rightHandSide );
}


//---------------------------------------------------------------------------
template < typename T >
inline bool Vec4< T >::operator==( const Vec4< T >& rightHandSide ) const
{
	return ( this->x == rightHandSide.x ) && ( this->y == rightHandSide.y ) && ( this->z == rightHandSide.z ) && ( this->w == rightHandSide.w );
}

//---------------------------------------------------------------------------
template < typename T >
T Vec4< T >::CalculateDistance() const
{
	T lengthSquared = ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
	return sqrt( lengthSquared );
}

//---------------------------------------------------------------------------
template < typename T >
T Vec4< T >::Normalize()
{
	T lengthSquared = ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );

	if ( lengthSquared > 0 )
	{
		T length = sqrt( lengthSquared );
		T oneOverDistance = 1 / length;
		x = x * oneOverDistance;
		y = y * oneOverDistance;
		z = z * oneOverDistance;
		w = w * oneOverDistance;
		return length;
	}
	else
	{
		return 0;
	}
}

//---------------------------------------------------------------------------
template < typename T >
T Vec4< T >::DotProduct( const Vec4< T >& rightHandSide ) const
{
	T result = 0;

	result = this->x * rightHandSide.x + this->y * rightHandSide.y + this->z * rightHandSide.z + this->w * rightHandSide.w;

	return result;
}
//
////---------------------------------------------------------------------------
//template < typename T >
//Vec4< T > Vec4< T >::CrossProduct( const Vec4< T >& rightHandSide )
//{
//	Vec4 result;
//
//	result.x = this->y * rightHandSide.z - this->z * rightHandSide.y;
//	result.y = this->z * rightHandSide.x - this->x * rightHandSide.z;
//	result.z = this->x * rightHandSide.y - this->y * rightHandSide.x;
//
//	return Vec4;
//}2f;

#endif