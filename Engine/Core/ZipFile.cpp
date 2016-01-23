//---------------------------------------------------------------------------
// ZipFile.cpp
//---------------------------------------------------------------------------

#include <string>
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ZipFile.hpp"

#define STATIC

STATIC ZipFileMap	ZipFile::s_zipFileMap;


//---------------------------------------------------------------------------
ZipFile::ZipFile( const char* zipFilePath )
{
	memset( &m_zipArchive, 0, sizeof( mz_zip_archive ) );
	mz_bool success = mz_zip_reader_init_file( &m_zipArchive, zipFilePath, 0 );
	if ( !success )
	{
		DebuggerPrintf( "Zip file creation failed!\n" );
	}
}


//---------------------------------------------------------------------------
ZipFile::~ZipFile()
{
	mz_zip_reader_end( &m_zipArchive );
}


//---------------------------------------------------------------------------
STATIC void ZipFile::FreeZipFileRegistryMemory()
{
	DestroyMap( s_zipFileMap );
}


//---------------------------------------------------------------------------
STATIC ZipFile* ZipFile::CreateOrGetZipFile( const char* zipFilePath )
{
	ZipFile* fileToReturn = nullptr;

	if ( s_zipFileMap.find( zipFilePath ) != s_zipFileMap.end() )
	{
		fileToReturn = s_zipFileMap[ zipFilePath ];
	}
	else
	{
		fileToReturn = new ZipFile( zipFilePath );
		s_zipFileMap.insert( std::make_pair( zipFilePath, fileToReturn ) );
	}

	return fileToReturn;
}


//---------------------------------------------------------------------------
char* ZipFile::LoadFileToHeap( const char* fileLocationInsideZip, size_t& out_fileSize )
{
	char* result = nullptr;

	result = reinterpret_cast< char* > ( mz_zip_reader_extract_file_to_heap( &m_zipArchive, fileLocationInsideZip, &out_fileSize, 0 ) );

	return result;
}


//---------------------------------------------------------------------------
STATIC char* ZipFile::LoadFileFromZipArchiveOnDiskToHeap( const char* zipFileLocation, const char* fileLocationInsideZip, size_t& out_fileSize )
{
	char* buffer = nullptr;
	buffer = reinterpret_cast< char* > ( mz_zip_extract_archive_file_to_heap( zipFileLocation, fileLocationInsideZip, &out_fileSize, 0 ) );
	return buffer;
}