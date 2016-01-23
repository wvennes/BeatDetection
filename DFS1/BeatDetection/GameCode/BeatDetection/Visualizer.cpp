//---------------------------------------------------------------------------
// Visualizer.cpp
//---------------------------------------------------------------------------

// std includes
#include <algorithm>

// engine includes
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/StringTable.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Sound/Audio.hpp"

// project includes
#include "BeatDetection/BeatDetector.hpp"
#include "BeatDetection/Visualizer.hpp"

#define STATIC

STATIC const unsigned int	Visualizer::SAMPLE_RATE = 1 << 13; // MAX MOD = 13


//---------------------------------------------------------------------------
Visualizer::Visualizer( AudioSystem* audioSystem, unsigned int soundID )
	: m_soundID( soundID )
	, m_audioSystem( audioSystem )
	, m_beatDetector( nullptr )
	, m_currentSongPositionMilliseconds( 0 )
	, m_totalSongLengthMilliseconds( 0 )
	, m_visualizerState( VISUALIZER_STATE_SHOWING_BEAT_DETECTOR )
{
	Initialize();
}


//---------------------------------------------------------------------------
Visualizer::~Visualizer()
{
	m_audioSystem->StopLoopedMusic();
	delete m_beatDetector;
}


//---------------------------------------------------------------------------
void Visualizer::Initialize()
{
	m_realTimeSpectrumData.reserve( 1024 );

	m_beatDetector = new BeatDetector( m_soundID, m_audioSystem );
//	m_beatDetector->ProcessAudio();

	//std::vector< int > left;
	//std::vector< int > right;
	//m_audioSystem->GetFullFileSpectrumDataFFT( m_soundID, left, right );
	m_audioSystem->GetLengthMS( m_soundID, m_totalSongLengthMilliseconds );

	m_audioSystem->StartLoopedMusic( m_soundID );
}


//---------------------------------------------------------------------------
void Visualizer::ProcessInput()
{
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_SPACE ) )
	{
		if ( m_visualizerState == VISUALIZER_STATE_SHOWING_BEAT_DETECTOR )
		{
			m_visualizerState = VISUALIZER_STATE_SHOWING_VISUALIZER;
		}
		else
		{
			m_visualizerState = VISUALIZER_STATE_SHOWING_BEAT_DETECTOR;
		}
	}

	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_UP ) )
	{
		m_beatDetector->IncrementEnergyRatioForBeatThreshold( 0.1f );
	}

	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_DOWN ) )
	{
		m_beatDetector->IncrementEnergyRatioForBeatThreshold( -0.1f );
	}

	if ( g_theInputManager->HasKeyBeenPressed( 'P' ) )
	{
		m_audioSystem->TogglePause();
	}
}


//---------------------------------------------------------------------------
void Visualizer::Update()
{
	if ( m_visualizerState == VISUALIZER_STATE_SHOWING_VISUALIZER ) UpdateShowingVisualizer();
	m_audioSystem->GetCurrentPositionMS( m_currentSongPositionMilliseconds );
	m_beatDetector->Update();
}


//---------------------------------------------------------------------------
void Visualizer::UpdateShowingVisualizer()
{
	m_audioSystem->GetAverageSpectrumData( m_realTimeSpectrumData, SAMPLE_RATE );

	float max = *std::max_element( m_realTimeSpectrumData.begin(), m_realTimeSpectrumData.end() );
	float oneOverMax = 1.f / max;

	// normalize the data
	for ( SpectrumData::iterator iter = m_realTimeSpectrumData.begin(); iter != m_realTimeSpectrumData.end(); ++ iter )
	{
		*iter = ( *iter ) * oneOverMax;
	}
}


//---------------------------------------------------------------------------
void Visualizer::Render()
{
	switch( m_visualizerState )
	{
		case VISUALIZER_STATE_SHOWING_VISUALIZER:
		{
			RenderShowingVisualizer();
			break;
		}
		case VISUALIZER_STATE_SHOWING_BEAT_DETECTOR:
		{
			RenderShowingBeatDetector();
			break;
		}
	}

	// always render this information
	RenderTimelineAndEnergyRatio();
}


