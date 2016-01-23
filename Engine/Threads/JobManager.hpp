//---------------------------------------------------------------------------
// JobManager.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_JOBMANAGER_
#define _INCLUDED_JOBMANAGER_

#include <thread>
#include <mutex>
#include <deque>

class Job;
typedef std::deque< Job* >			JobQueue;
typedef std::vector< Job* >			FinishedJobs;
typedef std::vector< std::thread >	Threads;

/////////////////////////////////////////////////////////////////////////////
class JobManager
{
public:
	JobManager();
	JobManager( int numThreads );
	~JobManager();

	void			Update();
	void			AddJobToQueue( Job* job );
	unsigned int	GetNumberOfQueuedJobs();
	unsigned int	GetNumberOfFinishedJobs();
	bool			HasJobs();
	void			TestThreads();

private:
	bool			m_isTerminating;
	int				m_maxNumThreads;
	unsigned int	m_numFinishedJobs;
	Threads			m_threads;
	JobQueue		m_queuedJobs;
	FinishedJobs	m_finishedJobs;
	std::mutex		m_mutexLock;

private:
	void			Initialize( int numThreads );
	void			ThreadLoop( int threadNumber );
	void			ClaimPendingJob( int threadNumber );
	void			ThreadTest( int id );
	static bool		SortJobs( Job* firstJob, Job* secondJob );

};

#endif