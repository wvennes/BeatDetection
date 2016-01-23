//---------------------------------------------------------------------------
// ZipLoader.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ZIPLOADER_
#define _INCLUDED_ZIPLOADER_

#include <string>
#include <map>
#include "Engine/Core/miniz.h"


class ZipFile;

typedef std::map< std::string, ZipFile* >	ZipFileMap;

/////////////////////////////////////////////////////////////////////////////
class ZipFile
{
public:
	static ZipFile*		CreateOrGetZipFile( const char* zipFilePath );
	static void			FreeZipFileRegistryMemory();
	static char*		LoadFileFromZipArchiveOnDiskToHeap( const char* zipFileLocation, const char* fileLocationInsideZip, size_t& out_fileSize );

	char*				LoadFileToHeap( const char* fileLocationInsideZip, size_t& out_fileSize );

	~ZipFile();

private:
	static ZipFileMap	s_zipFileMap;

	mz_zip_archive		m_zipArchive;

private:
	ZipFile( const char* zipFilePath );

};

#endif