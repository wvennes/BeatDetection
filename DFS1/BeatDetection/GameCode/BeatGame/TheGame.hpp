//---------------------------------------------------------------------------
// TheGame.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_THEGAME_
#define _INCLUDED_THEGAME_

#include <vector>

class AudioSystem;
class BeatDetector;
class VennRenderer;
class TextRenderer;
class World;
class Player;
class Bullet;
class Enemy;
class Explosion;
class Material;

typedef std::vector< Bullet* >		Bullets;
typedef std::vector< Enemy* >		Enemies;
typedef std::vector< Explosion* >	Explosions;

/////////////////////////////////////////////////////////////////////////////
enum GameState
{
	GAMESTATE_PLAYING,
	GAMESTATE_PAUSED,
	GAMESTATE_GAME_OVER
};


/////////////////////////////////////////////////////////////////////////////
enum EnemyDirection
{
	ENEMY_NORTH,
	ENEMY_SOUTH,
	ENEMY_EAST,
	ENEMY_WEST,
	ENEMY_NORTHEAST,
	ENEMY_NORTHWEST,
	ENEMY_SOUTHEAST,
	ENEMY_SOUTHWEST,
	NUM_ENEMY_DIRECTIONS
};


/////////////////////////////////////////////////////////////////////////////
class TheGame
{
public:
	TheGame( AudioSystem* audioSystem, unsigned int currentSoundID );
	~TheGame();

	void				ProcessInput();
	bool				Update();
	void				Render();

	void				SendNextSong( unsigned int soundID );
	
private:
	bool				m_isPaused;
	bool				m_isGameOver;
	int					m_currentScore;
	unsigned int		m_currentSoundID;
	unsigned int		m_playerLives;
	unsigned int		m_currentLevel;
	float				m_beatTimerSeconds;
	float				m_time;
	GameState			m_gameState;
	AudioSystem*		m_audioSystem;
	BeatDetector*		m_beatDetector;
	World*				m_world;
	Player*				m_player;

	Bullets				m_bullets;
	Enemies				m_enemies;
	Explosions			m_explosions;

private:
	void				Initialize();
	void				SetUpMaterials();
	void				AddBullet( Bullet* bullet );
	void				AddEnemy();
	void				AddExplosion( Explosion* explosion );
	void				IncrementScoreAndAdjustLevel();
	void				DecrementScoreAndAdjustLevel();
	void				IncreaseRingRadiusIfAble();

	void				UpdateBullets();
	void				UpdateEnemies();
	void				UpdateExplosions();
	void				UpdateOnBeat();

	void				RenderBullets( Material* material );
	void				RenderEnemies( Material* material );
	void				RenderExplosions( Material* material );

};

extern VennRenderer*	g_theRenderer;
extern TextRenderer*	g_theTextRenderer;

#endif