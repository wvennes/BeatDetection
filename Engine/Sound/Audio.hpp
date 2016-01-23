//---------------------------------------------------------------------------
// Audio.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_AUDIO_
#define _INCLUDED_AUDIO_

// std includes
#include <string>
#include <vector>
#include <map>

// engine includes
#include "Engine/External/FMOD/fmod.hpp"

typedef unsigned int						SoundID;
typedef std::map< int, SoundID >			RegisteredSoundIDMap;
typedef std::vector< FMOD::Sound* >			RegisteredSounds;

const unsigned int	MISSING_SOUND_ID =		0xffffffff;
const unsigned int	SAMPLE_RATE =			44100;


/////////////////////////////////////////////////////////////////////////////
class AudioSystem
{
public:
	AudioSystem();
	virtual ~AudioSystem();
	SoundID					CreateOrGetSound( const std::string& soundFileName );
	void					FlushAudioSystem();
	void					PlaySound( SoundID soundID, float volumeLevel=1.f );
	void					StartLoopedMusic( SoundID soundID, float volumeLevel = 1.f, int timesToRepeat = -1 );
	bool					IsSongPlaying();
	void					TogglePause();
	void					SetIsPaused( bool isPaused );
	void					StopLoopedMusic();

	const unsigned int		GetSampleRate() const { return SAMPLE_RATE; }
	void					GetLengthPCM( SoundID soundID, unsigned int& out_length ) const;
	void					GetLengthMS( SoundID soundID, unsigned int& out_length ) const;
	void					GetCurrentPositionMS( unsigned int& out_position ) const;

	float*					GetAverageSpectrumData( int sampleSize = 1024 );
	void					GetAverageSpectrumData( std::vector< float >& out_data, int sampleSize = 1024 );
	void					GetSpectrumData( std::vector< float >& out_leftData, std::vector< float >& out_rightData, int sampleSize = 1024 );
	void					GetSpectrumData( float*& out_leftChannel, float*& out_rightChannel, int sampleSize = 1024 );
	void					GetFullFilePCMData( SoundID soundID, int*& out_leftChannel, int*& out_rightChannel );

	void					Update(); // Must be called at regular intervals (e.g. every frame)

protected:
	void					InitializeFMOD();
	void					ValidateResult( FMOD_RESULT result );

protected:
	bool					m_isMusicPaused;
	FMOD::System*			m_fmodSystem;
	FMOD::Channel*			m_musicChannel;
	RegisteredSoundIDMap	m_registeredSoundIDs;
	RegisteredSounds		m_registeredSounds;

};


//---------------------------------------------------------------------------
void InitializeAudio();


#endif // INCLUDED_AUDIO
