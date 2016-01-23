//---------------------------------------------------------------------------
// DeveloperConsole.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_DEVELOPERCONSOLE_
#define _INCLUDED_DEVELOPERCONSOLE_

#include <string>
#include <vector>
#include <map>
#include "Engine/Math/Rgba.hpp"
#include "Engine/Math/Vec2.hpp"

class VennRenderer;
class TextRenderer;
struct ConsoleCommandArguments;
struct CommandLogLine;
struct RegisteredCommand;

typedef void( CommandFunction ) ( const ConsoleCommandArguments& arguments );
typedef std::map< std::string, RegisteredCommand* >::iterator RegisteredCommandIter;

enum TextInputType
{
	INPUT_TEXT,
	INPUT_SPECIAL,
	NUM_INPUTS
};

/////////////////////////////////////////////////////////////////////////////
class DeveloperConsole
{
public:
	static std::vector< CommandLogLine >	m_logLines;

public:
	DeveloperConsole();
	DeveloperConsole( const char* fontMetaDataFilePath, const float fontCellHeight );
	~DeveloperConsole();

	bool									InputToCurrentLogCommand( const char input, const TextInputType inputType );

	void									RegisterCommand( const std::string& name, CommandFunction* function, const std::string& description );
	void									ExecuteCommandString( const std::string& commandString );
	static void								AddCommandLogLine( const std::string& text, const Rgba& color );
	void									Render( const unsigned int shaderProgramID );

private:
	std::vector< std::string >				m_commandHistory;
	TextRenderer*							m_textRenderer;
	float									m_fontCellHeight;
	std::string								m_currentConsoleCommandString;
	unsigned int							m_currentCursorIndex;
	unsigned int							m_currentCommandHistoryIndex;
	float									m_cursorBlinkTimerSeconds;
	float									m_currentCursorTimer;
	bool									m_isCursorVisible;

private:
	void									Initialize();
	void									RenderConsole( const unsigned int shaderProgramID );
	void									RenderCursorAndUpdateTimer( const unsigned int shaderProgramID );
	void									ResetCursorTimer();
};

/////////////////////////////////////////////////////////////////////////////
struct ConsoleCommandArguments
{
	std::string					m_rawArgumentString;
	std::vector< std::string >	m_argumentList;
};


/////////////////////////////////////////////////////////////////////////////
struct CommandLogLine
{
	std::string		m_text;
	Rgba			m_color;
};


/////////////////////////////////////////////////////////////////////////////
struct RegisteredCommand
{
	CommandFunction*	m_function;
	std::string			m_commandDescription;
	std::string			m_commandName;
};

extern VennRenderer* g_theRenderer;
extern bool g_isQuitting;

#endif