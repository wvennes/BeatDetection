//---------------------------------------------------------------------------
// TheGame.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Input/XboxControllerHandler.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Sound/Audio.hpp"

#include "BeatDetection/BeatDetector.hpp"
#include "BeatGame/GameCommon.hpp"
#include "BeatGame/World.hpp"
#include "BeatGame/Player.hpp"
#include "BeatGame/Bullet.hpp"
#include "BeatGame/Enemy.hpp"
#include "BeatGame/PulseRing.hpp"
#include "BeatGame/EnemyDeathExplosion.hpp"
#include "BeatGame/TheGame.hpp"


//---------------------------------------------------------------------------
TheGame::TheGame( AudioSystem* audioSystem, unsigned int currentSoundID )
	: m_isPaused( false )
	, m_isGameOver( false )
	, m_currentScore( 0 )
	, m_currentSoundID( currentSoundID )
	, m_playerLives( 5 )
	, m_currentLevel( 1 )
	, m_beatTimerSeconds( 0.f )
	, m_time( 0.f )
	, m_gameState( GAMESTATE_PLAYING )
	, m_audioSystem( audioSystem )
	, m_beatDetector( nullptr )
	, m_world( nullptr )
	, m_player( nullptr )
{
	Initialize();
}


//---------------------------------------------------------------------------
TheGame::~TheGame()
{
	m_audioSystem->StopLoopedMusic();
	if ( m_beatDetector ) delete m_beatDetector;
	if ( m_world ) delete m_world;
	if ( m_player ) delete m_player;
	DestroyVector( m_bullets );
	DestroyVector( m_enemies );
	DestroyVector( m_explosions );
	
	g_xboxHandlerOne.SetVibration( 0 );
}


//---------------------------------------------------------------------------
void TheGame::Initialize()
{
	float initialInnerRadius = 25.f;
	float outerRadius = 450.f;
	Degrees playerStartAngle = 90.f;

	m_audioSystem->StartLoopedMusic( m_currentSoundID, 1.f, 0 );
	m_beatDetector = new BeatDetector( m_currentSoundID, m_audioSystem );
	m_world = new World( initialInnerRadius, outerRadius );
	m_player = new Player( initialInnerRadius, playerStartAngle );
	m_player->SetColor( Colors::BLUE );

	SetUpMaterials();
}


//---------------------------------------------------------------------------
void TheGame::SendNextSong( unsigned int soundID )
{
	m_currentSoundID = soundID;
	m_audioSystem->StartLoopedMusic( m_currentSoundID, 1.f, 0 );
}


//---------------------------------------------------------------------------
void TheGame::SetUpMaterials()
{
	if ( !Material::GetMaterial( "BeatPulse" ) )
	{
		Material* mat = Material::CreateAndGetMaterial( "BeatPulse", new ShaderProgram( "Data/Shaders/BeatPulse_vs.glsl", "Data/Shaders/BeatPulse_fs.glsl" ) );

		mat->RegisterStaticUniform1f( "u_maxTime", BEAT_PULSE_TIMER_SECONDS );
		mat->RegisterStaticUniform1f( "u_scale", BEAT_PULSE_MAX_SCALE );
	}

	if ( !Material::GetMaterial( "ExplosionColors" ) )
	{
		Material::CreateAndGetMaterial( "ExplosionColors", new ShaderProgram( "Data/Shaders/ExplosionColors_vs.glsl", "Data/Shaders/ExplosionColors_fs.glsl" ) );
	}
}


//---------------------------------------------------------------------------
void TheGame::AddBullet( Bullet* bullet )
{
	bullet->SetColor( Colors::BLUE );
	m_bullets.push_back( bullet );
}


//---------------------------------------------------------------------------
void TheGame::AddEnemy()
{
	int direction = rand() % NUM_ENEMY_DIRECTIONS;
	float degrees = 0.f;

	switch ( ( EnemyDirection ) direction )
	{
		case ENEMY_NORTH:
		{
			degrees = 90.f;
			break;
		}
		case ENEMY_SOUTH:
		{
			degrees = 270.f;
			break;
		}
		case ENEMY_EAST:
		{
			degrees = 0.f;
			break;
		}
		case ENEMY_WEST:
		{
			degrees = 180.f;
			break;
		}
		case ENEMY_NORTHEAST:
		{
			degrees = 45.f;
			break;
		}
		case ENEMY_NORTHWEST:
		{
			degrees = 135.f;
			break;
		}
		case ENEMY_SOUTHEAST:
		{
			degrees = 315.f;
			break;
		}
		case ENEMY_SOUTHWEST:
		{
			degrees = 225.f;
			break;
		}
	}

	Enemy* enemy = new Enemy( m_world->GetInnerRingRadius(), m_world->GetOuterRingRadius(), degrees );
	enemy->SetColor( Colors::RED );
	m_enemies.push_back( enemy );
}


