#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "GameMath.hpp"

//-----------------------------------------------------------------------------------------------
Radians DegreesToRadians( const Degrees convertingAngle )
{
	return static_cast< Radians > ( convertingAngle * M_PI/180.f );
}


//-----------------------------------------------------------------------------------------------
Degrees RadiansToDegrees( const Radians convertingAngle )
{
	return static_cast< Degrees > ( convertingAngle * 180.f/M_PI );
}


//-----------------------------------------------------------------------------------------------
float Clamp( float clampingValue, const float minimumValue, const float maximumValue )
{
	if ( clampingValue > maximumValue )
	{
		clampingValue = maximumValue;
	}
	else if ( clampingValue < minimumValue )
	{
		clampingValue = minimumValue;
	}

	return clampingValue;
}


//-----------------------------------------------------------------------------------------------
Degrees ComputeShortestAngularDisplacement( Degrees angleToGoal )
{
	while ( angleToGoal > 180.f )
	{
		angleToGoal -= 360.f;
	}

	while ( angleToGoal < -180.f )
	{
		angleToGoal += 360.f;
	}

	return angleToGoal;
}


//-----------------------------------------------------------------------------------------------
float SinDegrees( const Degrees angle )
{
	return sin( DegreesToRadians( angle ) );
}


//-----------------------------------------------------------------------------------------------
float CosDegrees( const Degrees angle )
{
	return cos( DegreesToRadians( angle ) );
}


//---------------------------------------------------------------------------
int GetRandomIntInRange( int min, int max )
{
	return min + rand() % ( max - min + 1 );
}


//---------------------------------------------------------------------------
float MapValueInOldRangeToNewRange( float value, float oldMin, float oldMax, float newMin, float newMax )
{
	float oldRange = oldMax - oldMin;
	float newRange = newMax - newMin;

	float newValue = ( ( ( value - oldMin ) * newRange ) / oldRange ) + newMin;

	return newValue;
}


//-----------------------------------------------------------------------------------------------
// *** Code by Squirrel Eiserloh ***
//
// Returns TRUE if the Bitangent is "right handed" with respect to the Normal and Tangent;
// i.e. if [Tangent,Bitangent,Normal] is a right-handed basis.  Typically this bool is
// converted to a +/- 1.0 and passed into a vertex shader as the Tangent's "w" coordinate.
// The Bitangent is then computed within the vertex shader using cross product and "w" to
// disambiguate between which of the two possible perpendiculars to use for the Bitangent.
//
bool ComputeSurfaceTangentsAtVertex(
	Vec3f& surfaceTangentAtVertex_out,
	Vec3f& surfaceBitangentAtVertex_out,
	const Vec3f& normalAtThisVertex,
	const Vec3f& positionOfThisVertex,
	const Vec2f& texCoordsOfThisVertex,
	const Vec3f& positionOfPreviousAdjacentVertex,
	const Vec2f& texCoordsOfPreviousAdjacentVertex,
	const Vec3f& positionOfNextAdjacentVertex,
	const Vec2f& texCoordsOfNextAdjacentVertex )
{
	Vec3f vecToPrevious	= positionOfPreviousAdjacentVertex - positionOfThisVertex;
	Vec3f vecToNext		= positionOfNextAdjacentVertex - positionOfThisVertex;

	Vec2f texToPrevious	= texCoordsOfPreviousAdjacentVertex - texCoordsOfThisVertex;
	Vec2f texToNext		= texCoordsOfNextAdjacentVertex - texCoordsOfThisVertex;

	float determinant = ((texToPrevious.x * texToNext.y) - (texToNext.x * texToPrevious.y));

	Vec3f uDirectionInWorldSpace(	
		(texToNext.y * vecToPrevious.x - texToPrevious.y * vecToNext.x),
		(texToNext.y * vecToPrevious.y - texToPrevious.y * vecToNext.y),
		(texToNext.y * vecToPrevious.z - texToPrevious.y * vecToNext.z)
		);

	Vec3f vDirectionInWorldSpace(
		(texToPrevious.x * vecToNext.x - texToNext.x * vecToPrevious.x),
		(texToPrevious.x * vecToNext.y - texToNext.x * vecToPrevious.y),
		(texToPrevious.x * vecToNext.z - texToNext.x * vecToPrevious.z)
		);

	float invDeterminant = 1.0f / determinant;
	uDirectionInWorldSpace *= invDeterminant;
	vDirectionInWorldSpace *= invDeterminant;

	surfaceTangentAtVertex_out = uDirectionInWorldSpace;
	surfaceBitangentAtVertex_out = vDirectionInWorldSpace * 1.f; // NOTE: You should remove this minus sign if your V texture coordinates are using the opposite convention of mine!

	// NOTE: You don't need the following code, unless you intend to reconstitute the Bitangent vector inside the vertex shader (and not pass it in as a vertex attribute)
	Vec3f naturalBitangentFromCross = Vec3f::CrossProduct( normalAtThisVertex, surfaceTangentAtVertex_out );
	float computedBitangentDotNaturalBitangent = Vec3f::DotProduct( surfaceBitangentAtVertex_out, naturalBitangentFromCross );
	return( computedBitangentDotNaturalBitangent >= 0.f );
}