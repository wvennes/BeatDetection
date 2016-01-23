//---------------------------------------------------------------------------
// Alarm.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ALARM_
#define _INCLUDED_ALARM_

#include <string>

#include "Engine/Core/StringTable.hpp"


/////////////////////////////////////////////////////////////////////////////
class Alarm
{
public:
	Alarm( const char* alarmName, double durationSeconds, bool isRepeating = false );
	~Alarm();

	void			AdvanceAlarm( double deltaSeconds ) { m_secondsRemaining -= deltaSeconds; }

	double			GetSecondsElapsed() const { return m_totalDuration - m_secondsRemaining; }
	double			GetSecondsRemaining() const { return m_secondsRemaining; }
	double			GetPercentElapsed() const { return ( GetSecondsElapsed() / m_totalDuration ) * 100.0; }
	double			GetPercentRemaining() const { return ( GetSecondsRemaining() / m_totalDuration ) * 100.0; }
	bool			IsAlarmRepeating() const { return m_isRepeating; }
	int				GetAlarmNameID() const { return m_alarmNameID; }
	std::string		GetAlarmName() const { return StringValue( m_alarmNameID ); }

	bool			IsAlarmFinished();

private:
	bool			m_isRepeating;
	int				m_alarmNameID;
	double			m_totalDuration;
	double			m_secondsRemaining;

};

#endif