//---------------------------------------------------------------------------
void TheGame::AddExplosion( Explosion* explosion )
{
	m_explosions.push_back( explosion );
}


//---------------------------------------------------------------------------
void TheGame::ProcessInput()
{
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_UP ) )
	{
		m_world->IncrementInnerRingRadius( 5.f );
	}
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_DOWN ) )
	{
		m_world->IncrementInnerRingRadius( -5.f );
	}

	m_player->ProcessInput();
}


//---------------------------------------------------------------------------
void TheGame::IncrementScoreAndAdjustLevel()
{
	++ m_currentScore;
	if ( m_currentScore % SCORE_INCREMENT_TO_INCREASE_RING_SIZE == 0 )
	{
		if ( m_world->IsRadiusAtMaximum() )
		{
			// increase game level
			++ m_currentLevel;

			// reset the ring radius
			m_world->SetInnerRingRadius( 25.f );

			// increase enemy speed
			Enemy::IncrementEnemySpeed( 10.f );
		}
		else
		{
			m_world->IncrementInnerRingRadius( 25.f );
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::DecrementScoreAndAdjustLevel()
{
	-- m_currentScore;
	if ( m_currentScore < 0 ) m_currentScore = 0;
	if ( m_currentScore % SCORE_INCREMENT_TO_INCREASE_RING_SIZE == 0 )
	{
		if ( m_world->GetDestinationRingRadius() > 25.f )
		{
			m_world->IncrementInnerRingRadius( -25.f );
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::IncreaseRingRadiusIfAble()
{
	static bool giveExtraLife = false;

	if ( m_currentScore % SCORE_INCREMENT_TO_INCREASE_RING_SIZE == 0 )
	{
		m_world->IncrementInnerRingRadius( 10.f );

		if ( giveExtraLife ) ++ m_playerLives;

		giveExtraLife = !giveExtraLife;
	}
}


//---------------------------------------------------------------------------
bool TheGame::Update()
{
	static bool didWorldUpdate = false;
	static bool didPlayerUpdate = false;

	if ( m_audioSystem ) m_audioSystem->Update();
	if ( m_beatDetector ) m_beatDetector->Update();
	if ( m_world ) didWorldUpdate = m_world->Update();
	if ( didWorldUpdate )
	{
		m_player->SetRadius( m_world->GetInnerRingRadius() );
	}
	if ( m_player ) didPlayerUpdate = m_player->Update();

	if ( didPlayerUpdate )
	{
		if ( m_player->DidPlayerShoot() )
		{
			m_bullets.push_back( new Bullet( m_world->GetInnerRingRadius(), m_world->GetOuterRingRadius(), m_player->GetDirectionDegrees() ) );
		}
	}
	UpdateBullets();
	UpdateEnemies();
	UpdateExplosions();
	UpdateOnBeat();

	m_time += DELTA_TIME;

	if ( m_audioSystem ) return m_audioSystem->IsSongPlaying();
	return m_isGameOver;
}


//---------------------------------------------------------------------------
void TheGame::UpdateBullets()
{
	static float bulletExplosionLifetime = 0.5f;
	static float bulletExplosionRadius = 2.f;
	static float bulletExplosionSpeedPerSecond = 50.f;
	bool didUpdate = false;
	for ( unsigned int bulletIndex = 0; bulletIndex < ( unsigned int ) m_bullets.size(); ++ bulletIndex )
	{
		didUpdate = m_bullets[ bulletIndex ]->Update();

		for ( unsigned int enemyIndex = 0; enemyIndex < ( unsigned int ) m_enemies.size(); ++ enemyIndex )
		{
			if ( Actor::IsCollidingWithActor( *m_bullets[ bulletIndex ], *m_enemies[ enemyIndex ] ) )
			{
				//m_bullets[ bulletIndex ]->SetIsDead( true );
				m_enemies[ enemyIndex ]->SetIsDead( true );
				IncrementScoreAndAdjustLevel();
				break;
			}
		}

		if ( didUpdate || m_bullets[ bulletIndex ]->IsDead() )
		{
			if ( m_bullets[ bulletIndex ]->IsDead() )
			{
				EnemyDeathExplosion* explosion = new EnemyDeathExplosion( m_bullets[ bulletIndex ]->GetPosition(), bulletExplosionRadius, bulletExplosionSpeedPerSecond, bulletExplosionLifetime, Colors::WHITE );
				m_explosions.push_back( explosion );

				delete m_bullets[ bulletIndex ];
				m_bullets.erase( m_bullets.begin() + bulletIndex );
				-- bulletIndex;
			}
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::UpdateEnemies()
{
	static float explosionLifetime = 2.f;
	static float explosionRadius = 5.f;
	static float explosionSpeedPerSecond = 100.f;
	for ( unsigned int index = 0; index < ( unsigned int ) m_enemies.size(); ++ index )
	{
		Enemy* enemy = m_enemies[ index ];

		enemy->Update();

		if ( Actor::IsCollidingWithRing( *enemy, Vec2f( 0, 0 ), m_world->GetInnerRingRadius() ) )
		{
			enemy->SetIsDead( true );
			DecrementScoreAndAdjustLevel();
		}

		if ( enemy->IsDead() )
		{
			EnemyDeathExplosion* explosion = new EnemyDeathExplosion( enemy->GetPosition(), explosionRadius, explosionSpeedPerSecond, explosionLifetime, Colors::WHITE );
			m_explosions.push_back( explosion );

			delete enemy;
			m_enemies.erase( m_enemies.begin() + index );
			-- index;
			//++ m_currentScore;
			//IncreaseRingRadiusIfAble();
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::UpdateExplosions()
{
	for ( unsigned int index = 0; index < ( unsigned int ) m_explosions.size(); ++ index )
	{
		bool isDead = m_explosions[ index ]->Update();

		if ( isDead )
		{
			delete m_explosions[ index ];
			m_explosions.erase( m_explosions.begin() + index );
			-- index;
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::UpdateOnBeat()
{
	static bool didBeat = false;

	m_beatTimerSeconds -= DELTA_TIME;
	if ( m_beatTimerSeconds <= 0.f ) m_beatTimerSeconds = 0.f;

	if ( m_beatDetector->IsBeat() && !didBeat )
	{
		g_xboxHandlerOne.SetVibration( 60000 );
		m_beatTimerSeconds = BEAT_PULSE_TIMER_SECONDS;

		// add an enemy
		AddEnemy();

		// add a pulse ring
		float radiusIncrementSpeed = 100.f;
		float lifetime = ( m_world->GetOuterRingRadius() - m_world->GetInnerRingRadius() ) / radiusIncrementSpeed;
		PulseRing* pulse = new PulseRing( Vec2f( 0.f, 0.f ), m_world->GetInnerRingRadius(), radiusIncrementSpeed, lifetime, Colors::WHITE );
		AddExplosion( pulse );

		didBeat = true;
	}
	else if ( !m_beatDetector->IsBeat() )
	{
		g_xboxHandlerOne.SetVibration( 0 );
		didBeat = false;
	}
}


//---------------------------------------------------------------------------
void TheGame::Render()
{
	static float halfWindowX = GAME_WINDOW_X * 0.5f;
	static float halfWindowY = GAME_WINDOW_Y * 0.5f;

	static Material* defaultMaterial = Material::GetDefaultUnlitMaterial();
	Material* pulseMaterial = Material::GetMaterial( "BeatPulse" );
	Material* explosionMaterial = Material::GetMaterial( "ExplosionColors" ); //Material::GetDefaultUnlitMaterial();

	g_theRenderer->PushMatrix();
	{
		g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );

		// render score
		defaultMaterial->ActivateMaterial();
		g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
		g_theTextRenderer->TextRendererPrintf( 32.f, Vec2f( 0.f, GAME_WINDOW_Y - 32.f ), Colors::WHITE, defaultMaterial->GetShaderProgramID(), 
			"SCORE: %i", m_currentScore );
		g_theTextRenderer->TextRendererPrintf( 32.f, Vec2f( 0.f, GAME_WINDOW_Y - 64.f ), Colors::WHITE, defaultMaterial->GetShaderProgramID(),
			"LEVEL: %i", m_currentLevel );

		g_theRenderer->ApplyTranslation( Vec3f( halfWindowX, halfWindowY, 0.f ) );

		// draw the world with default material
		explosionMaterial->ActivateMaterial();
		explosionMaterial->SetUniform1f( "u_time", m_time );
		if ( m_world ) m_world->Render( explosionMaterial );
		RenderExplosions( explosionMaterial );

		// draw the actors with pulse
		pulseMaterial->ActivateMaterial();
		pulseMaterial->SetUniform1f( "u_currentTime", m_beatTimerSeconds );

		if ( m_player ) m_player->Render( pulseMaterial );
		RenderBullets( pulseMaterial );
		RenderEnemies( pulseMaterial );
	}
	g_theRenderer->PopMatrix();
}


//---------------------------------------------------------------------------
void TheGame::RenderBullets( Material* material )
{
	//static Material* defaultMaterial = Material::GetDefaultUnlitMaterial();
	for ( unsigned int index = 0; index < ( unsigned int ) m_bullets.size(); ++ index )
	{
		m_bullets[ index ]->Render( material );
	}
}


//---------------------------------------------------------------------------
void TheGame::RenderEnemies( Material* material )
{
	for ( unsigned int index = 0; index < ( unsigned int ) m_enemies.size(); ++ index )
	{
		m_enemies[ index ]->Render( material );
	}
}


//---------------------------------------------------------------------------
void TheGame::RenderExplosions( Material* material )
{
	for ( unsigned int index = 0; index < ( unsigned int ) m_explosions.size(); ++ index )
	{
		m_explosions[ index ]->Render( material );
	}
}