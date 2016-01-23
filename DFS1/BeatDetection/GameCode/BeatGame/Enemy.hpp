//---------------------------------------------------------------------------
// Enemy.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ENEMY_
#define _INCLUDED_ENEMY_

#include "BeatGame/Actor.hpp"

class Material;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class Enemy : public Actor
{
public:
	Enemy( float innerRadius, float outerRadius, float directionDegrees );
	~Enemy();

	bool				Update();
	void				Render( Material* mat );

	static void			IncrementEnemySpeed( float speedUnitsPerSecond );

private:
	static float		s_enemySpeedUnitsPerSecond;
	const static float	s_enemySizeRadius;

	float				m_innerRingRadius;
	float				m_outerRingRadius;
	float				m_directionDegrees;

};

extern VennRenderer* g_theRenderer;

#endif