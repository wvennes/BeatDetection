//---------------------------------------------------------------------------
// TimeSystem.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Engine/Core/TimeSystem.hpp"

double InitializeTimeSystem( LARGE_INTEGER& startTime );

static LARGE_INTEGER g_startTime; // static start time
static double g_secondsPerCount = InitializeTimeSystem( g_startTime ); // initialize the start time and store seconds per count


//---------------------------------------------------------------------------
double InitializeTimeSystem( LARGE_INTEGER& startTime )
{
	LARGE_INTEGER counterFrequency;
	QueryPerformanceFrequency( &counterFrequency );
	QueryPerformanceCounter( &startTime );

	return 1.0 / ( double ) ( counterFrequency.QuadPart );
}


//---------------------------------------------------------------------------
double GetAbsoluteTimeSeconds()
{
	LARGE_INTEGER currentPerformanceCount;
	QueryPerformanceCounter( &currentPerformanceCount ); // get the current counter time

	LONGLONG totalElapsedCount = currentPerformanceCount.QuadPart - g_startTime.QuadPart; // get the difference between the two

	return ( double ) ( totalElapsedCount ) * g_secondsPerCount; // convert back to seconds
}


//---------------------------------------------------------------------------
void LimitFPS()
{
	double deltaTime = 1.0 / 60.0;
	double timeNow = GetAbsoluteTimeSeconds();
	static double previousTime = timeNow;
	double timeSinceLastFrame = timeNow - previousTime;

	while( timeSinceLastFrame < ( deltaTime ) )
	{
		timeNow = GetAbsoluteTimeSeconds();
		timeSinceLastFrame = timeNow - previousTime;
	}
	previousTime = timeNow;
}