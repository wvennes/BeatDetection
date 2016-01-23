//---------------------------------------------------------------------------
// FileSystem.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FILESYSTEM_
#define _INCLUDED_FILESYSTEM_

#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	static bool			EnumerateFilesInDirectory( const std::string& directory, std::vector< std::string >& outFilesFound );
	static bool			IsFileDateOlder( const std::string& firstFile, const std::string& secondFile );
	static bool			CopyAllFilesToNewDirectoryRelativeToExecutable( const std::string& sourceDirectory, const std::string& destinationDirectory );
	static std::string	GetExecutablePath();

private:


};

#endif