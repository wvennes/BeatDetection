//---------------------------------------------------------------------------
// Bullet.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

#include "../BeatGame/Bullet.hpp"
#define STATIC

STATIC const float	Bullet::s_bulletSpeedUnitsPerSecond = 500.f;
STATIC const float	Bullet::s_bulletHitboxRadius = 20.f;
STATIC const float	Bullet::s_bulletSizeRadius = 3.f;

//---------------------------------------------------------------------------
Bullet::Bullet( float innerRadius, float outerRadius, float directionDegrees )
	: Actor()
	, m_innerRingRadius( innerRadius )
	, m_outerRingRadius( outerRadius )
	, m_directionDegrees( directionDegrees )
	, m_totalDistanceTraveled( 0.f )
{
	m_speedUnitsPerSecond = s_bulletSpeedUnitsPerSecond;
	m_hitboxRadius = s_bulletHitboxRadius;

	m_vertexes.reserve( 4 );
	for ( unsigned int count = 0; count < 4; ++ count )
	{
		m_vertexes.push_back( Vertex3D() );
	}
	m_vertexes[ 0 ].m_position = Vec3f( -1.f, -1.f, 0.f );
	m_vertexes[ 1 ].m_position = Vec3f( 1.f, -1.f, 0.f );
	m_vertexes[ 2 ].m_position = Vec3f( 1.f, 1.f, 0.f );
	m_vertexes[ 3 ].m_position = Vec3f( -1.f, 1.f, 0.f );

	m_direction = Vec2f( CosDegrees( m_directionDegrees ), SinDegrees( m_directionDegrees ) );
	m_position = m_direction * innerRadius;
}


//---------------------------------------------------------------------------
Bullet::~Bullet()
{

}


//---------------------------------------------------------------------------
bool Bullet::Update()
{
	m_position += m_direction * m_speedUnitsPerSecond * DELTA_TIME;
	m_totalDistanceTraveled += m_speedUnitsPerSecond * DELTA_TIME;

	if ( m_totalDistanceTraveled >= ( m_outerRingRadius - m_innerRingRadius ) )
	{
		m_isDead = true;
		m_didUpdate = true;
	}

	return m_didUpdate;
}


//---------------------------------------------------------------------------
void Bullet::Render( Material* mat )
{
	//Material* mat = Material::GetDefaultUnlitMaterial();
	g_theRenderer->PushMatrix();
	{
		g_theRenderer->ApplyTranslation( Vec3f( m_position.x, m_position.y, 0.f ) );
		g_theRenderer->ApplyRotation( m_directionDegrees, Vec3f( 0.f, 0.f, 1.f ) );
		g_theRenderer->ApplyScaling( s_bulletSizeRadius );

		RenderVertexArrayWithShader( m_vertexes.data(), m_vertexes.size(), mat->GetShaderProgramID(), V_QUADS );
	}
	g_theRenderer->PopMatrix();
}