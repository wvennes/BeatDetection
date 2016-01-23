//---------------------------------------------------------------------------
// BeatDetector.cpp
//---------------------------------------------------------------------------

#include <algorithm>

#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Sound/Audio.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"

#include "BeatDetection/BeatDetector.hpp"

#define STATIC

STATIC float			BeatDetector::s_energyRatio = 1.5f;
STATIC int				BeatDetector::s_sampleRateBits = 10; // leave this alone - we want 1024, haven't tested with any other values
STATIC int				BeatDetector::s_detectionSampleRate = 1 << s_sampleRateBits; // should be 1024
STATIC int				BeatDetector::s_numSubbands = 32; // 32 subbands, use more for greater sensitivity
STATIC int				BeatDetector::s_numHistoryValues = 43; // 43 values in the history buffer corresponds to 1 second of music for 44100 hz sample rate
STATIC unsigned int		BeatDetector::s_beatCutoffMS = 1000; // cutoff to remove older beats from the beat queue


//---------------------------------------------------------------------------
BeatDetector::BeatDetector( int soundID, AudioSystem* audioSystem )
	: m_instantEnergyBuffer( nullptr )
	, m_energyHistoryBuffer( nullptr )
	, m_audioSystem( audioSystem )
	, m_soundID( soundID )
	, m_soundLengthPCM( 0 )
	, m_millisecondsPerBeat( 0.f )
{
	Initialize();
}


//---------------------------------------------------------------------------
BeatDetector::~BeatDetector()
{
	delete m_instantEnergyBuffer;
	delete m_energyHistoryBuffer;
	delete m_energyPeaks;
}


//---------------------------------------------------------------------------
void BeatDetector::Initialize()
{
	m_audioSystem->GetLengthPCM( m_soundID, m_soundLengthPCM );
	m_instantEnergyBuffer = new float[ m_soundLengthPCM >> s_sampleRateBits ]; // integer divide by 1024, because we iterate by 1024 samples for the whole file
	m_energyHistoryBuffer = new float[ m_soundLengthPCM >> s_sampleRateBits ];
	m_energyPeaks = new float[ ( m_soundLengthPCM >> s_sampleRateBits ) + 21 ]; // 21 is about half a second for energy history, start in the middle of the first peak
	for ( int index = 0; index < ( int ) ( m_soundLengthPCM >> s_sampleRateBits ) + 21; ++ index )
	{
		m_energyPeaks[ index ] = 0.f;
	}

	m_fftSubbandAverageEnergies.reserve( s_numSubbands );
	m_fftSubbandInstantEnergyBuffer.reserve( s_numSubbands );
	m_fftSubbandEnergyHistoryBuffers.reserve( s_numSubbands );
	for ( int count = 0; count < s_numSubbands; ++ count )
	{
		m_fftSubbandEnergyHistoryBuffers.push_back( std::vector< float >() );
		m_beatTimesForSubband.push_back( std::queue< unsigned int >() );
		m_bpmEstimates.push_back( 0.f );
		m_beatHistory.push_back( std::map< float, unsigned int >() );
	}
}


//---------------------------------------------------------------------------
int BeatDetector::CalculateInstantEnergy( int* data, int dataOffset, int dataWindow )
{
	float resultEnergy = 0.f;
	float oneOverWindow = 1.f / ( ( float ) dataWindow );
	float dataAsFloat = 0.f;

	for ( int index = dataOffset; ( index < dataOffset + dataWindow ) && ( index < ( int ) m_soundLengthPCM ); ++ index )
	{
		dataAsFloat = ( float ) data[ index ];
		resultEnergy = resultEnergy + dataAsFloat * dataAsFloat * oneOverWindow;
	}

	return ( int ) resultEnergy;
}


//---------------------------------------------------------------------------
int BeatDetector::CalculateInstantEnergy( int* leftData, int* rightData, int dataOffset, int dataWindow )
{
	float resultEnergy = 0.f;
	float leftAsFloat = 0.f;
	float rightAsFloat = 0.f;
	for ( int index = dataOffset; ( index < dataOffset + dataWindow ) && ( index < ( int ) m_soundLengthPCM ); ++ index )
	{
		leftAsFloat = ( float ) leftData[ index ];
		rightAsFloat = ( float ) rightData[ index ];

		resultEnergy += leftAsFloat * leftAsFloat + rightAsFloat * rightAsFloat;
	}

	return ( int ) resultEnergy;
}


