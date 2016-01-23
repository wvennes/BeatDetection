#pragma once
#ifndef _INCLUDED_VEC2_
#define _INCLUDED_VEC2_

template < typename T >
class Vec2
{
//-----------------------------------------------------------------------------------------------
public:
	T	x;
	T	y;

	Vec2();
	Vec2( T x, T y );
	~Vec2();

	inline Vec2< T > operator=( const Vec2< T >& rightHandSide );
	inline Vec2< T > operator+( const Vec2< T >& rightHandSide ) const;
	inline Vec2< T > operator-( const Vec2< T >& rightHandSide ) const;
	inline Vec2< T > operator*( const T& rightHandSide ) const;
	inline Vec2< T > operator/( const T& rightHandSide ) const;
	inline Vec2< T > operator+=( const Vec2< T >& rightHandSide );
	inline Vec2< T > operator-=( const Vec2< T >& rightHandSide );
	inline bool operator<( const Vec2< T >& rightHandSide ) const;
	inline bool operator==( const Vec2< T >& rightHandSide ) const;
	inline bool operator!=( const Vec2< T >& rightHandSide ) const;

	T DotProduct( const Vec2< T >& rightHandSide );
	T CalculateDistanceSquared();
	T CalculateDistance();
	T Normalize();
//-----------------------------------------------------------------------------------------------
};
typedef Vec2< float > Vec2f;
typedef Vec2< int > Vec2i;

//---------------------------------------------------------------------------
template < typename T >
Vec2< T >::Vec2()
	: x( 0 )
	, y( 0 )
{

}

//---------------------------------------------------------------------------
template < typename T >
Vec2< T >::Vec2( T x, T y )
	: x( x )
	, y( y )
{

}

//---------------------------------------------------------------------------
template < typename T >
Vec2< T >::~Vec2()
{

}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator=( const Vec2< T > &rightHandSide ) 
{ 
	return Vec2< T >( this->x = rightHandSide.x, this->y = rightHandSide.y ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator+( const Vec2< T > &rightHandSide ) const 
{
	return Vec2< T >( this->x + rightHandSide.x, this->y + rightHandSide.y ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator+=( const Vec2< T > &rightHandSide ) 
{
	return Vec2< T >( this->x += rightHandSide.x, this->y += rightHandSide.y ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator-( const Vec2< T > &rightHandSide ) const 
{
	return Vec2< T >( this->x - rightHandSide.x, this->y - rightHandSide.y ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator-=( const Vec2< T > &rightHandSide ) 
{
	return Vec2< T >( this->x -= rightHandSide.x, this->y -= rightHandSide.y ); 
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator*( const T &rightHandSide ) const
{
	return Vec2< T >( this->x * rightHandSide, this->y * rightHandSide );
}

//---------------------------------------------------------------------------
template < typename T >
inline Vec2< T > Vec2< T >::operator/( const T &rightHandSide ) const
{
	return Vec2< T >( this->x / rightHandSide, this->y / rightHandSide );
}

//---------------------------------------------------------------------------
template < typename T >
inline bool Vec2< T >::operator<( const Vec2< T >& rightHandSide ) const
{
	if ( x != rightHandSide.x )
	{
		return x < rightHandSide.x;
	}
	else
	{
		return y < rightHandSide.y;
	}
}

//---------------------------------------------------------------------------
template < typename T >
inline bool Vec2< T >::operator==( const Vec2< T >& rightHandSide ) const
{
	return ( x == rightHandSide.x ) && ( y == rightHandSide.y );
}

//---------------------------------------------------------------------------
template < typename T >
inline bool Vec2< T >::operator!=( const Vec2< T >& rightHandSide ) const
{
	return ( x != rightHandSide.x ) || ( y != rightHandSide.y );
}

//---------------------------------------------------------------------------
template < typename T >
T Vec2< T >::CalculateDistance()
{
	T lengthSquared = ( x * x ) + ( y * y );
	return sqrt( lengthSquared );
}

//---------------------------------------------------------------------------
template < typename T >
T Vec2< T >::CalculateDistanceSquared()
{
	return x * x + y * y;
}

//---------------------------------------------------------------------------
template < typename T >
T Vec2< T >::Normalize()
{
	T lengthSquared = ( x * x ) + ( y * y );

	if ( lengthSquared > 0.f )
	{
		T length = sqrt( lengthSquared );
		T oneOverDistance = 1.f / length;
		x = x * oneOverDistance;
		y = y * oneOverDistance;
		return length;
	}
	else
	{
		return 0.f;
	}
}

//---------------------------------------------------------------------------
template < typename T >
T Vec2< T >::DotProduct( const Vec2< T >& rightHandSide )
{
	T result = 0;

	result = this->x * rightHandSide.x + this->y * rightHandSide.y;

	return result;
}

#endif