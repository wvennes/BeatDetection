//---------------------------------------------------------------------------
// CommandLineParser.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_COMMANDLINEPARSER_
#define _INCLUDED_COMMANDLINEPARSER_

#include <string>
#include <vector>

struct CommandLineArgument;
typedef std::vector< std::string >			Arguments;
typedef std::vector< CommandLineArgument >	CommandLineArguments;

/////////////////////////////////////////////////////////////////////////////
struct CommandLineArgument
{
	std::string		m_command;
	Arguments		m_arguments;
};


//---------------------------------------------------------------------------
bool	ParseArguments( CommandLineArguments& out_arguments );

#endif
