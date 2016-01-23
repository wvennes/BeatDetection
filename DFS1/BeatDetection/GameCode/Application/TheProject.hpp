//---------------------------------------------------------------------------
// TheProject.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_THEPROJECT_
#define _INCLUDED_THEPROJECT_

#include <vector>

typedef std::vector< int >	SoundFilePathIDs;

class AudioSystem;
class Visualizer;
class VennRenderer;
class ShaderProgram;
class InputManager;
class TextRenderer;
class TheGame;

/////////////////////////////////////////////////////////////////////////////
enum ProjectState
{
	STATE_SHOWING_MENU,
	STATE_SHOWING_VISUALIZER,
	STATE_LOADING_GAME,
	STATE_SHOWING_GAME,
	NUM_STATES
};


/////////////////////////////////////////////////////////////////////////////
class TheProject
{
public:
	TheProject();
	~TheProject();

	void				ProcessInput();
	bool				Update();
	void				Render();

private:
	bool				m_isQuitting;
	unsigned int		m_currentSongID;
	int					m_currentSongIndex;
	AudioSystem*		m_theAudioSystem;
	Visualizer*			m_visualizer;
	TheGame*			m_theGame;
	ProjectState		m_projectState;

	SoundFilePathIDs	m_soundFilePathIDs;

private:
	void				Initialize();
	void				SetUpMaterials();

	// input group
	void				ProcessInputShowingMenu();
	void				ProcessInputShowingVisualizer();
	void				ProcessInputShowingGame();

	// update group
	void				UpdateShowingMenu();
	void				UpdateShowingVisualizer();
	void				UpdateLoadingGame();
	void				UpdateShowingGame();
	
	// render group
	void				RenderShowingMenu();
	void				RenderShowingVisualizer();
	void				RenderLoadingGame();
	void				RenderShowingGame();

};

extern InputManager*	g_theInputManager;
extern VennRenderer*	g_theRenderer;
extern TextRenderer*	g_theTextRenderer;

#endif