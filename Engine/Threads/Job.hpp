//---------------------------------------------------------------------------
// Job.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_JOB_
#define _INCLUDED_JOB_


/////////////////////////////////////////////////////////////////////////////
enum JobPriority
{
	HIGH		= 3,
	AVERAGE		= 2,
	LOW			= 1,
};


/////////////////////////////////////////////////////////////////////////////
class Job
{
public:
	Job( JobPriority priority = AVERAGE );
	~Job();

	virtual bool	Execute() { return true; } // child jobs override this function
	virtual void	Callback() {}

	bool			IsJobFinished() const { return m_isJobFinished; }
	JobPriority		GetPriority() const { return m_jobPriority; }

private:
	bool			m_isJobFinished;
	JobPriority		m_jobPriority;

};

#endif