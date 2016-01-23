//-----------------------------------------------------------------------------------------------
// GameMath.hpp
//-----------------------------------------------------------------------------------------------
#pragma once
#ifndef _INCLUDED_GAMEMATH_
#define _INCLUDED_GAMEMATH_

#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"

typedef float	Degrees;
typedef float	Radians;

Radians DegreesToRadians( const Degrees convertingValue );

Degrees RadiansToDegrees( const Radians convertingValue );
Degrees ComputeShortestAngularDisplacement( Degrees angleToGoal );

float Clamp( float clampingValue, const float minimumValue, const float maximumValue );
float SinDegrees( const Degrees angle );
float CosDegrees( const Degrees angle );

int GetRandomIntInRange( int min, int max );

float MapValueInOldRangeToNewRange( float value, float oldMin, float oldMax, float newMin, float newMax );

bool ComputeSurfaceTangentsAtVertex( Vec3f& surfaceTangentAtVertex_out, Vec3f& surfaceBitangentAtVertex_out, const Vec3f& normalAtThisVertex, const Vec3f& positionOfThisVertex, const Vec2f& texCoordsOfThisVertex, const Vec3f& positionOfPreviousAdjacentVertex, const Vec2f& texCoordsOfPreviousAdjacentVertex, const Vec3f& positionOfNextAdjacentVertex, const Vec2f& texCoordsOfNextAdjacentVertex );


#endif