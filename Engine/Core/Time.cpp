//-----------------------------------------------------------------------------------------------
// Time.cpp - Code by Squirrel Eiserloh
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
#include "Time.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//---------------------------------------------------------------------------
double InitializeTime( LARGE_INTEGER& out_initialTime )
{
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency( &countsPerSecond );
	QueryPerformanceCounter( &out_initialTime );
	return( 1.0 / static_cast< double >( countsPerSecond.QuadPart ) );
}

//---------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	static LARGE_INTEGER initialTime;
	static double secondsPerCount = InitializeTime( initialTime );
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter( &currentCount );
	LONGLONG elapsedCountsSinceInitialTime = currentCount.QuadPart - initialTime.QuadPart;

	double currentSeconds = static_cast< double >( elapsedCountsSinceInitialTime ) * secondsPerCount;
	return currentSeconds;
}

//---------------------------------------------------------------------------
void LimitFPS()
{
	float deltaTime = 1.f / 60.f;
	double timeNow = GetCurrentTimeSeconds();
	static double previousTime = timeNow;
	double timeSinceLastFrame = timeNow - previousTime;

	while( timeSinceLastFrame < ( deltaTime ) )
	{
		timeNow = GetCurrentTimeSeconds();
		timeSinceLastFrame = timeNow - previousTime;
	}
	previousTime = timeNow;
}
