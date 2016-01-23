//---------------------------------------------------------------------------
// Visualizer.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_VISUALIZER_
#define _INCLUDED_VISUALIZER_

class TextRenderer;
class VennRenderer;
class AudioSystem;
class BeatDetector;

typedef std::vector< float >	SpectrumData;


/////////////////////////////////////////////////////////////////////////////
enum VisualizerState
{
	VISUALIZER_STATE_SHOWING_VISUALIZER,
	VISUALIZER_STATE_SHOWING_BEAT_DETECTOR
};


/////////////////////////////////////////////////////////////////////////////
class Visualizer
{
public:
	Visualizer( AudioSystem* audioSystem, unsigned int soundID );
	~Visualizer();

	void						ProcessInput();
	void						Update();
	void						Render();

private:
	static const unsigned int	SAMPLE_RATE;

	unsigned int				m_soundID;
	AudioSystem*				m_audioSystem;
	BeatDetector*				m_beatDetector;
	unsigned int				m_currentSongPositionMilliseconds;
	unsigned int				m_totalSongLengthMilliseconds;
	VisualizerState				m_visualizerState;
	SpectrumData				m_realTimeSpectrumData;

private:
	void						Initialize();
	void						UpdateShowingVisualizer();
	void						RenderShowingVisualizer();
	void						RenderShowingBeatDetector();
	void						RenderTimelineAndEnergyRatio();

};

extern VennRenderer*	g_theRenderer;
extern TextRenderer*	g_theTextRenderer;

#endif