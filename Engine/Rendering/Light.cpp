//---------------------------------------------------------------------------
// Light.cpp
//---------------------------------------------------------------------------

#include "Engine/Math/Colors.hpp"
#include "Engine/Rendering/Light.hpp"


//---------------------------------------------------------------------------
Light::Light()
	: m_position( Vec3f( 0.f, 0.f, 0.f ) ),
	m_color( Colors::WHITE ),
	m_ambientness( 0.f ),
	m_innerFalloffRadius( 9998.f ),
	m_outerFalloffRadius( 9999.f ),
	m_direction( Vec3f( 1.f, 0.f, 0.f ) ),
	m_innerApertureDotProduct( -2.f ),
	m_outerApertureDotProduct( -3.f )
{

}


//---------------------------------------------------------------------------
Light::Light( const Light& lightToCopy )
	: m_position( lightToCopy.m_position ),
	m_color( lightToCopy.m_color ),
	m_ambientness( lightToCopy.m_ambientness ),
	m_innerFalloffRadius( lightToCopy.m_innerFalloffRadius ),
	m_outerFalloffRadius( lightToCopy.m_outerFalloffRadius ),
	m_direction( lightToCopy.m_direction ),
	m_innerApertureDotProduct( lightToCopy.m_innerApertureDotProduct ),
	m_outerApertureDotProduct( lightToCopy.m_outerApertureDotProduct )
{
	m_direction.Normalize();
}


//---------------------------------------------------------------------------
Light::Light( Vec3f& position, Rgba& color, float ambientness )
	: m_position( position ),
	m_color( color ),
	m_ambientness( ambientness ),
	m_innerFalloffRadius( 9998.f ),
	m_outerFalloffRadius( 9999.f ),
	m_direction( Vec3f( 1.f, 0.f, 0.f ) ),
	m_innerApertureDotProduct( -2.f ),
	m_outerApertureDotProduct( -3.f )
{

}


//---------------------------------------------------------------------------
Light::Light( Vec3f& position, Rgba& color, float ambientness, float innerFalloffRadius, float outerFalloffRadius )
	: m_position( position ),
	m_color( color ),
	m_ambientness( ambientness ),
	m_innerFalloffRadius( innerFalloffRadius ),
	m_outerFalloffRadius( outerFalloffRadius ),
	m_direction( Vec3f( 1.f, 0.f, 0.f ) ),
	m_innerApertureDotProduct( -2.f ),
	m_outerApertureDotProduct( -3.f )
{

}


//---------------------------------------------------------------------------
Light::Light( Vec3f& position, Rgba& color, float ambientness, float innerFalloffRadius, float outerFalloffRadius, Vec3f& direction, Degrees innerAperture, Degrees outerAperture )
	: m_position( position ),
	m_color( color ),
	m_ambientness( ambientness ),
	m_innerFalloffRadius( innerFalloffRadius ),
	m_outerFalloffRadius( outerFalloffRadius ),
	m_direction( direction )
{
	SetApertureRangeDegrees( innerAperture, outerAperture );
	direction.Normalize();
}


//---------------------------------------------------------------------------
Light::~Light()
{

}