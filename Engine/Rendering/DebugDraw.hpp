//---------------------------------------------------------------------------
// DebugDraw.hpp
// * mini drawing classes for debug purposes
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_DEBUGDRAW_
#define _INCLUDED_DEBUGDRAW_

#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Rgba.hpp"
#include "Engine/Core/EngineCommon.hpp"

class VennRenderer;

extern VennRenderer* g_theRenderer;

enum DrawMode
{
	DEPTH_TEST_ON,
	DEPTH_TEST_OFF,
	DUAL_MODE,
	NUM_DRAWMODES
};
void DrawDebugPoint( const Vec3f& position, const Rgba& color, const float radius, const DrawMode drawMode, float duration = 0.f );
void DrawDebugLine( const Vec3f& startPosition, const Vec3f& endPosition, const Rgba& startColor, const Rgba& endColor, const DrawMode drawMode, float duration = 0.f );
void DrawDebugArrow( const Vec3f& startPosition, const Vec3f& endPosition, const Rgba& startColor, const Rgba& endColor, const DrawMode drawMode, float duration = 0.f );
void DrawDebugAABB( const Vec3f& mins, const Vec3f& maxes, const Rgba& edgeColor, const Rgba& faceColor, const DrawMode drawMode, float duration = 0.f );
void DrawDebugSphere( const Vec3f& center, const Rgba& color, const float radius, const DrawMode drawMode, float durationSeconds = 0.f );

/////////////////////////////////////////////////////////////////////////////
class DebugDrawObject
{
public:
	DebugDrawObject() : m_drawMode( DEPTH_TEST_OFF ), m_durationSeconds( 0.f ) {}
	DebugDrawObject( DrawMode drawMode, float durationSeconds ) : m_drawMode( drawMode ), m_durationSeconds( durationSeconds ) {}
	~DebugDrawObject() {}

	virtual void Render() {}


	void Update()
	{
		m_durationSeconds -= DELTA_TIME;
		Render();
	}

	bool IsDead()
	{
		if ( m_durationSeconds <= 0.f )
		{
			return true;
		}
		return false;
	}

protected:
	DrawMode	m_drawMode;
	float		m_durationSeconds;
};


/////////////////////////////////////////////////////////////////////////////
class DebugPoint : public DebugDrawObject
{
public:
	DebugPoint( const Vec3f& position, const Rgba& color, const float radius, const DrawMode drawMode, const float durationSeconds )
		: m_position( position ),
		m_color( color ),
		m_radius( radius )
	{
		m_drawMode = drawMode;
		m_durationSeconds = durationSeconds;
	}

	void Render() { DrawDebugPoint( m_position, m_color, m_radius, m_drawMode ); }

private:
	float	m_radius;
	Vec3f	m_position;
	Rgba	m_color;
};


/////////////////////////////////////////////////////////////////////////////
class DebugLine : public DebugDrawObject
{
public:
	DebugLine( const Vec3f& startPosition, const Vec3f& endPosition, const Rgba& startColor, const Rgba& endColor, const DrawMode drawMode, const float durationSeconds )
		: m_startPosition( startPosition ), m_endPosition( endPosition ), m_startColor( startColor ), m_endColor( endColor )
	{
		m_drawMode = drawMode;
		m_durationSeconds = durationSeconds;
	}

	void Render() { DrawDebugLine( m_startPosition, m_endPosition, m_startColor, m_endColor, m_drawMode ); }

private:
	Vec3f	m_startPosition;
	Vec3f	m_endPosition;
	Rgba	m_startColor;
	Rgba	m_endColor;
};


/////////////////////////////////////////////////////////////////////////////
class DebugArrow : public DebugDrawObject
{
public:
	DebugArrow( const Vec3f& startPosition, const Vec3f& endPosition, const Rgba& startColor, const Rgba& endColor, const DrawMode drawMode, const float durationSeconds )
		: m_startPosition( startPosition ), m_endPosition( endPosition ), m_startColor( startColor ), m_endColor( endColor )
	{
		m_drawMode = drawMode;
		m_durationSeconds = durationSeconds;
	}

	void Render() { DrawDebugArrow( m_startPosition, m_endPosition, m_startColor, m_endColor, m_drawMode ); }

private:
	Vec3f	m_startPosition;
	Vec3f	m_endPosition;
	Rgba	m_startColor;
	Rgba	m_endColor;
};


/////////////////////////////////////////////////////////////////////////////
class DebugAABB : public DebugDrawObject
{
public:
	DebugAABB( const Vec3f& mins, const Vec3f& maxes, const Rgba& edgeColor, const Rgba& faceColor, const DrawMode drawMode, const float durationSeconds )
		: m_mins( mins ), m_maxes( maxes ), m_edgeColor( edgeColor ), m_faceColor( faceColor )
	{
		m_drawMode = drawMode;
		m_durationSeconds = durationSeconds;
	}

	void Render() { DrawDebugAABB( m_mins, m_maxes, m_edgeColor, m_faceColor, m_drawMode ); }

private:
	Vec3f	m_mins;
	Vec3f	m_maxes;
	Rgba	m_edgeColor;
	Rgba	m_faceColor;
};


/////////////////////////////////////////////////////////////////////////////
class DebugSphere : public DebugDrawObject
{
public:
	DebugSphere( const Vec3f& center, const Rgba& color, const float radius, const DrawMode drawMode, const float durationSeconds )
		: m_center( center ), m_color( color ), m_radius( radius )
	{
		m_drawMode = drawMode;
		m_durationSeconds = durationSeconds;
	}

	void Render() { DrawDebugSphere( m_center, m_color, m_radius, m_drawMode ); }

private:
	Vec3f	m_center;
	Rgba	m_color;
	float	m_radius;
};

#endif