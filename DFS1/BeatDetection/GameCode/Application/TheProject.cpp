//---------------------------------------------------------------------------
// TheProject.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Core/StringTable.hpp"
#include "Engine/Input/XboxControllerHandler.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Sound/Audio.hpp"

#include "BeatDetection/Visualizer.hpp"
#include "BeatGame/TheGame.hpp"
#include "Application/TheProject.hpp"


//---------------------------------------------------------------------------
TheProject::TheProject()
	: m_isQuitting( false )
	, m_currentSongID( 0 )
	, m_currentSongIndex( 0 )
	, m_theAudioSystem( nullptr )
	, m_visualizer( nullptr )
	, m_theGame( nullptr )
	, m_projectState( STATE_SHOWING_MENU )
{
	Initialize();
}


//---------------------------------------------------------------------------
TheProject::~TheProject()
{
	delete m_theAudioSystem;
	if ( m_visualizer ) delete m_visualizer;
	if ( m_theGame ) delete m_theGame;
	Material::FreeMaterialRegistryMemory();
}


//---------------------------------------------------------------------------
void TheProject::Initialize()
{
	m_theAudioSystem = new AudioSystem();

	std::vector< std::string > outFilesFound;
	FileSystem::EnumerateFilesInDirectory( "Data/Music/*", outFilesFound );

	for ( unsigned int index = 0; index < ( unsigned int ) outFilesFound.size(); ++ index )
	{
		m_soundFilePathIDs.push_back( StringID( outFilesFound[ index ] ) );
	}

	if ( m_soundFilePathIDs.empty() )
	{
		DebuggerPrintf( "No song files found!\n" );
		__debugbreak();
	}

	//m_shaderProgram = new ShaderProgram( "Data/Shaders/DoNothing_vs.glsl", "Data/Shaders/DoNothing_fs.glsl" );
	SetUpMaterials();
	//int x = 0;
	//m_currentSongID = m_theAudioSystem->CreateOrGetSound( "Data/Music/Rostik - Liquid (Paul Rosenthal Remix).mp3" );
	//m_theAudioSystem->StartLoopedMusic( m_currentSongID );
}


//---------------------------------------------------------------------------
void TheProject::SetUpMaterials()
{
	Material::GenerateDefaultMaterials();
	// text material
// 	Material* material = Material::CreateAndGetMaterial( 
// 		"TextRender",
// 		new ShaderProgram( "Data/Shaders/TextRender_vs.glsl", "Data/Shaders/TextRender_fs.glsl" ) );
// 
// 	material->RegisterStaticUniform1i( "u_diffuseMap", 0 );
}


//---------------------------------------------------------------------------
void TheProject::ProcessInput()
{
	switch ( m_projectState )
	{
		case STATE_SHOWING_MENU:
		{
			ProcessInputShowingMenu();
			break;
		}
		case STATE_SHOWING_VISUALIZER:
		{
			ProcessInputShowingVisualizer();
			break;
		}
		case STATE_SHOWING_GAME:
		{
			ProcessInputShowingGame();
			break;
		}
	}
}


//---------------------------------------------------------------------------
void TheProject::ProcessInputShowingMenu()
{
	// quit if the application is quitting
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_ESCAPE ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_GAMEPAD_B ) )
	{
		m_isQuitting = true;
	}

	// process application input
	if ( g_theInputManager->HasKeyBeenPressed( 'V' ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_GAMEPAD_BACK ) )
	{
		m_currentSongID = m_theAudioSystem->CreateOrGetSound( StringValue( m_soundFilePathIDs[ m_currentSongIndex ] ) );
		m_projectState = STATE_SHOWING_VISUALIZER;
		m_visualizer = new Visualizer( m_theAudioSystem, m_currentSongID );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'G' ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_GAMEPAD_START ) )
	{
		m_projectState = STATE_LOADING_GAME;
	}
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_RIGHT ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_DPAD_RIGHT ) )
	{
		++ m_currentSongIndex;
		if ( ( unsigned int ) m_currentSongIndex >= m_soundFilePathIDs.size() )
		{
			m_currentSongIndex = 0;
		}
	}
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_LEFT ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_DPAD_LEFT ) )
	{
		-- m_currentSongIndex;
		if ( m_currentSongIndex < 0 )
		{
			m_currentSongIndex = ( int ) ( m_soundFilePathIDs.size() ) - 1;
		}
	}
}


//---------------------------------------------------------------------------
void TheProject::ProcessInputShowingVisualizer()
{
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_ESCAPE ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_GAMEPAD_B ) )
	{
		m_projectState = STATE_SHOWING_MENU;
		delete m_visualizer;
		m_visualizer = nullptr;
		return;
	}

	m_visualizer->ProcessInput();
}


//---------------------------------------------------------------------------
void TheProject::ProcessInputShowingGame()
{
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_ESCAPE ) || g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_GAMEPAD_B ) )
	{
		m_projectState = STATE_SHOWING_MENU;
		delete m_theGame;
		m_theGame = nullptr;
		return;
	}

	m_theGame->ProcessInput();
}


