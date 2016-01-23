//---------------------------------------------------------------------------
// Clock.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CLOCK_
#define _INCLUDED_CLOCK_

#include <vector>

#include "Engine/Core/Alarm.hpp"

class Clock;
typedef std::vector< Clock* >	ChildClocks;
typedef std::vector< Alarm >	Alarms;

/////////////////////////////////////////////////////////////////////////////
class Clock
{
public:
	Clock( Clock* parent );
	~Clock(); // NEED TO DESTROY CLOCK BEFORE DELETE!

	static void			DestroyRootClock();
	static Clock*		GetRootClock();

	void				DestroyClock();

	bool				GetIsPaused() const { return m_isPaused; }
	float				GetTimeScale() const { return m_timeScale; }
	double				GetCurrentTimeSeconds() const { return m_currentTime; }
	const ChildClocks*	GetClockChildren() const { return &m_children; }
	const Alarms*		GetClockAlarms() const { return &m_alarms; }
	int					GetClockID() const { return m_clockID; }

	void				SetIsPaused( bool isPaused ) { m_isPaused = isPaused; }
	void				ToggleIsPaused() { m_isPaused = !m_isPaused; }
	void				SetTimeScale( float timeScale ) { m_timeScale = timeScale; }
	void				SetAlarm( const char* alarmName, double duration, bool isRepeating = false );

	void				ChangeClockParent( Clock* newParent );
	void				RemoveClockFromHierarchyAndReparentChildren();
	void				AdvanceTime( double deltaSeconds );
	
private:
	static Clock*		s_root;
	static int			s_id;

	bool				m_isPaused;
	int					m_clockID;
	Clock*				m_parent;
	float				m_timeScale;
	double				m_currentTime;

	ChildClocks			m_children;
	Alarms				m_alarms;

private:
	Clock();

	void				RemoveSelfFromParentChildList();
	void				DestroyClock( Clock* node, Clock* root );

};

#endif