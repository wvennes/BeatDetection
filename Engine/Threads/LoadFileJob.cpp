//---------------------------------------------------------------------------
// LoadFileJob.cpp
//---------------------------------------------------------------------------

#include <mutex>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ZipFile.hpp"
#include "Engine/Threads/LoadFileJob.hpp"


//---------------------------------------------------------------------------
LoadFileJob::LoadFileJob( const char* zipFileName, const char* fileName, char*& byteBuffer, size_t& fileSize, LoadFileJobType fileType, JobPriority priority /* = AVERAGE */ )
	: Job( priority )
	, m_byteBuffer( byteBuffer )
	, m_fileSize( fileSize )
	, m_zipFileName( zipFileName )
	, m_fileName( fileName )
	, m_fileType( fileType )
{

}


//---------------------------------------------------------------------------
LoadFileJob::~LoadFileJob()
{

}


//---------------------------------------------------------------------------
bool LoadFileJob::Execute()
{
	std::mutex fileLoadLock;

	fileLoadLock.lock();

	switch ( m_fileType )
	{
		default:
		case LOAD_NORMAL:
		{
			FILE* file = nullptr;
			fopen_s( &file, m_fileName.c_str(), "rb" );

			if ( file == nullptr ) return false;

			fseek( file, 0, SEEK_END );
			size_t fileSize = ftell( file );
			rewind( file );


			m_byteBuffer = new char[ fileSize ];
			fread( m_byteBuffer, sizeof( char ), fileSize, file );
			fclose( file );
			m_fileSize = fileSize;
			break;
		}
		case LOAD_ZIP:
		{
			m_byteBuffer = ZipFile::CreateOrGetZipFile( m_zipFileName.c_str() )->LoadFileToHeap( m_fileName.c_str(), m_fileSize );
			break;
		}
	}
	fileLoadLock.unlock();
	return true;
}


//---------------------------------------------------------------------------
void LoadFileJob::Callback()
{
	//DebuggerPrintf( "Load file job finished!\n" );
}