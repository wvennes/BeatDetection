//---------------------------------------------------------------------------
// DeveloperConsole.cpp
//---------------------------------------------------------------------------

#include <vector>
#include <map>
#include "Engine/Input/InputWrapper.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Rendering/BitmapFont.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Core/DeveloperConsole.hpp"

#define STATIC

STATIC std::vector< CommandLogLine >			DeveloperConsole::m_logLines;
std::map< std::string, RegisteredCommand* >		g_registeredCommands;


//---------------------------------------------------------------------------
void Command_Help( const ConsoleCommandArguments& arguments )
{
	DeveloperConsole::AddCommandLogLine( arguments.m_rawArgumentString, Colors::WHITE );

	if ( arguments.m_argumentList.size() > 2 )
	{
		DeveloperConsole::AddCommandLogLine( "ERROR: Incorrect usage (type \"help\" or \"help <command>\").", Colors::RED );
	}
	else if ( arguments.m_argumentList.size() == 2 )
	{
		if ( g_registeredCommands.find( arguments.m_argumentList[ 1 ] ) != g_registeredCommands.end() )
		{
			DeveloperConsole::AddCommandLogLine( g_registeredCommands[ arguments.m_argumentList[ 1 ] ]->m_commandDescription, Colors::CYAN );
		}
		else
		{
			DeveloperConsole::AddCommandLogLine( "ERROR: Command \"" + arguments.m_argumentList[ 1 ] + "\" not found. Type <help> for a list of commands.", Colors::RED );
		}
	}
	else
	{
		for ( RegisteredCommandIter iter = g_registeredCommands.begin(); iter != g_registeredCommands.end(); ++iter )
		{
			DeveloperConsole::AddCommandLogLine( iter->second->m_commandName + " - " + iter->second->m_commandDescription, Colors::CYAN );
		}
	}
	//DeveloperConsole::AddCommandLogLine( "I'm helping!", Vec4f( 0.f, 1.f, 1.f, 1.f ) );
}


//---------------------------------------------------------------------------
void Command_Clear( const ConsoleCommandArguments& arguments )
{
	DeveloperConsole::AddCommandLogLine( arguments.m_rawArgumentString, Colors::WHITE );

	if ( arguments.m_argumentList.size() > 1 )
	{
		DeveloperConsole::AddCommandLogLine( "ERROR: Incorrect usage (type <help> for a list of commands).", Colors::RED );
	}
	else
	{
		DeveloperConsole::m_logLines.clear();
	}
}


