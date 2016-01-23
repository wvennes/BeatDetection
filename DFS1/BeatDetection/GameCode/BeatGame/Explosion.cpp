//---------------------------------------------------------------------------
// Explosion.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"

#include "BeatGame/Explosion.hpp"


//---------------------------------------------------------------------------
Explosion::Explosion()
	: m_currentRadius( 0.f )
	, m_radiusIncrementsPerSecond( 0.f )
	, m_lifetimeSeconds( 0.f )
	, m_currentSeconds( 0.f )
	, m_position( 0.f, 0.f )
	, m_color( 0, 0, 0, 1 )
{

}


//---------------------------------------------------------------------------
Explosion::Explosion( const Vec2f& position, float radius, float radiusIncrementPerSecond, float lifetimeSeconds, const Rgba& color )
	: m_currentRadius( radius )
	, m_radiusIncrementsPerSecond( radiusIncrementPerSecond )
	, m_lifetimeSeconds( lifetimeSeconds )
	, m_currentSeconds( 0.f )
	, m_position( position )
	, m_color( color )
{

}


//---------------------------------------------------------------------------
Explosion::~Explosion()
{

}


//---------------------------------------------------------------------------
bool Explosion::Update()
{
	m_currentSeconds += DELTA_TIME;
	if ( m_currentSeconds >= m_lifetimeSeconds ) return true;

	return false;
}