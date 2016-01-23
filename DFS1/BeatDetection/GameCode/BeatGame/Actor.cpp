//---------------------------------------------------------------------------
// Actor.cpp
//---------------------------------------------------------------------------

#include "../BeatGame/Actor.hpp"

#define STATIC


//---------------------------------------------------------------------------
Actor::Actor()
	: m_didUpdate( false )
	, m_isDead( false )
	, m_speedUnitsPerSecond( 0.f )
	, m_hitboxRadius( 0.f )
	, m_position( 0.f, 0.f )
	, m_direction( 1.f, 0.f )
{

}


//---------------------------------------------------------------------------
Actor::~Actor()
{

}


//---------------------------------------------------------------------------
void Actor::SetColor( const Rgba& color )
{
	for ( unsigned int index = 0; index < ( unsigned int ) m_vertexes.size(); ++ index )
	{
		m_vertexes[ index ].m_color = color;
	}
}


//---------------------------------------------------------------------------
STATIC bool Actor::IsCollidingWithActor( const Actor& lhs, const Actor& rhs )
{
	Vec2f displacement = lhs.m_position - rhs.m_position;
	float distanceSquared = displacement.CalculateDistanceSquared();
	float radiiSquared = ( lhs.m_hitboxRadius + rhs.m_hitboxRadius ) * ( lhs.m_hitboxRadius + rhs.m_hitboxRadius );

	return distanceSquared <= radiiSquared;
}


//---------------------------------------------------------------------------
STATIC bool Actor::IsCollidingWithRing( const Actor& actor, const Vec2f& ringPosition, const float ringRadius )
{
	Vec2f displacement = actor.m_position - ringPosition;
	float distanceSqured = displacement.CalculateDistanceSquared();
	float radiiSquared = ( actor.m_hitboxRadius + ringRadius ) * ( actor.m_hitboxRadius + ringRadius );

	return distanceSqured <= radiiSquared;
}