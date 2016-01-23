//---------------------------------------------------------------------------
// ProfileSection.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PROFILESECTION_
#define _INCLUDED_PROFILESECTION_

typedef double	Seconds;


/////////////////////////////////////////////////////////////////////////////
enum ProfileCategory
{
	CATEGORY_OVERALL,
	CATEGORY_INPUT,
	CATEGORY_UPDATE,
	CATEGORY_RENDER,
	NUM_PROFILE_CATEGORIES
};


/////////////////////////////////////////////////////////////////////////////
class ProfileSection
{
public:
	ProfileSection( ProfileCategory profileCategory = NUM_PROFILE_CATEGORIES );
	~ProfileSection();

	void				StartProfiling();
	void				StopProfiling();

	Seconds				GetAverageTimeForSection() const { return m_averageTimeForSection; }
	Seconds				GetTotalTimeForSection() const { return m_totalTimeForSection; }

private:
	ProfileCategory		m_profileSectionCategory;
	int					m_profileFrameCount;
	Seconds				m_averageTimeForSection;
	Seconds				m_totalTimeForSection;
	Seconds				m_lastStartTime;

};


#endif