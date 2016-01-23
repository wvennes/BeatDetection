//---------------------------------------------------------------------------
// Alarm.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/Alarm.hpp"


//---------------------------------------------------------------------------
Alarm::Alarm( const char* alarmName, double duration, bool isRepeating /* = false */ )
	: m_isRepeating( isRepeating )
	, m_alarmNameID( StringID( alarmName ) )
	, m_totalDuration( duration )
	, m_secondsRemaining( duration )
{

}


//---------------------------------------------------------------------------
Alarm::~Alarm()
{

}


//---------------------------------------------------------------------------
bool Alarm::IsAlarmFinished()
{
	bool result = false;
	if ( m_secondsRemaining <= 0.0 )
	{
		result = true;

		if ( m_isRepeating ) m_secondsRemaining = m_totalDuration;
	}

	return result;
}