//---------------------------------------------------------------------------
// World.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_WORLD_
#define _INCLUDED_WORLD_

#include <vector>
#include "Engine/Rendering/Vertex3D.hpp"

class Material;
class VennRenderer;

typedef std::vector< Vertex3D >	InnerRingCoords;
typedef std::vector< Vertex3D >	OuterRingCoords;

/////////////////////////////////////////////////////////////////////////////
class World
{
public:
	World( float innerRingRadius, float outerRingRadius );
	~World();

	void				Render( Material* mat );
	bool				Update();

	float				GetInnerRingRadius() const { return m_innerRingRadius; }
	float				GetOuterRingRadius() const { return m_outerRingRadius; }
	float				GetDestinationRingRadius() const { return m_destinationRingRadius; }
	void				SetInnerRingRadius( float radius ) { m_destinationRingRadius = radius; }
	void				IncrementInnerRingRadius( float radius ) { m_destinationRingRadius += radius; }

	bool				IsRadiusAtMaximum();

private:
	float				m_ringResizeTimer;
	float				m_innerRingRadius;
	float				m_outerRingRadius;
	float				m_destinationRingRadius;

	InnerRingCoords		m_innerRingCoords;
	OuterRingCoords		m_outerRingCoords;

private:
	void				Initialize();
	void				UpdateRingCoords( std::vector< Vertex3D >& ringCoords, float radius );

};

extern VennRenderer*	g_theRenderer;

#endif