//---------------------------------------------------------------------------
bool TheProject::Update()
{
	g_xboxHandlerOne.UpdateControllerState();

	switch ( m_projectState )
	{
		case STATE_SHOWING_MENU:
		{
			UpdateShowingMenu();
			break;
		}
		case STATE_SHOWING_VISUALIZER:
		{
			UpdateShowingVisualizer();
			break;
		}
		case STATE_SHOWING_GAME:
		{
			UpdateShowingGame();
			break;
		}
	}
	return m_isQuitting;
}


//---------------------------------------------------------------------------
void TheProject::UpdateShowingMenu()
{
	//m_theAudioSystem->Update();
}


//---------------------------------------------------------------------------
void TheProject::UpdateShowingVisualizer()
{
	m_theAudioSystem->Update();
	m_visualizer->Update();
}


//---------------------------------------------------------------------------
void TheProject::UpdateShowingGame()
{
	bool isSongPlaying = m_theGame->Update();
	if ( !isSongPlaying )
	{
		if ( ( unsigned int ) m_currentSongIndex == m_soundFilePathIDs.size() - 1 )
		{
			m_currentSongIndex = 0;
		}
		else
		{
			++ m_currentSongIndex;
		}

		m_currentSongID = m_theAudioSystem->CreateOrGetSound( StringValue( m_soundFilePathIDs[ m_currentSongIndex ] ) );
		m_theGame->SendNextSong( m_currentSongID );
	}
}


//---------------------------------------------------------------------------
void TheProject::Render()
{
	switch ( m_projectState )
	{
		case STATE_SHOWING_MENU:
		{
			RenderShowingMenu();
			return;
		}
		case STATE_SHOWING_VISUALIZER:
		{
			RenderShowingVisualizer();
			return;
		}
		case STATE_LOADING_GAME:
		{
			RenderLoadingGame();
			return;
		}
		case STATE_SHOWING_GAME:
		{
			RenderShowingGame();
			return;
		}
	}
}


//---------------------------------------------------------------------------
void TheProject::RenderShowingMenu()
{
	float cellSize = 64.f;
	Vec2f chooseSongMessagePosition( 0.f, GAME_WINDOW_Y * 0.5f );
	Vec2f songFileNamePosition = chooseSongMessagePosition - Vec2f( 0, cellSize );
	Vec2f projectModeMessagePosition = songFileNamePosition - Vec2f( 0, 5.f * cellSize );

	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	Material* textMaterial = Material::GetDefaultUnlitMaterial();
	textMaterial->ActivateMaterial();
	g_theTextRenderer->TextRendererPrintf( cellSize, chooseSongMessagePosition, Colors::WHITE, textMaterial->GetShaderProgramID(),
		"Left and right arrow keys to scroll through songs." );
	g_theTextRenderer->TextRendererPrintf( cellSize, songFileNamePosition, Colors::WHITE, textMaterial->GetShaderProgramID(), 
		"<-- %s -->", StringValue( m_soundFilePathIDs[ m_currentSongIndex ] ) );
	g_theTextRenderer->TextRendererPrintf( cellSize, projectModeMessagePosition, Colors::WHITE, textMaterial->GetShaderProgramID(),
		"Press G to play Game, Press V to open Visualizer" );
}


//---------------------------------------------------------------------------
void TheProject::RenderShowingVisualizer()
{
	float cellSize = 64.f;
	Vec2f fileNamePosition( 0.f, GAME_WINDOW_Y - cellSize );

	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	Material* textMaterial = Material::GetDefaultUnlitMaterial();
	textMaterial->ActivateMaterial();
	g_theTextRenderer->TextRendererPrintf( 64.f, fileNamePosition, Colors::WHITE, textMaterial->GetShaderProgramID(), 
		"Filename: %s", StringValue( m_soundFilePathIDs[ m_currentSongIndex ] ) );

	m_visualizer->Render();
}


//---------------------------------------------------------------------------
void TheProject::RenderLoadingGame()
{
	static char count = 0;
	float cellSize = 64.f;
	Vec2f fileNamePosition( GAME_WINDOW_X * 0.5f - 200.f, GAME_WINDOW_Y * 0.5f );

	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	Material* textMaterial = Material::GetDefaultUnlitMaterial();
	textMaterial->ActivateMaterial();
	g_theTextRenderer->TextRendererPrintf( cellSize, fileNamePosition, Colors::WHITE, textMaterial->GetShaderProgramID(), "Loading songs..." );
	++ count;
	if ( count < 10 ) return;

	for ( unsigned int index = 0; index < ( unsigned int ) m_soundFilePathIDs.size(); ++ index )
	{
		m_theAudioSystem->CreateOrGetSound( StringValue( m_soundFilePathIDs[ index ] ) );
	}
	m_currentSongID = m_theAudioSystem->CreateOrGetSound( StringValue( m_soundFilePathIDs[ m_currentSongIndex ] ) );
	m_theGame = new TheGame( m_theAudioSystem, m_currentSongID );
	m_projectState = STATE_SHOWING_GAME;
}


//---------------------------------------------------------------------------
void TheProject::RenderShowingGame()
{
	m_theGame->Render();
}