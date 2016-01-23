//---------------------------------------------------------------------------
// Audio.cpp
//---------------------------------------------------------------------------

//#pragma comment( lib, "Engine/External/FMOD/fmodex_vc" ) // Link in the fmodex_vc.lib static library

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/StringTable.hpp"
#include "Engine/Sound/Audio.hpp"
/*#include "GameCommon.hpp"*/
#define UNUSED( x ) ( void ) ( x )


//---------------------------------------------------------------------------
AudioSystem::AudioSystem()
	: m_isMusicPaused( false )
	, m_fmodSystem( nullptr )
	, m_musicChannel( nullptr )
{
	InitializeFMOD();
}


//---------------------------------------------------------------------------
// FMOD startup code based on "GETTING STARTED With FMOD Ex Programmer’s API for Windows" document
//	from the FMOD programming API at http://www.fmod.org/download/
//
void AudioSystem::InitializeFMOD()
{
	const int MAX_AUDIO_DEVICE_NAME_LEN = 256;
	FMOD_RESULT result;
	unsigned int fmodVersion;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS deviceCapabilities;
	char audioDeviceName[ MAX_AUDIO_DEVICE_NAME_LEN ];

	// Create a System object and initialize.
	result = FMOD::System_Create( &m_fmodSystem );
	ValidateResult( result );

	result = m_fmodSystem->getVersion( &fmodVersion );
	ValidateResult( result );

	if( fmodVersion < FMOD_VERSION )
	{
		//DebuggerPrintf( "AUDIO SYSTEM ERROR!  Your FMOD .dll is of an older version (0x%08x == %d) than that the .lib used to compile this code (0x%08x == %d).\n", fmodVersion, fmodVersion, FMOD_VERSION, FMOD_VERSION );
	}

	result = m_fmodSystem->getNumDrivers( &numDrivers );
	ValidateResult( result );

	if( numDrivers == 0 )
	{
		result = m_fmodSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
		ValidateResult( result );
	}
	else
	{
		result = m_fmodSystem->getDriverCaps( 0, &deviceCapabilities, 0, &speakerMode );
		ValidateResult( result );

		// Set the user selected speaker mode.
		result = m_fmodSystem->setSpeakerMode( speakerMode );
		ValidateResult( result );

		if( deviceCapabilities & FMOD_CAPS_HARDWARE_EMULATED )
		{
			// The user has the 'Acceleration' slider set to off! This is really bad
			// for latency! You might want to warn the user about this.
			result = m_fmodSystem->setDSPBufferSize( 1024, 10 );
			ValidateResult( result );
		}

		result = m_fmodSystem->getDriverInfo( 0, audioDeviceName, MAX_AUDIO_DEVICE_NAME_LEN, 0 );
		ValidateResult( result );

		if( strstr( audioDeviceName, "SigmaTel" ) )
		{
			// Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			// PCM floating point output seems to solve it.
			result = m_fmodSystem->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR );
			ValidateResult( result );
		}
	}

	result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
	if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
	{
		// Ok, the speaker mode selected isn't supported by this sound card. Switch it
		// back to stereo...
		result = m_fmodSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
		ValidateResult( result );

		// ... and re-init.
		result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
		ValidateResult( result );
	}
}


//---------------------------------------------------------------------------
AudioSystem::~AudioSystem()
{
//	FMOD_RESULT result = FMOD_OK;
// 	result = FMOD_System_Close( m_fmodSystem );
// 	result = FMOD_System_Release( m_fmodSystem );
	m_fmodSystem->close();
	m_fmodSystem->release();
}


//---------------------------------------------------------------------------
SoundID AudioSystem::CreateOrGetSound( const std::string& soundFileName )
{
	int fileNameID = StringID( soundFileName );
	RegisteredSoundIDMap::iterator found = m_registeredSoundIDs.find( fileNameID );
	if( found != m_registeredSoundIDs.end() )
	{
		return found->second;
	}
	else
	{
		FMOD::Sound* newSound = nullptr;
		m_fmodSystem->createSound( soundFileName.c_str(), FMOD_DEFAULT, nullptr, &newSound );
		if( newSound )
		{
			SoundID newSoundID = m_registeredSounds.size();
			m_registeredSoundIDs[ fileNameID ] = newSoundID;
			m_registeredSounds.push_back( newSound );
			return newSoundID;
		}
	}

	return MISSING_SOUND_ID;
}