//---------------------------------------------------------------------------
void BeatDetector::Update()
{
	float* leftData = nullptr;
	float* rightdata = nullptr;
	m_audioSystem->GetSpectrumData( leftData, rightdata, s_detectionSampleRate );
	float* squaredModsOfSpectrumData = GetSquaredModsOfSpectrumData( leftData, rightdata, s_detectionSampleRate );
	CalculateInstantEnergies( squaredModsOfSpectrumData, m_fftSubbandInstantEnergyBuffer, s_numSubbands, s_detectionSampleRate );
	UpdateEnergyHistoryBuffers( m_fftSubbandInstantEnergyBuffer, m_fftSubbandEnergyHistoryBuffers );

	CalculateBPMs();

	delete leftData;
	delete rightdata;
	delete squaredModsOfSpectrumData;
}


// for the FFT banding approach, we need the square of the mod of the complex number formed by audio data
// -- complex number = left + right * i
// -- mod = sqrt( left^2 + right^2 )
// returns frequency amplitudes of signal
//---------------------------------------------------------------------------
float* BeatDetector::GetSquaredModsOfSpectrumData( float* leftData, float* rightData, int dataWindow /* = 1024 */ )
{
	float* result = new float[ dataWindow ];
	for ( int index = 0; index < dataWindow; ++ index )
	{
		result[ index ] = leftData[ index ] * leftData[ index ] + rightData[ index ] * rightData[ index ];
	}

	//float x = result[ dataWindow - 1 ];

	return result;
}


//---------------------------------------------------------------------------
void BeatDetector::CalculateInstantEnergies( float* squaredModsOfSpectrumData, std::vector< float >& instantEnergyBuffer, int numSubbands, int dataWindow /* = 1024 */ )
{
	instantEnergyBuffer.clear();
	float constant = ( ( float ) numSubbands ) / ( ( float ) dataWindow );

	// make sure the spectrum data has at least numSubbands^2 entries of data
	for ( int subband = 0; subband < numSubbands; ++ subband )
	{
		float result = 0;
		for ( int index = subband * numSubbands; index < ( subband + 1 ) * numSubbands; ++ index )
		{
			if ( index > dataWindow ) break;
			result += constant * squaredModsOfSpectrumData[ index ];
		}
		instantEnergyBuffer.push_back( result );
	}
}


//---------------------------------------------------------------------------
void BeatDetector::UpdateEnergyHistoryBuffers( const std::vector< float >& instantEnergyBuffer, std::vector< std::vector< float > >& energyHistoryBuffers )
{
	m_fftSubbandAverageEnergies.clear();
	float averageEnergy = 0.f;
	for ( unsigned int subband = 0; subband < ( unsigned int ) instantEnergyBuffer.size(); ++ subband )
	{
		averageEnergy = CalculateAverageEnergyForSubband( subband, energyHistoryBuffers );
		m_fftSubbandAverageEnergies.push_back( averageEnergy );
	}
}


//---------------------------------------------------------------------------
void BeatDetector::CalculateBPMs()
{
	static bool haveBeatsBeenInitialized = false;
	static bool hasBeatBeenDetectedForSubband[ 32 ];

	if ( !haveBeatsBeenInitialized )
	{
		for ( int index = 0; index < 32; ++ index )
		{
			hasBeatBeenDetectedForSubband[ index ] = false;
		}
		haveBeatsBeenInitialized = true;
	}

	unsigned int currentTime = 0;
	for ( int subband = 0; subband < s_numSubbands; ++ subband )
	{
		std::queue< unsigned int >& beatTimes = m_beatTimesForSubband[ subband ];

		// remove the oldest beats if they're past our cutoff time
		if ( IsBeat( subband ) )
		{
			if ( !hasBeatBeenDetectedForSubband[ subband ] )
			{
				hasBeatBeenDetectedForSubband[ subband ] = true;
				m_audioSystem->GetCurrentPositionMS( currentTime );
				beatTimes.push( currentTime );

				while ( currentTime - beatTimes.front() > s_beatCutoffMS )
				{
					beatTimes.pop();
					if ( beatTimes.empty() ) break;
				}
			}
		}
		else
		{
			hasBeatBeenDetectedForSubband[ subband ] = false;
		}

		// estimate the bpm if we have 2 or more beats based on the average time between the last and first beat ticks
		if ( beatTimes.size() >= 2 )
		{
			m_millisecondsPerBeat = ( float ) ( beatTimes.back() - beatTimes.front() ) / ( ( float ) beatTimes.size() - 1 );
			m_bpmEstimates[ subband ] = 60000.f / m_millisecondsPerBeat;
			float roundedBpmEstimate = ( float ) ( floor( m_bpmEstimates[ subband ] * 100.f ) / 100.f );
			if ( m_beatHistory[ subband ].find( roundedBpmEstimate ) != m_beatHistory[ subband ].end() )
			{
				++ m_beatHistory[ subband ][ roundedBpmEstimate ];
			}
			else
			{
				m_beatHistory[ subband ].insert( std::make_pair( roundedBpmEstimate, 1 ) );
			}
		}
		else
		{
			m_bpmEstimates[ subband ] = 0.f;
		}
	}
}


