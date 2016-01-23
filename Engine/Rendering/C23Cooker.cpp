//---------------------------------------------------------------------------
// C23Cooker.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Rendering/OBJFileHandler.hpp"
#include "Engine/Rendering/C23FileHandler.hpp"
#include "Engine/Rendering/C23Cooker.hpp"
#define STATIC


//---------------------------------------------------------------------------
C23Cooker::C23Cooker()
{

}


//---------------------------------------------------------------------------
C23Cooker::~C23Cooker()
{

}


//---------------------------------------------------------------------------
STATIC void C23Cooker::CookObjFilesInFolder( const std::string& rawDataDirectory, const std::string& cookedDataDestinationDirectory, bool isCookAllEnabled )
{
	std::vector< Vertex3D > vertexes;
	std::vector< unsigned int > indexes;
	std::vector< std::string > filePaths;
	std::string objFileName;
	std::string c23FileName;
	std::string destination( cookedDataDestinationDirectory );
	OBJFileHandler objFileHandler;
	C23FileHandler c23FileHandler;
	
	// find all the .obj files in this directory
// 	if ( rawDataDirectory.back() == '/' )
// 	{
// 		FileSystem::EnumerateFilesInDirectory( rawDataDirectory + "*.obj", filePaths );
// 	}
// 	else
// 	{
// 		FileSystem::EnumerateFilesInDirectory( rawDataDirectory + "/*.obj", filePaths );
// 	}
	FileSystem::EnumerateFilesInDirectory( rawDataDirectory, filePaths );


	// if we found no files, return
	if ( filePaths.empty() )
	{
		std::cerr << "No files found!" << std::endl;
		return;
	}

	// add a trailing '/' to destination if don't already have
	if ( destination.back() != '/' )
	{
		destination += '/';
	}

	std::wstring dataFolder( destination.begin(), destination.end() );
	CreateDirectoryW( dataFolder.c_str(), nullptr );

	for ( unsigned int index = 0; index < ( unsigned int ) filePaths.size(); ++ index )
	{
		std::string& currentFilePath = filePaths[ index ];
		destination = cookedDataDestinationDirectory + currentFilePath.substr( rawDataDirectory.find_first_of( '*' ) );

		if ( currentFilePath.substr( 0, currentFilePath.find_last_of( '/' ) + 1 ) == currentFilePath )
		{
			std::cout << "Creating directory " + destination + "... ";
			std::wstring newDir( destination.begin(), destination.end() );
			CreateDirectoryW( newDir.c_str(), nullptr );
			std::cout << "Done!" << std::endl;
		}
		else if ( currentFilePath.substr( currentFilePath.find_last_of( '.' ) ) == ".obj" )
		{
			if ( FileSystem::IsFileDateOlder( currentFilePath, destination.substr( 0, destination.find_last_of( '.' ) ) + ".c23" ) && !isCookAllEnabled ) continue;

			destination = destination.substr( 0, destination.find_last_of( '/' ) + 1 );
			objFileName = currentFilePath.substr( currentFilePath.find_last_of( '/' ) + 1 );

			c23FileName = objFileName.substr( 0, objFileName.find_last_of( '.' ) ) + ".c23";

			std::cout << "Cooking " << objFileName << "... ";

			objFileHandler.ReadFromObjAndStoreVertexesAndIndexes( currentFilePath.c_str(), vertexes, indexes );

			if ( vertexes.empty() )
			{
				std::cerr << "Error - file failed to load!" << std:: endl;
				continue;
			}

			c23FileHandler.WriteToFile( ( destination + c23FileName ).c_str(), vertexes, indexes );

			std::cout << "Success!" << std::endl;

			vertexes.clear();
			indexes.clear();
		}
		else
		{
			if ( FileSystem::IsFileDateOlder( currentFilePath, destination ) && !isCookAllEnabled ) continue;

			std::cout << "Copying " << currentFilePath << " to " << destination << "... ";
			std::wstring oldFile( currentFilePath.begin(), currentFilePath.end() );
			std::wstring newFile( destination.begin(), destination.end() );
			CopyFileW( oldFile.c_str(), newFile.c_str(), false );
			std::cout << "Done!" << std::endl;
		}
	}

// 	std::cout << "Cooking " << filePaths.size() << " items..." << std::endl;
	// cook each file to the destination directory
}