//---------------------------------------------------------------------------
void Visualizer::RenderTimelineAndEnergyRatio()
{
	std::vector< Vertex3D > vertexes;
	Vertex3D currentVertex;
	float lineLength = 500.f;
	float radius = 10.f;
	float energyRatioCellHeight = 64.f;
	float currentTimeCellHeight = 32.f;
	float bpmCellHeight = 32.f;
	Vec2f startPosition( GAME_WINDOW_X * 0.5f - 250.f, GAME_WINDOW_Y - 200.f );
	Vec2f endPosition( startPosition.x + lineLength, startPosition.y );
	float currentSongLocation = MapValueInOldRangeToNewRange(
		( float ) m_currentSongPositionMilliseconds,
		0.f,
		( float ) m_totalSongLengthMilliseconds,
		0.f,
		lineLength );

	Vec2f currentSongPosition( currentSongLocation + startPosition.x, startPosition.y );

	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	Material* mat = Material::GetDefaultUnlitMaterial();
	mat->ActivateMaterial();

	// render the timeline
	currentVertex.m_color = Colors::WHITE;
	currentVertex.m_position = Vec3f( startPosition.x, startPosition.y, 0.f );
	vertexes.push_back( currentVertex );

	currentVertex.m_position = Vec3f( endPosition.x, endPosition.y, 0.f );
	vertexes.push_back( currentVertex );
	RenderVertexArrayWithShader( vertexes.data(), vertexes.size(), mat->GetShaderProgramID(), V_LINES );
	vertexes.clear();

	// render a quad where the current location is
	currentVertex.m_position = Vec3f( currentSongPosition.x - radius, currentSongPosition.y - radius, 0.f );
	vertexes.push_back( currentVertex );

	currentVertex.m_position = Vec3f( currentSongPosition.x - radius, currentSongPosition.y + radius, 0.f );
	vertexes.push_back( currentVertex );

	currentVertex.m_position = Vec3f( currentSongPosition.x + radius, currentSongPosition.y + radius, 0.f );
	vertexes.push_back( currentVertex );

	currentVertex.m_position = Vec3f( currentSongPosition.x + radius, currentSongPosition.y - radius, 0.f );
	vertexes.push_back( currentVertex );

	RenderVertexArrayWithShader( vertexes.data(), vertexes.size(), mat->GetShaderProgramID(), V_QUADS );
	vertexes.clear();

	// render the current energy ratio being used to detect beats
	g_theTextRenderer->TextRendererPrintf(
		energyRatioCellHeight,
		endPosition + Vec2f( 100.f, 0.f ),
		Colors::WHITE,
		mat->GetShaderProgramID(),
		"Energy ratio: %0.1f",
		m_beatDetector->GetEnergyRatioForBeatThreshold() );

	// render the current and total times of the song
	unsigned int currentSeconds = m_currentSongPositionMilliseconds / 1000;
	unsigned int currentMinutes = currentSeconds / 60;
	currentSeconds = currentSeconds % 60;

	unsigned int totalSeconds = m_totalSongLengthMilliseconds / 1000;
	unsigned int totalMinutes = totalSeconds / 60;
	totalSeconds = totalSeconds % 60;

	g_theTextRenderer->TextRendererPrintf(
		currentTimeCellHeight,
		startPosition + Vec2f( 0.f, currentTimeCellHeight ),
		Colors::WHITE,
		mat->GetShaderProgramID(),
		"%i:%02i/%i:%02i",
		currentMinutes,
		currentSeconds,
		totalMinutes,
		totalSeconds );

	// flash BPM for beats per minute
	float bpm = m_beatDetector->GetBPMForSubband( 0 );
	unsigned int currentSongTime = 0;
	m_audioSystem->GetCurrentPositionMS( currentSongTime );
	currentSongTime = currentSongTime * 100;
	unsigned int interval = 0;

	if ( bpm > 0 ) interval = ( unsigned int ) ( ( ( ( ( 60.f / bpm ) * 1000.f ) * 100000.f ) / 100000.f ) * 100.f );

	if ( interval > 0 && ( currentSongTime % interval <= 3500 && currentSongTime % interval >=0 ) )
	{
		g_theTextRenderer->TextRendererPrintf( bpmCellHeight, endPosition + Vec2f( 100.f, -100.f ), Colors::WHITE, mat->GetShaderProgramID(), "BPM" );
	}
	g_theTextRenderer->TextRendererPrintf( bpmCellHeight, endPosition + Vec2f( 100.f, -132.f ), Colors::WHITE, mat->GetShaderProgramID(), "%.2f", m_beatDetector->GetBPMForSubband( 0 ) );
}