//---------------------------------------------------------------------------
float BeatDetector::CalculateAverageEnergyForSubband( int subband, std::vector< std::vector< float > >& energyHistoryBuffers )
{
	std::vector< float >& historyBufferForSubband = energyHistoryBuffers[ subband ];
	historyBufferForSubband.push_back( m_fftSubbandInstantEnergyBuffer[ subband ] );
	if ( historyBufferForSubband.size() > ( unsigned int ) s_numHistoryValues )
	{
		historyBufferForSubband.erase( historyBufferForSubband.begin() );
	}
	//float oneOverNumHistoryValues = 1.f / ( ( float ) NUM_HISTORY_VALUES );
	float result = 0.f;
	unsigned int count = 0;
	for ( ; count < ( unsigned int ) historyBufferForSubband.size(); ++ count )
	{
		result += historyBufferForSubband[ count ];
	}
	result = result / ( count + 1 );

	return result;
}


//---------------------------------------------------------------------------
bool BeatDetector::IsBeat( int subband /* = 0 */ ) const
{
	float instantEnergy = m_fftSubbandInstantEnergyBuffer[ subband ];
	float averageEnergy = m_fftSubbandAverageEnergies[ subband ];

	return ( instantEnergy > ( s_energyRatio * averageEnergy ) );
}


//---------------------------------------------------------------------------
float BeatDetector::GetBPMForSubband( int subband /* = 0 */ ) const
{
	/*return m_bpmEstimates[ subband ];*/
	float bestGuessBpm = 0.f;
	unsigned int highestCount = 0;
	for ( std::map< float, unsigned int >::const_iterator iter = m_beatHistory[ subband ].begin(); iter != m_beatHistory[ subband ].end(); ++ iter )
	{
		if ( iter->second > highestCount )
		{
			bestGuessBpm = iter->first;
			highestCount = iter->second;
		}
	}

	return bestGuessBpm;
}


//---------------------------------------------------------------------------
float BeatDetector::GetEnergyRatioForBeatThreshold() const
{
	return s_energyRatio;
}


//---------------------------------------------------------------------------
void BeatDetector::IncrementEnergyRatioForBeatThreshold( float value )
{
	s_energyRatio += value;
	if ( s_energyRatio <= 0.f ) s_energyRatio = 0.f;
}


