//---------------------------------------------------------------------------
// PulseRing.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PULSERING_
#define _INCLUDED_PULSERING_

#include "BeatGame/Explosion.hpp"

/////////////////////////////////////////////////////////////////////////////
class PulseRing : public Explosion
{
public:
	PulseRing( const Vec2f& position, float radius, float radiusIncrementPerSecond, float lifetimeSeconds, const Rgba& color );
	~PulseRing();

	bool	Update();
	void	Render( Material* mat );

private:


};

#endif