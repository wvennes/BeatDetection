//---------------------------------------------------------------------------
// Light.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_LIGHT_
#define _INCLUDED_LIGHT_

#include "Engine/Math/GameMath.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Rgba.hpp"


/////////////////////////////////////////////////////////////////////////////
class Light
{
public:
	// falloff / attentuation
	float	m_innerFalloffRadius;
	float	m_outerFalloffRadius;

	// spotlight
	Vec3f	m_direction;
	float	m_innerApertureDotProduct;
	float	m_outerApertureDotProduct;

	Vec3f	m_position;
	Rgba	m_color; // alpha is brightness

	float	m_ambientness;

public:
	Light();
	Light( Vec3f& position, Rgba& color, float ambientness );
	Light( Vec3f& position, Rgba& color, float ambientness, float innerFalloffRadius, float outerFalloffRadius );
	Light( Vec3f& position, Rgba& color, float ambientness, float innerFalloffRadius, float outerFalloffRadius, Vec3f& direction, Degrees innerAperture, Degrees outerAperture );
	Light( const Light& lightToCopy );
	~Light();

	void SetFalloffRadiusRange( const float innerFalloffRadius, const float outerFalloffRadius );
	void SetApertureRangeDegrees( const Degrees innerAperture, const Degrees outerAperture );
};


//---------------------------------------------------------------------------
inline void Light::SetApertureRangeDegrees( const Degrees innerAperture, const Degrees outerAperture )
{
	m_innerApertureDotProduct = CosDegrees( innerAperture );
	m_outerApertureDotProduct = CosDegrees( outerAperture );
}


//---------------------------------------------------------------------------
inline void Light::SetFalloffRadiusRange( const float innerFalloffRadius, const float outerFalloffRadius )
{
	m_innerFalloffRadius = innerFalloffRadius;
	m_outerFalloffRadius = outerFalloffRadius;
}


#endif