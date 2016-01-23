//---------------------------------------------------------------------------
// ProfileSection.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TimeSystem.hpp"
#include "Engine/Core/ProfileSection.hpp"


//---------------------------------------------------------------------------
ProfileSection::ProfileSection( ProfileCategory profileCategory /* = NUM_PROFILE_CATEGORIES */ )
	: m_profileSectionCategory( profileCategory )
	, m_profileFrameCount( 0 )
	, m_averageTimeForSection( 0.0 )
	, m_totalTimeForSection( 0.0 )
	, m_lastStartTime( 0.0 )
{

}


//---------------------------------------------------------------------------
ProfileSection::~ProfileSection()
{

}


//---------------------------------------------------------------------------
void ProfileSection::StartProfiling()
{
	m_lastStartTime = GetAbsoluteTimeSeconds();
}


//---------------------------------------------------------------------------
void ProfileSection::StopProfiling()
{
	Seconds stopTime = GetAbsoluteTimeSeconds();
	Seconds elapsedTime = stopTime - m_lastStartTime;

	m_totalTimeForSection += elapsedTime;
	++ m_profileFrameCount;

	if ( m_profileFrameCount > 0 ) m_averageTimeForSection = m_totalTimeForSection / ( double ) ( m_profileFrameCount );
}