//---------------------------------------------------------------------------
void Command_Quit( const ConsoleCommandArguments& arguments )
{
	DeveloperConsole::AddCommandLogLine( arguments.m_rawArgumentString, Colors::WHITE );

	if ( arguments.m_argumentList.size() > 1 )
	{
		DeveloperConsole::AddCommandLogLine( "ERROR: Incorrect usage (type <help> for a list of commands).", Colors::RED );
	}
	else if ( arguments.m_argumentList[ 0 ] == "qqq" || arguments.m_argumentList[ 0 ] == "quit" )
	{
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
DeveloperConsole::DeveloperConsole()
	: m_textRenderer( nullptr )
	, m_fontCellHeight( 0.f )
	, m_currentConsoleCommandString( "" )
	, m_currentCursorIndex( 0 )
	, m_currentCommandHistoryIndex( 0 )
	, m_cursorBlinkTimerSeconds( 0.f )
	, m_currentCursorTimer( 0.f )
	, m_isCursorVisible( false )
{
	Initialize();
}


//---------------------------------------------------------------------------
DeveloperConsole::DeveloperConsole( const char* fontMetaDataFilePath, const float fontCellHeight )
	: m_textRenderer( nullptr )
	, m_fontCellHeight( fontCellHeight )
	, m_currentConsoleCommandString( "" )
	, m_currentCursorIndex( 0 )
	, m_currentCommandHistoryIndex( 0 )
	, m_cursorBlinkTimerSeconds( 0.f )
	, m_currentCursorTimer( 0.f )
	, m_isCursorVisible( false )
{
	Initialize();
	m_textRenderer = new TextRenderer( fontMetaDataFilePath );
}


//---------------------------------------------------------------------------
void DeveloperConsole::Initialize()
{
	m_cursorBlinkTimerSeconds = 0.5f;
	m_currentCursorTimer = 0.f;
	m_currentCursorIndex = 0;
	m_currentCommandHistoryIndex = 0;
	m_isCursorVisible = true;
	//m_currentConsoleCommandString = "\0"; // null terminate

	RegisterCommand( "help", Command_Help, "Displays command information. Usage: <help> <command>" );
	RegisterCommand( "clear", Command_Clear, "Clears the command log. Usage: <clear>" );
	RegisterCommand( "quit", Command_Quit, "Quits the program. Usage: <quit>" );
	RegisterCommand( "qqq", Command_Quit, "Quits the program. Usage: <qqq>" );
}


//---------------------------------------------------------------------------
DeveloperConsole::~DeveloperConsole()
{
	//std::map< std::string, RegisteredCommand* >::iterator RegisteredCommandIter;
	delete m_textRenderer;
	for ( RegisteredCommandIter iter = g_registeredCommands.begin(); iter != g_registeredCommands.end(); ++iter )
	{
		delete iter->second;
	}
	g_registeredCommands.clear();
}


//---------------------------------------------------------------------------
bool DeveloperConsole::InputToCurrentLogCommand( const char input, const TextInputType inputType )
{
	bool result = true;
	switch( inputType )
	{
		case INPUT_TEXT:
		{
			if ( input == INPUT_RETURN )
			{
				if ( !m_currentConsoleCommandString.empty() )
				{
					ExecuteCommandString( m_currentConsoleCommandString );
					m_commandHistory.push_back( m_currentConsoleCommandString );
					++m_currentCommandHistoryIndex;
					m_currentConsoleCommandString.clear();
					m_currentCursorIndex = 0;
				}
				else
				{
					result = false;
				}
			}
			else if ( input == INPUT_ESCAPE )
			{
				if ( !m_currentConsoleCommandString.empty() )
				{
					m_currentConsoleCommandString.clear();
					m_currentCursorIndex = 0;
				}
				else
				{
					result = false;
				}
			}
			else if ( input == INPUT_BACKSPACE )
			{
				if ( !m_currentConsoleCommandString.empty() )
				{
					m_currentConsoleCommandString.erase( m_currentConsoleCommandString.begin() + m_currentCursorIndex - 1 );
					--m_currentCursorIndex;
				}
			}
			else
			{
				if ( m_textRenderer->m_bitmapFont->m_glyphData.find( input ) != m_textRenderer->m_bitmapFont->m_glyphData.end() )
				{
					m_currentConsoleCommandString.insert( m_currentConsoleCommandString.begin() + m_currentCursorIndex, input );
					++m_currentCursorIndex;
				}
			}
			break;
		}
		case INPUT_SPECIAL:
		{
			switch ( input )
			{
				case INPUT_LEFT:
				{
					if ( m_currentCursorIndex > 0 ) --m_currentCursorIndex;
					break;
				}
				case INPUT_RIGHT:
				{
					if ( m_currentCursorIndex < m_currentConsoleCommandString.length() ) ++m_currentCursorIndex;
					break;
				}
				case INPUT_UP:
				{
					if ( !m_commandHistory.empty() )
					{
						if ( m_currentCommandHistoryIndex > 0 ) --m_currentCommandHistoryIndex;
						m_currentConsoleCommandString = m_commandHistory[ m_currentCommandHistoryIndex ];
					}
					break;
				}
				case INPUT_DOWN:
				{
					if ( !m_commandHistory.empty() )
					{
						if ( m_currentCommandHistoryIndex < m_commandHistory.size() ) ++m_currentCommandHistoryIndex;
						m_currentConsoleCommandString = m_commandHistory[ m_currentCommandHistoryIndex - 1 ];
					}
					break;
				}
				case INPUT_HOME:
				{
					m_currentCursorIndex = 0;
					break;
				}
				case INPUT_END:
				{
					if ( !m_currentConsoleCommandString.empty() ) m_currentCursorIndex = m_currentConsoleCommandString.length();
					break;
				}
				case INPUT_DELETE:
				{
					if ( !m_currentConsoleCommandString.empty() )
					{
						if ( m_currentCursorIndex != m_currentConsoleCommandString.length() )
						{
							m_currentConsoleCommandString.erase( m_currentConsoleCommandString.begin() + m_currentCursorIndex );
						}
					}
				}
			}
			break;
		}
	}

	return result;
}


//---------------------------------------------------------------------------
void DeveloperConsole::RegisterCommand( const std::string& name, CommandFunction* function, const std::string& description )
{
	if ( function == nullptr ) return;

	RegisteredCommand* command = new RegisteredCommand();
	command->m_commandName = name;
	command->m_commandDescription = description;
	command->m_function = function;

	g_registeredCommands[ name ] = command;
}


//---------------------------------------------------------------------------
void DeveloperConsole::ExecuteCommandString( const std::string& commandString )
{
	ConsoleCommandArguments arguments;
	std::string currentTokenString;
	char currentCharacter;
	for ( unsigned int index = 0; index < commandString.length(); ++index )
	{
		currentCharacter = commandString[ index ];

		if ( currentCharacter == ' ' )
		{
			if ( !currentTokenString.empty() )
			{
				arguments.m_argumentList.push_back( currentTokenString );
				currentTokenString.clear();
			}
		}
		else if ( currentCharacter == '\"' )
		{
			++index;
			currentCharacter = commandString[ index ];
			while ( currentCharacter != '\"' && index < commandString.length() )
			{
				currentTokenString += currentCharacter;
				++index;
				currentCharacter = commandString[ index ];
			}
			arguments.m_argumentList.push_back( currentTokenString );
			currentTokenString.clear();
		}
		else
		{
			currentTokenString += currentCharacter;
		}
	}

	if ( !currentTokenString.empty() )
	{
		arguments.m_argumentList.push_back( currentTokenString );
	}

	arguments.m_rawArgumentString = commandString;

	if ( !arguments.m_argumentList.empty() && g_registeredCommands.find( arguments.m_argumentList[ 0 ] ) != g_registeredCommands.end() )
	{
		g_registeredCommands[ arguments.m_argumentList[ 0 ] ]->m_function( arguments );
	}
	else
	{
		AddCommandLogLine( commandString, Colors::WHITE );
		if ( !arguments.m_argumentList.empty() )
		{
			AddCommandLogLine( "ERROR: Command \"" + arguments.m_argumentList[ 0 ] + "\" is not valid. Type \"help\" for a list of commands.",
				Colors::RED );
		}
		else
		{
			AddCommandLogLine( "ERROR: Command \"" + arguments.m_rawArgumentString + "\" is not valid. Type \"help\" for a list of commands.",
				Colors::RED );
		}
	}
}


//---------------------------------------------------------------------------
void DeveloperConsole::AddCommandLogLine( const std::string& text, const Rgba& color )
{
	CommandLogLine line;
	line.m_text = text;
	line.m_color = color;
	m_logLines.push_back( line );
}


//---------------------------------------------------------------------------
void DeveloperConsole::Render( const unsigned int shaderProgramID )
{
	g_theRenderer->VennDisable( V_DEPTH_TEST );
	RenderConsole( shaderProgramID );
	RenderCursorAndUpdateTimer( shaderProgramID );
	g_theRenderer->VennEnable( V_DEPTH_TEST );
}


//---------------------------------------------------------------------------
void DeveloperConsole::RenderConsole( const unsigned int shaderProgramID )
{
	float inputBoxOffset = 5.f;
	Vertex3D vertices[ 8 ];
	Vec2f inputWindowMins( 0.f, 0.f );
	Vec2f inputWindowMaxes( GAME_WINDOW_X, m_fontCellHeight + inputBoxOffset );
	Vec2f logWindowMins( 0.f, m_fontCellHeight + inputBoxOffset * 2.f );
	Vec2f logWindowMaxes( GAME_WINDOW_X, GAME_WINDOW_Y );
	Rgba windowColor( 0, 0, 0, 125 );
	Rgba commandTextColor( Colors::WHITE );

// 	g_theRenderer->VennLoadIdentity();
// 	g_theRenderer->VennOrtho( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
//	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );

	// the input window
	vertices[ 0 ].m_position = Vec3f( inputWindowMins.x, inputWindowMins.y, 0.f );
	vertices[ 0 ].m_color = windowColor;

	vertices[ 1 ].m_position = Vec3f( inputWindowMins.x, inputWindowMaxes.y, 0.f );
	vertices[ 1 ].m_color = windowColor;

	vertices[ 2 ].m_position = Vec3f( inputWindowMaxes.x, inputWindowMaxes.y, 0.f );
	vertices[ 2 ].m_color = windowColor;

	vertices[ 3 ].m_position = Vec3f( inputWindowMaxes.x, inputWindowMins.y, 0.f );
	vertices[ 3 ].m_color = windowColor;

	// render the log window
	vertices[ 4 ].m_position = Vec3f( logWindowMins.x, logWindowMins.y, 0.f );
	vertices[ 4 ].m_color = windowColor;

	vertices[ 5 ].m_position = Vec3f( logWindowMins.x, logWindowMaxes.y, 0.f );
	vertices[ 5 ].m_color = windowColor;

	vertices[ 6 ].m_position = Vec3f( logWindowMaxes.x, logWindowMaxes.y, 0.f );
	vertices[ 6 ].m_color = windowColor;

	vertices[ 7 ].m_position = Vec3f( logWindowMaxes.x, logWindowMins.y, 0.f );
	vertices[ 7 ].m_color = windowColor;

// 	g_theRenderer->VennEnableClientState( V_VERTEX_ARRAY );
// 	g_theRenderer->VennEnableClientState( V_COLOR_ARRAY );
// 
// 	g_theRenderer->VennVertexPointer( 3, V_FLOAT, sizeof( Vertex3D ), &vertices[0].m_position );
// 	g_theRenderer->VennColorPointer( 4, V_FLOAT, sizeof( Vertex3D ), &vertices[0].m_color );
// 
// 	g_theRenderer->VennDrawArrays( V_QUADS, 0, 8 );
// 
// 	g_theRenderer->VennDisableClientState( V_VERTEX_ARRAY );
// 	g_theRenderer->VennDisableClientState( V_COLOR_ARRAY );
	RenderVertexArrayWithShader( vertices, 8, shaderProgramID, V_QUADS );

	// render current command string
	m_textRenderer->RenderText2D( m_currentConsoleCommandString.c_str(), m_fontCellHeight, Vec2f( inputWindowMins.x, inputWindowMins.y ), commandTextColor, shaderProgramID );

	// render log lines
	for ( unsigned int index = m_logLines.size(); index > 0; --index )
	{
		if ( logWindowMins.y >= GAME_WINDOW_Y ) break;

		CommandLogLine& currentLine = m_logLines[ index - 1 ];
		m_textRenderer->RenderText2D( currentLine.m_text.c_str(), m_fontCellHeight, logWindowMins, currentLine.m_color, shaderProgramID );

		logWindowMins.y += m_fontCellHeight + inputBoxOffset;
	}
}


//---------------------------------------------------------------------------
void DeveloperConsole::RenderCursorAndUpdateTimer( const unsigned int shaderProgramID )
{
	if ( m_isCursorVisible )
	{
		Rgba color = Colors::WHITE;
		Vertex3D vertices[ 2 ];
		float cursorPosition = m_textRenderer->CalculateTextWidth( m_currentConsoleCommandString.substr( 0, m_currentCursorIndex ).c_str(), m_fontCellHeight );

		vertices[ 0 ].m_position = Vec3f( cursorPosition, 0.f, 0.f );
		vertices[ 0 ].m_color = color;
		vertices[ 1 ].m_position = Vec3f( cursorPosition, m_fontCellHeight, 0.f );
		vertices[ 1 ].m_color = color;

		RenderVertexArrayWithShader( vertices, 2, shaderProgramID, V_QUADS );

// 		g_theRenderer->VennEnableClientState( V_VERTEX_ARRAY );
// 		g_theRenderer->VennEnableClientState( V_COLOR_ARRAY );
// 		g_theRenderer->VennVertexPointer( 3, V_FLOAT, sizeof( Vertex3D ), &vertices[ 0 ].m_position );
// 		g_theRenderer->VennColorPointer( 4, V_FLOAT, sizeof( Vertex3D ), &vertices[ 0 ].m_color );
// 		g_theRenderer->VennDrawArrays( V_LINES, 0, 2 );
// 		g_theRenderer->VennDisableClientState( V_VERTEX_ARRAY );
// 		g_theRenderer->VennDisableClientState( V_COLOR_ARRAY );
	}

	m_currentCursorTimer -= DELTA_TIME;

	if ( m_currentCursorTimer <= 0.f )
	{
		m_currentCursorTimer = m_cursorBlinkTimerSeconds;
		m_isCursorVisible = !m_isCursorVisible;
	}
}


//---------------------------------------------------------------------------
void DeveloperConsole::ResetCursorTimer()
{
	m_currentCursorTimer = m_cursorBlinkTimerSeconds;
	m_isCursorVisible = true;
}