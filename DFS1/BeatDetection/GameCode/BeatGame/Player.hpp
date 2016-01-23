//---------------------------------------------------------------------------
// Player.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PLAYER_
#define _INCLUDED_PLAYER_

#include "BeatGame/Actor.hpp"

/////////////////////////////////////////////////////////////////////////////
class Player : public Actor
{
public:
	Player( float ringRadius, float degrees );
	~Player();

	void		ProcessInput();
	bool		Update();
	void		Render( Material* mat );

	bool		DidPlayerPause() const { return m_didPlayerPause; }
	bool		DidPlayerShoot() const { return m_didPlayerShoot; }

	float		GetDirectionDegrees() const { return m_degrees; }

	void		SetRadius( float degrees ) { m_currentRadius = degrees; }

private:
	bool		m_didPlayerPause;
	bool		m_didPlayerShoot;
	float		m_currentRadius;
	float		m_degrees;

};

#endif