//---------------------------------------------------------------------------
// HashBufferJob.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_HASHBUFFERJOB_
#define _INCLUDED_HASHBUFFERJOB_

#include <mutex>
#include "Engine/Threads/Job.hpp"

/////////////////////////////////////////////////////////////////////////////
class HashBufferJob : public Job
{
public:
	HashBufferJob( const char* buffer, const size_t bufferSize, unsigned int& out_hashResult, JobPriority priority = AVERAGE );
	~HashBufferJob();

	bool			Execute();
	void			Callback();

private:
	const char*		m_buffer;
	const size_t	m_bufferSize;
	unsigned int&	m_out_hashResult;
	std::mutex		m_mutexLock;


private:
	HashBufferJob&	operator=( const HashBufferJob& ) = delete; // remove operator due to reference member variable

};

#endif