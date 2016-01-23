//---------------------------------------------------------------------------
// Bullet.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BULLET_
#define _INCLUDED_BULLET_

#include "BeatGame/Actor.hpp"

/////////////////////////////////////////////////////////////////////////////
class Bullet : public Actor
{
public:
	Bullet( float innerRadius, float outerRadius, float directionDegrees );
	~Bullet();

	bool				Update();
	void				Render( Material* mat );

private:
	const static float	s_bulletSpeedUnitsPerSecond;
	const static float	s_bulletHitboxRadius;
	const static float	s_bulletSizeRadius;
	
	float				m_innerRingRadius;
	float				m_outerRingRadius;
	float				m_directionDegrees;
	float				m_totalDistanceTraveled;

};

#endif