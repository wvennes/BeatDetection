//---------------------------------------------------------------------------
// Explosion.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_EXPLOSION_
#define _INCLUDED_EXPLOSION_

#include <vector>
#include "Engine/Rendering/Vertex3D.hpp"

typedef std::vector< Vertex3D >		Vertexes;
class Material;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class Explosion
{
public:
	Explosion();
	Explosion( const Vec2f& position, float radius, float radiusIncrementPerSecond, float lifetimeSeconds, const Rgba& color );
	~Explosion();

	virtual bool	Update();
	virtual void	Render( Material* ) {}

protected:
	float			m_currentRadius;
	float			m_radiusIncrementsPerSecond;
	float			m_lifetimeSeconds;
	float			m_currentSeconds;
	Vec2f			m_position;
	Rgba			m_color;
	Vertexes		m_vertexes;

};

extern VennRenderer*	g_theRenderer;

#endif