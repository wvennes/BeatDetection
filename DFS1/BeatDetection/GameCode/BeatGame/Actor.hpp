//---------------------------------------------------------------------------
// Actor.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ACTOR_
#define _INCLUDED_ACTOR_

#include <vector>
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Math/Vec2.hpp"

typedef Vec2f						Position;
typedef Vec2f						Direction;
typedef std::vector< Vertex3D >		Vertexes;

class Material;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class Actor
{
public:
	Actor();
	~Actor();

	virtual void		ProcessInput() {}
	virtual bool		Update() { return false; }
	virtual void		Render( Material* ) {}

	bool				DidUpdate() const { return m_didUpdate; }
	bool				IsDead() const { return m_isDead; }

	Vec2f				GetPosition() const { return m_position; }

	void				SetIsDead( bool isDead ) { m_isDead = isDead; }
	void				SetColor( const Rgba& color );

	static bool			IsCollidingWithActor( const Actor& lhs, const Actor& rhs );
	static bool			IsCollidingWithRing( const Actor& actor, const Vec2f& ringPosition, const float ringRadius );

protected:
	bool				m_didUpdate;
	bool				m_isDead;
	float				m_speedUnitsPerSecond;
	float				m_hitboxRadius;
	Position			m_position;
	Direction			m_direction;
	Vertexes			m_vertexes;

};

extern VennRenderer*	g_theRenderer;

#endif