//---------------------------------------------------------------------------
// EnemyDeathExplosion.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "BeatGame/EnemyDeathExplosion.hpp"


//---------------------------------------------------------------------------
EnemyDeathExplosion::EnemyDeathExplosion( const Vec2f& position, float radius, float radiusIncrementPerSecond, float lifetimeSeconds, const Rgba& color )
	: Explosion( position, radius, radiusIncrementPerSecond, lifetimeSeconds, color )
{
	float degrees = 0.f;
	float scale = 0.f;
	Vertex3D vertex;
	int numParticles = 200;
	m_vertexes.reserve( numParticles );
	for ( int index = 0; index < numParticles; ++ index )
	{
		degrees = ( float ) ( rand() % 360 );
		scale = ( float ) ( rand() % ( ( int ) radius ) + 1 );
		vertex.m_position = Vec3f( CosDegrees( degrees ), SinDegrees( degrees ), 0.f ) * scale;
		m_vertexes.push_back( vertex );
	}
}


//---------------------------------------------------------------------------
EnemyDeathExplosion::~EnemyDeathExplosion()
{

}


//---------------------------------------------------------------------------
bool EnemyDeathExplosion::Update()
{
	Rgba color = m_color;
	float percentOfLifetime = 1.f - ( m_currentSeconds / m_lifetimeSeconds );

	color.r = ( unsigned char ) ( ( float ) m_color.r * percentOfLifetime );
	color.g = ( unsigned char ) ( ( float ) m_color.g * percentOfLifetime );
	color.b = ( unsigned char ) ( ( float ) m_color.b * percentOfLifetime );
	color.a = ( unsigned char ) ( ( float ) m_color.a * percentOfLifetime );

	// update colors
	for ( unsigned int index = 0; index < ( unsigned int ) m_vertexes.size(); ++ index )
	{
		m_vertexes[ index ].m_color = color;
	}

	m_currentRadius += m_radiusIncrementsPerSecond * DELTA_TIME;

	return Explosion::Update();
}


//---------------------------------------------------------------------------
void EnemyDeathExplosion::Render( Material* mat )
{
	g_theRenderer->PushMatrix();
	{
		g_theRenderer->ApplyTranslation( Vec3f( m_position.x, m_position.y, 0.f ) );
		g_theRenderer->ApplyScaling( m_currentRadius );
		RenderVertexArrayWithShader( m_vertexes.data(), m_vertexes.size(), mat->GetShaderProgramID(), V_POINTS );
	}
	g_theRenderer->PopMatrix();
}