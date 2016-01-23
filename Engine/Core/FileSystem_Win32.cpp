//---------------------------------------------------------------------------
// FileSystem.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
//#include <csystem>
#include "Engine/Core/FileSystem.hpp"
#define STATIC


//---------------------------------------------------------------------------
FileSystem::FileSystem()
{

}


//---------------------------------------------------------------------------
FileSystem::~FileSystem()
{

}


//---------------------------------------------------------------------------
// Code based on snippet from Ken Harward
STATIC bool FileSystem::EnumerateFilesInDirectory( const std::string& directory, std::vector< std::string >& outFilesFound )
{
	bool isADirectory;
	bool isHidden;
	int error = 0;
	struct _finddata_t fileInfo;
	intptr_t searchHandle = _findfirst( directory.c_str(), &fileInfo );

	while ( searchHandle != -1 && !error )
	{
		isADirectory = fileInfo.attrib & _A_SUBDIR ? true : false; // throw away directory
		isHidden = fileInfo.attrib & _A_HIDDEN ? true : false; // throw away hidden files
		if ( strcmp( fileInfo.name, "." ) != 0 && strcmp( fileInfo.name, ".." ) != 0 )
		{
			if ( isADirectory )
			{
				outFilesFound.push_back( directory.substr( 0, directory.find_last_of( '/' ) + 1 ) + fileInfo.name + "/"  );
				EnumerateFilesInDirectory( directory.substr( 0, directory.find_last_of( '/' ) + 1 ) + fileInfo.name + "/*", outFilesFound );
			}
			else
			{
				outFilesFound.push_back( directory.substr( 0, directory.find_last_of( '/' ) + 1 ) + fileInfo.name );
			}
		}
		error = _findnext( searchHandle, &fileInfo );
	}
	return true;
}


//---------------------------------------------------------------------------
STATIC bool FileSystem::IsFileDateOlder( const std::string& firstFile, const std::string& secondFile )
{
	HANDLE hFirstFile = CreateFileA(
		firstFile.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL );

	HANDLE hSecondFile = CreateFileA(
		secondFile.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL );

	if ( hSecondFile == INVALID_HANDLE_VALUE )
	{
		// file doesn't exist
		CloseHandle( hFirstFile );
		CloseHandle( hSecondFile );
		return false;
	}

	FILETIME firstCreationTime;
	FILETIME firstLastAccessTime;
	FILETIME firstLastWriteTime;
	int errorFirst = GetFileTime( hFirstFile, &firstCreationTime, &firstLastAccessTime, &firstLastWriteTime );
	
	FILETIME secondCreationTime;
	FILETIME secondLastAccessTime;
	FILETIME secondLastWriteTime;
	int errorSecond = GetFileTime( hSecondFile, &secondCreationTime, &secondLastAccessTime, &secondLastWriteTime );

	CloseHandle( hFirstFile );
	CloseHandle( hSecondFile );

	if ( !errorFirst || !errorSecond ) return false;

	SYSTEMTIME firstFileTime;
	SYSTEMTIME secondFileTime;
	errorFirst = FileTimeToSystemTime( &firstLastWriteTime, &firstFileTime );
	errorSecond = FileTimeToSystemTime( &secondLastWriteTime, &secondFileTime );

	if ( !errorFirst || !errorSecond ) return false;

	if ( firstFileTime.wYear < secondFileTime.wYear ) return true;
	if ( firstFileTime.wYear > secondFileTime.wYear ) return false;

	if ( firstFileTime.wMonth < secondFileTime.wMonth ) return true;
	if ( firstFileTime.wMonth > secondFileTime.wMonth ) return false;

	if ( firstFileTime.wDay < secondFileTime.wDay ) return true;
	if ( firstFileTime.wDay > secondFileTime.wDay ) return false;

	if ( firstFileTime.wHour < secondFileTime.wHour ) return true;
	if ( firstFileTime.wHour > secondFileTime.wHour ) return false;

	if ( firstFileTime.wMinute < secondFileTime.wMinute ) return true;
	if ( firstFileTime.wMinute > secondFileTime.wMinute ) return false;

	if ( firstFileTime.wSecond < secondFileTime.wSecond ) return true;
	if ( firstFileTime.wSecond > secondFileTime.wSecond ) return false;

	return false;
}


//---------------------------------------------------------------------------
// TODO: finish this, doesn't work
STATIC bool FileSystem::CopyAllFilesToNewDirectoryRelativeToExecutable( const std::string& sourceDirectory, const std::string& destinationDirectory )
{
	//bool result = true;
	std::vector< std::string > filePaths;
	std::string source( sourceDirectory );
	std::string dest( destinationDirectory );
	std::string command;
	std::string exeFilePath = GetExecutablePath() + "/";
	//if ( source.back() != '/' ) source += '/';
	//if ( dest.back() != '/' ) dest += '/';

	//SHFILEOPSTRUCT fileOperation = { 0 };
	//fileOperation.wFunc = FO_COPY;
	//fileOperation.
	//SHFileOperation()

//	result = EnumerateFilesInDirectory( sourceDirectory + "*", filePaths );

// 	for ( unsigned int index = 0; index < ( unsigned int ) filePaths.size(); ++ index )
// 	{
// 		//xcopy /Y /F /I "$(SolutionDir)RawData\Textures\*" "$(SolutionDir)Run_$(PlatformName)\Data\Textures"
// 		command = "xcopy \"" + filePaths[ index ] + "\" \"" + destinationDirectory + "\"";
// 		system( command.c_str() );
// 	}
	command = "xcopy /Y /F /I \"" + exeFilePath + sourceDirectory + "/*" + "\" \"" + exeFilePath + destinationDirectory + "\"";
	system( command.c_str() );
	
	return true;
}


//---------------------------------------------------------------------------
STATIC std::string FileSystem::GetExecutablePath()
{
	TCHAR tCharBuffer[ MAX_PATH ];
	char charBuffer[ MAX_PATH ];
	GetModuleFileName( NULL, tCharBuffer, MAX_PATH );
	
	for ( unsigned int index = 0; tCharBuffer[ index ] != 0; ++ index )
	{
		charBuffer[ index ] = ( char ) tCharBuffer[ index ];
	}
	std::string::size_type position = std::string( charBuffer ).find_last_of( "\\/" );
	return std::string( charBuffer ).substr( 0, position );
}