//---------------------------------------------------------------------------
void AudioSystem::FlushAudioSystem()
{
	//m_fmodSystem->close();

	m_registeredSoundIDs.clear();
	m_registeredSounds.clear();

	m_fmodSystem->release();
}


//---------------------------------------------------------------------------
void AudioSystem::PlaySound( SoundID soundID, float volumeLevel )
{
	unsigned int numSounds = m_registeredSounds.size();
	if( soundID < 0 || soundID >= numSounds )
		return;

	FMOD::Sound* sound = m_registeredSounds[ soundID ];
	if( !sound )
		return;

	FMOD::Channel* channelAssignedToSound = nullptr;
	m_fmodSystem->playSound( FMOD_CHANNEL_FREE, sound, false, &channelAssignedToSound );
	if( channelAssignedToSound )
	{
		channelAssignedToSound->setVolume( volumeLevel );
	}
}

//---------------------------------------------------------------------------
void AudioSystem::StartLoopedMusic( SoundID soundID, float volumeLevel, int timesToRepeat )
{
	unsigned int numSounds = m_registeredSounds.size();
	if( soundID < 0 || soundID >= numSounds )
		return;

	FMOD::Sound* sound = m_registeredSounds[ soundID ];
	if( !sound )
		return;

	m_fmodSystem->playSound( FMOD_CHANNEL_FREE, sound, false, &m_musicChannel );
	if( m_musicChannel )
	{
		m_musicChannel->setVolume( volumeLevel );
		m_musicChannel->setMode( FMOD_LOOP_NORMAL );
		m_musicChannel->setLoopCount( timesToRepeat );
	}
}


//---------------------------------------------------------------------------
bool AudioSystem::IsSongPlaying()
{
	bool result = false;
	FMOD_RESULT fmodResult = m_musicChannel->isPlaying( &result );

	if ( fmodResult != FMOD_OK )
	{
		result = false;
	}

	return result;
}


//---------------------------------------------------------------------------
void AudioSystem::TogglePause()
{
	m_isMusicPaused = !m_isMusicPaused;

	m_musicChannel->setPaused( m_isMusicPaused );
}

//---------------------------------------------------------------------------
void AudioSystem::SetIsPaused( bool isPaused )
{
	m_isMusicPaused = isPaused;

	m_musicChannel->setPaused( m_isMusicPaused );
}

//---------------------------------------------------------------------------
void AudioSystem::StopLoopedMusic()
{
	if ( m_musicChannel )
	{
		m_musicChannel->stop();
		m_musicChannel->setPosition( 0, FMOD_TIMEUNIT_MS );
	}
}


//---------------------------------------------------------------------------
void AudioSystem::GetLengthPCM( SoundID soundID, unsigned int& out_length ) const
{
	FMOD::Sound& sound = *m_registeredSounds[ soundID ];
	sound.getLength( &out_length, FMOD_TIMEUNIT_PCM );
}


//---------------------------------------------------------------------------
void AudioSystem::GetLengthMS( SoundID soundID, unsigned int& out_length ) const
{
	FMOD::Sound& sound = *m_registeredSounds[ soundID ];
	sound.getLength( &out_length, FMOD_TIMEUNIT_MS );
}


//---------------------------------------------------------------------------
void AudioSystem::GetCurrentPositionMS( unsigned int& out_position ) const
{
	m_musicChannel->getPosition( &out_position, FMOD_TIMEUNIT_MS );
}


//---------------------------------------------------------------------------
float* AudioSystem::GetAverageSpectrumData( int sampleSize /* = 1024 */ )
{
	float* data = new float[ sampleSize ];
	float* rightChannel = new float[ sampleSize ];
	float* leftChannel = new float[ sampleSize ];

	m_musicChannel->getSpectrum( rightChannel, sampleSize, 0, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS );
	m_musicChannel->getSpectrum( leftChannel, sampleSize, 1, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS );

	for ( int count = 0; count < sampleSize; ++ count )
	{
		data[ count ] = ( rightChannel[ count ] + leftChannel[ count ] ) * 0.5f;
	}

	delete rightChannel;
	delete leftChannel;
	return data;
}


