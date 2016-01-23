//---------------------------------------------------------------------------
// JobManager.cpp
//---------------------------------------------------------------------------

//#include <thread>
#include <algorithm>
#include <chrono>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Threads/Job.hpp"
#include "Engine/Threads/JobManager.hpp"

#define STATIC


//---------------------------------------------------------------------------
JobManager::JobManager()
	: m_isTerminating( false )
	, m_maxNumThreads( -1 )
	, m_numFinishedJobs( 0 )
{
	Initialize( -1 );
}


//---------------------------------------------------------------------------
JobManager::JobManager( int numThreads )
	: m_isTerminating( false )
	, m_maxNumThreads( numThreads )
	, m_numFinishedJobs( 0 )
{
	Initialize( numThreads );
}


//---------------------------------------------------------------------------
JobManager::~JobManager()
{
	m_isTerminating = true;
	for ( int index = 0; index < m_maxNumThreads; ++ index )
	{
		m_threads[ index ].join();
	}

	DestroyDeque( m_queuedJobs );
	DestroyVector( m_finishedJobs );
}


//---------------------------------------------------------------------------
void JobManager::Initialize( int numThreads )
{
	if ( numThreads < 0 )
	{
		m_maxNumThreads = std::thread::hardware_concurrency();
		m_threads.reserve( m_maxNumThreads );
	}
	else if ( numThreads == 0 )
	{
		DebuggerPrintf( "0 threads, using main thread\n" );
		return;
	}
	else
	{
		m_threads.reserve( m_maxNumThreads );
	}

	for ( int count = 0; count < m_maxNumThreads; ++ count )
	{
		m_threads.push_back( std::thread( &JobManager::ThreadLoop, this, count ) );
	}

	DebuggerPrintf( "%i threads created\n", m_maxNumThreads );
}


//---------------------------------------------------------------------------
bool JobManager::HasJobs()
{
	return !m_queuedJobs.empty();
}


//---------------------------------------------------------------------------
unsigned int JobManager::GetNumberOfQueuedJobs()
{
	return ( unsigned int ) m_queuedJobs.size();
}


//---------------------------------------------------------------------------
unsigned int JobManager::GetNumberOfFinishedJobs()
{
	return m_numFinishedJobs;
}


//---------------------------------------------------------------------------
void JobManager::ThreadLoop( int threadNumber )
{
	while ( !m_isTerminating )
	{
		ClaimPendingJob( threadNumber );
	}
}


//---------------------------------------------------------------------------
void JobManager::AddJobToQueue( Job* job )
{
	m_mutexLock.lock();
	switch ( job->GetPriority() )
	{
		case HIGH:
		{
			m_queuedJobs.push_back( job );
			break;
		}
		case AVERAGE:
		{
			m_queuedJobs.push_front( job );
			std::sort( m_queuedJobs.begin(), m_queuedJobs.end(), &JobManager::SortJobs );
			break;
		}
		case LOW:
		{
			m_queuedJobs.push_front( job );
			break;
		}
	}
	m_mutexLock.unlock();
}


//---------------------------------------------------------------------------
void JobManager::Update()
{
	if ( m_threads.empty() ) ClaimPendingJob( 0 ); // if no threads, main thread handles jobs
	m_mutexLock.lock();
	{
		if ( !m_finishedJobs.empty() )
		{
			Job* job = m_finishedJobs.back();
			job->Callback();
			delete job;
			m_finishedJobs.pop_back();
		}
	}
	m_mutexLock.unlock();
}


//---------------------------------------------------------------------------
void JobManager::ClaimPendingJob( int threadNum )
{
	m_mutexLock.lock();
	if ( m_queuedJobs.empty() )
	{
		m_mutexLock.unlock();
		if ( m_maxNumThreads > 0 ) std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) ); // sleep this thread for a short time
		return;
	}

	Job* currentJob = nullptr;
	{
		currentJob = m_queuedJobs.back();
		m_queuedJobs.pop_back();
	}
	m_mutexLock.unlock(); // unlock this mutex - no longer acting on this queue

	currentJob->Execute();

	m_mutexLock.lock();
	{
		++ m_numFinishedJobs;
		DebuggerPrintf( "Thread %i finished a priority %i job! %u\n", threadNum, currentJob->GetPriority(), m_numFinishedJobs );
		m_finishedJobs.push_back( currentJob );
	}
	m_mutexLock.unlock();
}


// test function for threads
//---------------------------------------------------------------------------
void JobManager::TestThreads()
{
	for ( int index = 0; index < m_maxNumThreads; ++ index )
	{
		m_threads[ index ] = std::thread( &JobManager::ThreadTest, this, index );
	}

	for ( unsigned int index = 0; index < ( unsigned int ) m_threads.size(); ++ index )
	{
		m_threads[ index ].join();
	}
}


// test function for threads
//---------------------------------------------------------------------------
void JobManager::ThreadTest( int id )
{
	DebuggerPrintf( "thread %i is waiting\n", id );
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
}


//---------------------------------------------------------------------------
STATIC bool JobManager::SortJobs( Job* firstJob, Job* secondJob )
{
	return firstJob->GetPriority() < secondJob->GetPriority();
}