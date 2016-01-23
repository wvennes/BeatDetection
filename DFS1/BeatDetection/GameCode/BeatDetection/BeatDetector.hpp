//---------------------------------------------------------------------------
// BeatDetector.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BEATDETECTOR_
#define _INCLUDED_BEATDETECTOR_

#include <map>
#include <queue>

class AudioSystem;
class TextRenderer;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class BeatDetector
{
public:
	BeatDetector( int soundID, AudioSystem* audioSystem );
	~BeatDetector();

	void														Update();
	void														Render();
	void														RenderInstantEnergies();
	bool														IsBeat( int subband = 0 ) const;
	float														GetBPMForSubband( int subband = 0 ) const;
	float														GetEnergyRatioForBeatThreshold() const;
	void														IncrementEnergyRatioForBeatThreshold( float value );

private:
	static float												s_energyRatio;
	static int													s_sampleRateBits;
	static int													s_detectionSampleRate;
	static int													s_numSubbands;
	static int													s_numHistoryValues;
	static unsigned int											s_beatCutoffMS;

	float*														m_instantEnergyBuffer;
	float*														m_energyHistoryBuffer;
	float*														m_energyPeaks;
	AudioSystem*												m_audioSystem;
	int															m_soundID;
	unsigned int												m_soundLengthPCM;
	float														m_millisecondsPerBeat;
	std::vector< float >										m_bpmEstimates;
	std::vector< float >										m_fftSubbandInstantEnergyBuffer;
	std::vector< float >										m_fftSubbandAverageEnergies;
	std::vector< std::vector< float > >							m_fftSubbandEnergyHistoryBuffers;
	std::vector< std::queue< unsigned int > >					m_beatTimesForSubband;
	std::vector< std::map< float, unsigned int > >				m_beatHistory;

private:
	void														Initialize();
	int															CalculateInstantEnergy( int* data, int dataOffset, int dataWindow );
	int															CalculateInstantEnergy( int* leftData, int* rightData, int dataOffset, int dataWindow );

	float*														GetSquaredModsOfSpectrumData( float* leftData, float* rightData, int dataWindow = 1024 );
	void														CalculateInstantEnergies( float* squaredModsOfSpectrumData, std::vector< float >& instantEnergyBuffer, int numSubbands, int dataWindow = 1024 );
	float														CalculateAverageEnergyForSubband( int subband, std::vector< std::vector< float > >& energyHistoryBuffers );
	void														UpdateEnergyHistoryBuffers( const std::vector< float >& instantEnergyBuffer, std::vector< std::vector< float > >& energyHistoryBuffers );

	void														CalculateBPMs();

	void														RenderFFTInstantEnergies();
	void														RenderFFTAverageEnergies();

};

extern VennRenderer*											g_theRenderer;
extern TextRenderer*											g_theTextRenderer;

#endif