//---------------------------------------------------------------------------
void BeatDetector::RenderInstantEnergies()
{
	static float valueScale = 0.0000001f;
	std::vector< Vertex3D > energyInstantVertexes;
	std::vector< Vertex3D > energyHistoryVertexes;
	std::vector< Vertex3D > energyPeaks;
	Vertex3D currentVert;
	
	g_theRenderer->ApplyOrthoProjection( 0.f, ( float ) ( m_soundLengthPCM >> s_sampleRateBits ), 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	for ( unsigned int x = 0; x < m_soundLengthPCM >> s_sampleRateBits; ++ x )
	{
		currentVert.m_position = Vec3f( ( float ) x, m_energyHistoryBuffer[ x ] * valueScale, 0.f );
		currentVert.m_color = Colors::RED;
		energyHistoryVertexes.push_back( currentVert );

		currentVert.m_position = Vec3f( ( float ) x, m_instantEnergyBuffer[ x ] * valueScale, 0.f );
		currentVert.m_color = Colors::YELLOW;
		energyInstantVertexes.push_back( currentVert );

		if ( m_energyPeaks[ x ] > 0.f )
		{
			currentVert.m_position = Vec3f( ( float ) x, 0.f, 0.f );
			currentVert.m_color = Colors::WHITE;
			energyPeaks.push_back( currentVert );

			currentVert.m_position = Vec3f( ( float ) x, 500.f, 0.f );
			energyPeaks.push_back( currentVert );
		}
	}

	Material* mat = Material::GetDefaultUnlitMaterial();
	mat->ActivateMaterial();
	RenderVertexArrayWithShader( energyInstantVertexes.data(), energyInstantVertexes.size(), mat->GetShaderProgramID(), V_LINES );
	RenderVertexArrayWithShader( energyHistoryVertexes.data(), energyHistoryVertexes.size(), mat->GetShaderProgramID(), V_LINES );
	//RenderVertexArrayWithShader( energyPeaks.data(), energyPeaks.size(), mat->GetShaderProgramID(), V_LINES );
}


//---------------------------------------------------------------------------
void BeatDetector::Render()
{
	RenderFFTInstantEnergies();
	RenderFFTAverageEnergies();
}


//---------------------------------------------------------------------------
void BeatDetector::RenderFFTInstantEnergies()
{
	static float valueScale = 25000.f;
	static float yOffset = 100.f;
	std::vector< Vertex3D > instantEnergyVertexes;
	Vertex3D currentVertex;
	g_theRenderer->ApplyOrthoProjection( 0.f, ( float ) s_numSubbands, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	for ( unsigned int index = 0; index < ( unsigned int ) m_fftSubbandInstantEnergyBuffer.size(); ++ index )
	{
		currentVertex.m_color = Colors::BLUE;

		currentVertex.m_position.x = ( float ) index;
		currentVertex.m_position.y = yOffset;
		instantEnergyVertexes.push_back( currentVertex );

		currentVertex.m_color = Colors::RED;
		currentVertex.m_position.y = m_fftSubbandInstantEnergyBuffer[ index ] * valueScale + yOffset;
		instantEnergyVertexes.push_back( currentVertex );

		currentVertex.m_position.x += 1.f;
		instantEnergyVertexes.push_back( currentVertex );

		currentVertex.m_color = Colors::BLUE;
		currentVertex.m_position.y = yOffset;
		instantEnergyVertexes.push_back( currentVertex );

		if ( IsBeat( index ) )
		{
			currentVertex.m_color = Colors::GREY;
			currentVertex.m_position = Vec3f( ( float ) index, 0.f, 0.f );
			instantEnergyVertexes.push_back( currentVertex );

			currentVertex.m_position = Vec3f( ( float ) index, yOffset * 0.5f, 0.f );
			instantEnergyVertexes.push_back( currentVertex );

			currentVertex.m_position = Vec3f( ( float ) index + 1.f, yOffset * 0.5f, 0.f );
			instantEnergyVertexes.push_back( currentVertex );

			currentVertex.m_position = Vec3f( ( float ) index + 1.f, 0.f, 0.f );
			instantEnergyVertexes.push_back( currentVertex );
		}
	}

	Material* mat = Material::GetDefaultUnlitMaterial();
	mat->ActivateMaterial();
	RenderVertexArrayWithShader( instantEnergyVertexes.data(), instantEnergyVertexes.size(), mat->GetShaderProgramID(), V_QUADS );
}


//---------------------------------------------------------------------------
void BeatDetector::RenderFFTAverageEnergies()
{
	static float valueScale = 25000.f;
	static float yOffset = 100.f;
	static unsigned char shade = 120;
	static unsigned char alpha = 1;
	std::vector< Vertex3D > averageEnergyVertexes;
	Vertex3D currentVertex;
	g_theRenderer->ApplyOrthoProjection( 0.f, ( float ) s_numSubbands, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	for ( unsigned int index = 0; index < ( unsigned int ) m_fftSubbandAverageEnergies.size(); ++ index )
	{
		currentVertex.m_color = Rgba( shade, shade, shade, alpha );

		currentVertex.m_position.x = ( float ) index;
		currentVertex.m_position.y = yOffset;
		averageEnergyVertexes.push_back( currentVertex );

		currentVertex.m_position.y = m_fftSubbandAverageEnergies[ index ] * valueScale + yOffset;
		averageEnergyVertexes.push_back( currentVertex );

		currentVertex.m_position.x += 1.f;
		averageEnergyVertexes.push_back( currentVertex );

		currentVertex.m_position.y = yOffset;
		averageEnergyVertexes.push_back( currentVertex );
	}

	Material* mat = Material::GetDefaultUnlitMaterial();
	mat->ActivateMaterial();
	RenderVertexArrayWithShader( averageEnergyVertexes.data(), averageEnergyVertexes.size(), mat->GetShaderProgramID(), V_QUADS );
}