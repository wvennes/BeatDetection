//---------------------------------------------------------------------------
// StringTable.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_STRINGTABLE_
#define _INCLUDED_STRINGTABLE_

#include <string>
#include <map>

unsigned int	StringID( const std::string& stringValue );
const char*		StringValue( const unsigned int stringID );

#endif