//---------------------------------------------------------------------------
void Visualizer::RenderShowingBeatDetector()
{
	//m_beatDetector->RenderInstantEnergies();
	m_beatDetector->Render();

	Material* mat = Material::GetDefaultUnlitMaterial();
	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	if ( !m_beatDetector->IsBeat() ) return;

	// render the word "Beat!" based on the real-time beat data instead of the BPM
	g_theTextRenderer->TextRendererPrintf( 100.f, Vec2f( GAME_WINDOW_X * 0.5f, GAME_WINDOW_Y * 0.5f ), Colors::WHITE, mat->GetShaderProgramID(), "Beat!" );
}


//---------------------------------------------------------------------------
void Visualizer::RenderShowingVisualizer()
{
	Material* mat = Material::GetDefaultUnlitMaterial();
	static std::vector< Vertex3D > vertexes( SAMPLE_RATE );
	 
	float xOffset = GAME_WINDOW_X / ( ( float ) ( 2 * SAMPLE_RATE ) );
	float yOffset = 100.f;
	float yScale = 500.f;
	float currentPositionX = 0.f;
	float currentPositionY = 0.f;
	float currentColorValue = 0.f;
	float currentData = 0.f;
	Vertex3D currentVertex;
	 
	// left half
	for ( unsigned int index = 0; index < ( unsigned int ) m_realTimeSpectrumData.size(); ++ index )
	{
	 	currentData = m_realTimeSpectrumData[ m_realTimeSpectrumData.size() - 1 - index ];
	 
	 	currentPositionY = yOffset + ( currentData * yScale );
	 	currentColorValue = MapValueInOldRangeToNewRange( currentPositionY - yOffset, 0.f, yScale, 0.f, 255.f );
	 	currentVertex.m_position = Vec3f( currentPositionX, currentPositionY, 0.f );
	 	currentVertex.m_color = Rgba( ( unsigned char ) currentColorValue, 75, 0, 255 );
	 	currentPositionX += xOffset;
	 
	 	vertexes.push_back( currentVertex );
	}
	 
	// right half
	for ( unsigned int index = 0; index < ( unsigned int ) m_realTimeSpectrumData.size(); ++ index )
	{
	 	currentData = m_realTimeSpectrumData[ index ];
	 
	 	currentPositionY = yOffset + ( currentData * yScale );
	 	currentColorValue = MapValueInOldRangeToNewRange( currentPositionY - yOffset, 0.f, yScale, 0.f, 255.f );
	 	currentVertex.m_position = Vec3f( currentPositionX, currentPositionY, 0.f );
	 	currentVertex.m_color = Rgba( ( unsigned char ) currentColorValue, 75, 0, 255 );
	 	currentPositionX += xOffset;
	 
	 	vertexes.push_back( currentVertex );
	}
	 
	RenderVertexArrayWithShader( vertexes.data(), vertexes.size(), mat->GetShaderProgramID(), V_LINES );
	 
	vertexes.clear();
}