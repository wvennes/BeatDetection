//---------------------------------------------------------------------------
// HashBufferJob.cpp
//---------------------------------------------------------------------------

#include <mutex>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Threads/HashBufferJob.hpp"


//---------------------------------------------------------------------------
HashBufferJob::HashBufferJob( const char* buffer, const size_t bufferSize, unsigned int& out_hashResult, JobPriority priority /* = AVERAGE */ )
	: Job( priority )
	, m_buffer( buffer )
	, m_bufferSize( bufferSize )
	, m_out_hashResult( out_hashResult )
{

}


//---------------------------------------------------------------------------
HashBufferJob::~HashBufferJob()
{

}


//---------------------------------------------------------------------------
bool HashBufferJob::Execute()
{
	unsigned int result = 0;
	for ( size_t index = 0; index < m_bufferSize; ++ index )
	{
		result = ( result * 31 ) ^ ( ( unsigned int ) m_buffer[ index ] );
	}

	m_mutexLock.lock();
	m_out_hashResult = result;
	m_mutexLock.unlock();

	return true;
}


//---------------------------------------------------------------------------
void HashBufferJob::Callback()
{
	//DebuggerPrintf( "Hash job finished!\n" );
}