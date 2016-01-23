//---------------------------------------------------------------------------
// CommandLineParser.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <io.h>
#include <fcntl.h>
#include <atlstr.h>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/CommandLineParser.hpp"


//---------------------------------------------------------------------------
bool ParseArguments( CommandLineArguments& out_arguments )
{
	LPWSTR* argumentList = nullptr;
	int argumentCount = 0;
	argumentList = CommandLineToArgvW( GetCommandLine(), &argumentCount );

	if ( argumentCount < 2 ) return false;

	CommandLineArgument currentArgument;
	std::string tempArgString;

	// allocate a console for output - source : https://justcheckingonall.wordpress.com/2008/08/29/console-window-win32-app/
// 	AllocConsole();
// 
// 	HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
// 	int hCrt = _open_osfhandle( ( long ) handle_out, _O_TEXT );
// 	FILE* hf_out = _fdopen( hCrt, "w" );
// 	setvbuf( hf_out, NULL, _IONBF, 1 );
// 	*stdout = *hf_out;
// 
// 	HANDLE handle_in = GetStdHandle( STD_INPUT_HANDLE );
// 	hCrt = _open_osfhandle( ( long ) handle_in, _O_TEXT );
// 	FILE* hf_in = _fdopen( hCrt, "r" );
// 	setvbuf( hf_in, NULL, _IONBF, 128 );
// 	*stdin = *hf_in;
// 
// 	DebuggerPrintf( "Arguments found!\n" );

	bool hasFirstArgumentBeenFound = false;
	// parse arguments and store in vector
	for ( int argumentIndex = 1; argumentIndex < argumentCount; ++ argumentIndex ) // start at 1 to skip the .exe command
	{
		tempArgString = CW2A( argumentList[ argumentIndex ] );

		if ( tempArgString[ 0 ] == '-' )
		{
			if ( hasFirstArgumentBeenFound )
			{
				out_arguments.push_back( currentArgument );
				currentArgument.m_command.clear();
				currentArgument.m_arguments.clear();
			}
			currentArgument.m_command = tempArgString;
			hasFirstArgumentBeenFound = true;
		}
		else
		{
			currentArgument.m_arguments.push_back( tempArgString );
		}
	}

	out_arguments.push_back( currentArgument ); // push back the last argument worked on

// 	for ( unsigned int index = 0; index < ( unsigned int ) out_arguments.size(); ++ index )
// 	{
// 		DebuggerPrintf( "Command = %s\n", out_arguments[ index ].m_command.c_str() );
// 		for ( unsigned int argIndex = 0; argIndex < ( unsigned int ) out_arguments[ index ].m_arguments.size(); ++ argIndex )
// 		{
// 			DebuggerPrintf( "Argument = %s\n", out_arguments[ index ].m_arguments[ argIndex ].c_str() );
// 		}
// 		DebuggerPrintf( "\n" );
// 	}

	return true;
}