//---------------------------------------------------------------------------
void AudioSystem::GetAverageSpectrumData( std::vector< float >& out_data, int sampleSize /* = 1024 */ )
{
	out_data.clear();
	float* rightChannel = new float[ sampleSize ];
	float* leftChannel = new float[ sampleSize ];

	m_musicChannel->getSpectrum( rightChannel, sampleSize, 0, FMOD_DSP_FFT_WINDOW_HAMMING );
	m_musicChannel->getSpectrum( leftChannel, sampleSize, 1, FMOD_DSP_FFT_WINDOW_HAMMING );

	for ( int count = 0; count < sampleSize; ++ count )
	{
		out_data.push_back( ( rightChannel[ count ] + leftChannel[ count ] ) * 0.5f );
	}

	delete rightChannel;
	delete leftChannel;
}


//---------------------------------------------------------------------------
void AudioSystem::GetSpectrumData( std::vector< float >& out_leftData, std::vector< float >& out_rightData, int sampleSize /* = 1024 */ )
{
	out_leftData.clear();
	out_rightData.clear();
	float* leftChannel = new float[ sampleSize ];
	float* rightChannel = new float[ sampleSize ];

	m_musicChannel->getSpectrum( leftChannel, sampleSize, 0, FMOD_DSP_FFT_WINDOW_HAMMING );
	m_musicChannel->getSpectrum( rightChannel, sampleSize, 1, FMOD_DSP_FFT_WINDOW_HAMMING );

	for ( int count = 0; count < sampleSize; ++ count )
	{
		out_leftData.push_back( leftChannel[ count ] );
		out_rightData.push_back( rightChannel[ count ] );
	}

	delete leftChannel;
	delete rightChannel;
}


//---------------------------------------------------------------------------
void AudioSystem::GetSpectrumData( float*& out_leftChannel, float*& out_rightChannel, int sampleSize /* = 1024 */ )
{
	out_leftChannel = new float[ sampleSize ];
	out_rightChannel = new float[ sampleSize ];

	m_musicChannel->getSpectrum( out_leftChannel, sampleSize, 0, FMOD_DSP_FFT_WINDOW_HAMMING );
	m_musicChannel->getSpectrum( out_rightChannel, sampleSize, 1, FMOD_DSP_FFT_WINDOW_HAMMING );
}


//---------------------------------------------------------------------------
void AudioSystem::GetFullFilePCMData( SoundID soundID, int*& out_leftChannel, int*& out_rightChannel )
{
	unsigned int fileLength = 0;
	FMOD::Sound& sound = *m_registeredSounds[ soundID ];
	FMOD_RESULT fmodResult = sound.getLength( &fileLength, FMOD_TIMEUNIT_PCM );

	if ( fmodResult != FMOD_OK ) return;

	out_leftChannel = new int[ fileLength ];
	out_rightChannel = new int[ fileLength ];

	void* pointerOne = nullptr;
	void* pointerTwo = nullptr;
	unsigned int lengthOne = 0;
	unsigned int lengthTwo = 0;

	sound.lock( 0, fileLength, &pointerOne, &pointerTwo, &lengthOne, &lengthTwo );

	for ( unsigned int index = 0; index < fileLength; ++ index )
	{
		out_leftChannel[ index ] = ( ( int* ) pointerOne )[ index ] >> 16; // data fills in a whole short, shifting by 2^16 will normalize the data for the left channel
		out_rightChannel[ index ] = ( ( ( int* ) pointerOne )[ index ] << 16 ) >> 16 ;

		// 		if ( out_rightChannel.back() > 0 )
		// 		{
		// 			int x = out_rightChannel.back();
		// 			int y = out_leftChannel.back();
		// 			int z = 0;
		// 		}
	}

	sound.unlock( pointerOne, pointerTwo, lengthOne, lengthTwo );

	DebuggerPrintf( "length = %i\n", fileLength );
}


//---------------------------------------------------------------------------
void AudioSystem::Update()
{
	FMOD_RESULT result = m_fmodSystem->update();
	ValidateResult( result );
}


//---------------------------------------------------------------------------
void AudioSystem::ValidateResult( FMOD_RESULT result )
{
	if( result != FMOD_OK )
	{
		DebuggerPrintf( "AUDIO SYSTEM ERROR: Got error result code %d.\n", result );
		__debugbreak();
	}
}
