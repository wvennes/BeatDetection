#include "Engine/Core/EngineCommon.hpp"
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>


const int STRINGF_STACK_LOCAL_TEMP_LENGTH = 1024;


//-----------------------------------------------------------------------------------------------
// Construct a temporary std::string using printf-style formatting.  For example:
//	std::string studentInfoString = Stringf( "%s is a %i-year-old %s.", studentName, studentAge, studentGender );
//
const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
// Same as the above version of Stringf(), but with an explicitly chosen maximum buffer size.
//
const std::string Stringf( const int maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
// Print a string to the Visual Studio "Output" debug window, if we are running in the debugger.
// Uses printf-style formatting, for example:
//	DebuggerPrintf( "%s is a %i-year-old %s.", studentName, studentAge, studentGender );
// 
void DebuggerPrintf( const char* messageFormat, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, messageFormat );
	vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
	va_end( variableArgumentList );
	messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	OutputDebugStringA( messageLiteral ); // Print the string to the "Output" debug window in Visual Studio
	std::cout << messageLiteral; // Also write it out to standard out, if anyone is listening
}


//-----------------------------------------------------------------------------------------------
void TestMemoryIntegrity()
{
	char c = rand() % 10;
	char dummyStackMemory[ 512 * 1024 ];
	dummyStackMemory[0] = c;

	char* dummyHeapMemory = new char[ 100 * 1024 * 1024 ];
	dummyHeapMemory[0] = c;

	DebuggerPrintf( "Checking memory (dummies=%i,%i)\n", dummyStackMemory[0], dummyHeapMemory[0] );
	delete dummyHeapMemory;
}


//---------------------------------------------------------------------------
void CreateConsole()
{
	 //allocate a console for output - source : https://justcheckingonall.wordpress.com/2008/08/29/console-window-win32-app/
	 AllocConsole();
	 
	 HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
	 int hCrt = _open_osfhandle( ( long ) handle_out, _O_TEXT );
	 FILE* hf_out = _fdopen( hCrt, "w" );
	 setvbuf( hf_out, NULL, _IONBF, 1 );
	 *stdout = *hf_out;
	 
	 HANDLE handle_in = GetStdHandle( STD_INPUT_HANDLE );
	 hCrt = _open_osfhandle( ( long ) handle_in, _O_TEXT );
	 FILE* hf_in = _fdopen( hCrt, "r" );
	 setvbuf( hf_in, NULL, _IONBF, 128 );
	 *stdin = *hf_in;
}