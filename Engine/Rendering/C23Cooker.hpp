//---------------------------------------------------------------------------
// C23Cooker.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_C23COOKER_
#define _INCLUDED_C23COOKER_

#include <string>

/////////////////////////////////////////////////////////////////////////////
class C23Cooker
{
public:
	C23Cooker();
	~C23Cooker();

	static void		CookObjFilesInFolder( const std::string& rawDataLocation, const std::string& cookedDataDestination, bool isCookAllEnabled );

private:

};


#endif