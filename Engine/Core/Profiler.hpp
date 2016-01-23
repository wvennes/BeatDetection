//---------------------------------------------------------------------------
// ProfileSection.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PROFILER_
#define _INCLUDED_PROFILER_

#include <vector>

#include "Engine/Core/ProfileSection.hpp"

class VennRenderer;
class TextRenderer;


/////////////////////////////////////////////////////////////////////////////
class Profiler
{
public:
	Profiler();
	~Profiler();

	//static void				DestroyProfiler();
	//static Profiler*		GetProfilerRoot() { return s_root; }
	//static void				StartProfiler();
	//static void				StopProfiler();

	void					Render( float cellHeight = 32.f );

	// TODO: Support adding new kinds of profile sections
	void					StartProfileSection( ProfileCategory profileCategory );
	void					StopProfileSection( ProfileCategory profileCategory );
	//void					AddProfileSection( const char* name );
	//void					StartProfileSection( const char* name );
	//void					StopProfileSection( const char* name );

	Seconds					GetOverallTimeForSection( ProfileCategory profileCategory );
	Seconds					GetAverageTimeForSection( ProfileCategory profileCategory );
	double					GetTotalPercentageOfOverallTime( ProfileCategory profileCategory );
	double					GetAveragePercentageOfOverallTime( ProfileCategory profileCategory );

private:
	ProfileSection*			m_overallSection;
	ProfileSection*			m_inputSection;
	ProfileSection*			m_updateSection;
	ProfileSection*			m_renderSection;

private:
	void					Initialize();
	void					RenderSectionInformation( Vec2f& screenPosition, float cellHeight, ProfileCategory profileSection );

};

extern VennRenderer*	g_theRenderer;
extern TextRenderer*	g_theTextRenderer;

#endif