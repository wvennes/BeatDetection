//---------------------------------------------------------------------------
// Clock.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/TimeSystem.hpp"
#include "Engine/Core/Clock.hpp"

#define STATIC

STATIC Clock*	Clock::s_root = nullptr;
STATIC int		Clock::s_id = 0;


//---------------------------------------------------------------------------
Clock::Clock()
	: m_isPaused( false )
	, m_clockID( s_id ++ )
	, m_parent( nullptr )
	, m_timeScale( 1.f )
	, m_currentTime( GetAbsoluteTimeSeconds() )
{

}


//---------------------------------------------------------------------------
Clock::Clock( Clock* parent )
	: m_isPaused( false )
	, m_clockID( s_id ++ )
	, m_parent( parent )
	, m_timeScale( 1.f )
	, m_currentTime( parent->GetCurrentTimeSeconds() )
{
	parent->m_children.push_back( this );
}


//---------------------------------------------------------------------------
Clock::~Clock()
{
// 	// if there is a parent, remove from their child list
// 	RemoveSelfFromParentChildList();
	
	//TODO: Fix so that parent doesn't hold garbage references
	//DestroyClock();
	//RemoveSelfFromParentChildList();
}


//---------------------------------------------------------------------------
void Clock::DestroyClock()
{
	DestroyClock( this, this );
}


//---------------------------------------------------------------------------
void Clock::DestroyClock( Clock* node, Clock* root )
{
	for ( unsigned int index = 0; index < ( unsigned int ) node->m_children.size(); ++ index )
	{
		DestroyClock( node->m_children[ index ], root );
	}

	if ( node != root )
	{
		delete node;
	}
	else
	{
		RemoveSelfFromParentChildList();
	}
}


//---------------------------------------------------------------------------
STATIC void Clock::DestroyRootClock()
{
	if ( s_root )
	{
		s_root->DestroyClock();
		delete s_root;
	}
	s_root = nullptr;
}


//---------------------------------------------------------------------------
STATIC Clock* Clock::GetRootClock()
{
	if ( !s_root ) s_root = new Clock();
	return s_root;
}


//---------------------------------------------------------------------------
void Clock::SetAlarm( const char* alarmName, double duration, bool isRepeating /* = false */ )
{
	m_alarms.push_back( Alarm( alarmName, duration, isRepeating ) );
}


//---------------------------------------------------------------------------
void Clock::ChangeClockParent( Clock* newParent )
{
	// remove from child list of old parent if exists
	RemoveSelfFromParentChildList();

	m_parent = newParent;
}


//---------------------------------------------------------------------------
void Clock::RemoveClockFromHierarchyAndReparentChildren()
{
	for ( ChildClocks::iterator iter = m_children.begin(); iter != m_children.end(); ++ iter )
	{
		( *iter )->ChangeClockParent( m_parent );
	}

	ChangeClockParent( nullptr );
}


//---------------------------------------------------------------------------
void Clock::RemoveSelfFromParentChildList()
{
	if ( !m_parent ) return;

	ChildClocks& parentChildVector = m_parent->m_children;
	for ( ChildClocks::iterator iter = parentChildVector.begin(); iter != parentChildVector.end(); ++ iter )
	{
		if ( *iter == this )
		{
			parentChildVector.erase( iter );
			break;
		}
	}
}


//---------------------------------------------------------------------------
void Clock::AdvanceTime( double deltaSeconds )
{
	if ( m_isPaused ) return;

	double internalDeltaSeconds = deltaSeconds * ( double ) ( m_timeScale );
	m_currentTime += internalDeltaSeconds;

	for ( Alarms::iterator alarmIter = m_alarms.begin(); alarmIter != m_alarms.end(); )
	{
		Alarm& currentAlarm = *alarmIter;
		currentAlarm.AdvanceAlarm( internalDeltaSeconds );

		if ( currentAlarm.IsAlarmFinished() && !currentAlarm.IsAlarmRepeating() )
		{
			alarmIter = m_alarms.erase( alarmIter );
		}
		else
		{
			++ alarmIter;
		}
	}

	for ( ChildClocks::iterator clockIter = m_children.begin(); clockIter != m_children.end(); ++ clockIter )
	{
		( *clockIter )->AdvanceTime( internalDeltaSeconds );
	}
}