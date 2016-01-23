//---------------------------------------------------------------------------
// EnemyDeathExplosion.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ENEMYDEATHEXPLOSION_
#define _INCLUDED_ENEMYDEATHEXPLOSION_

#include "BeatGame/Explosion.hpp"

/////////////////////////////////////////////////////////////////////////////
class EnemyDeathExplosion : public Explosion
{
public:
	EnemyDeathExplosion( const Vec2f& position, float radius, float radiusIncrementPerSecond, float lifetimeSeconds, const Rgba& color );
	~EnemyDeathExplosion();

	bool	Update();
	void	Render( Material* mat );

private:


};

#endif