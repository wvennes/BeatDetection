//---------------------------------------------------------------------------
// LoadFileJob.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_LOADFILEJOB_
#define _INCLUDED_LOADFILEJOB_

#include <string>

#include "Engine/Threads/Job.hpp"

class ZipFile;

/////////////////////////////////////////////////////////////////////////////
enum LoadFileJobType
{
	LOAD_NORMAL,
	LOAD_ZIP
};


/////////////////////////////////////////////////////////////////////////////
class LoadFileJob : public Job
{
public:
	LoadFileJob( const char* zipFileName, const char* fileName, char*& byteBuffer, size_t& fileSize, LoadFileJobType fileType, JobPriority priority = AVERAGE );
	~LoadFileJob();

	bool				Execute();
	void				Callback();

private:
	char*&				m_byteBuffer;
	size_t&				m_fileSize;
	std::string			m_zipFileName;
	std::string			m_fileName;
	LoadFileJobType		m_fileType;

private:
	LoadFileJob& operator=( const LoadFileJob& ) = delete;

};

#endif