//---------------------------------------------------------------------------
// Player.cpp
//---------------------------------------------------------------------------

#include "Engine/Input/XboxControllerHandler.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

#include "../BeatGame/Player.hpp"


//---------------------------------------------------------------------------
Player::Player( float ringRadius, float degrees )
	: Actor()
	, m_currentRadius( ringRadius )
	, m_degrees( degrees )
{
	m_hitboxRadius = 5.f;
	m_vertexes.reserve( 4 );
	for ( unsigned int count = 0; count < 4; ++ count )
	{
		m_vertexes.push_back( Vertex3D() );
	}
	m_vertexes[ 0 ].m_position = Vec3f( -m_hitboxRadius, -m_hitboxRadius, 0.f );
	m_vertexes[ 1 ].m_position = Vec3f( m_hitboxRadius, -m_hitboxRadius, 0.f );
	m_vertexes[ 2 ].m_position = Vec3f( m_hitboxRadius, m_hitboxRadius, 0.f );
	m_vertexes[ 3 ].m_position = Vec3f( -m_hitboxRadius, m_hitboxRadius, 0.f );

	m_position = Vec2f( CosDegrees( m_degrees ), SinDegrees( m_degrees ) ) * m_currentRadius;
}


//---------------------------------------------------------------------------
Player::~Player()
{

}


//---------------------------------------------------------------------------
void Player::ProcessInput()
{
//	static bool isButtonDown = false;
	m_didUpdate = false;

	if ( g_xboxHandlerOne.m_stickMagnitudeLeft > 0.f )
	{
		m_degrees = g_xboxHandlerOne.m_stickDegreesLeft;
	}
// 	if ( g_xboxHandlerOne.IsButtonDown( XINPUT_GAMEPAD_A ) && !isButtonDown )
// 	{
// 		m_didUpdate = true;
// 		m_didPlayerShoot = true;
// 		isButtonDown = true;
// 	}
// 	else if ( g_xboxHandlerOne.IsButtonDown( XINPUT_GAMEPAD_A ) && isButtonDown )
// 	{
// 		m_didPlayerShoot = false;
// 		m_didUpdate = false;
// 	}
// 	else if ( !g_xboxHandlerOne.IsButtonDown( XINPUT_GAMEPAD_A ) )
// 	{
// 		m_didUpdate = false;
// 		m_didPlayerShoot = false;
// 		isButtonDown = false;
// 	}

	if ( g_xboxHandlerOne.HasButtonBeenPressed( BUTTON_GAMEPAD_A ) )
	{
		m_didUpdate = true;
		m_didPlayerShoot = true;
	} else { m_didPlayerShoot = false; }
}


//---------------------------------------------------------------------------
bool Player::Update()
{
	//g_xboxHandlerOne.UpdateControllerState();
	m_position = Vec2f( CosDegrees( m_degrees ), SinDegrees( m_degrees ) ) * m_currentRadius;
	return m_didUpdate;
}


//---------------------------------------------------------------------------
void Player::Render( Material* mat )
{
	//Material* mat = Material::GetDefaultUnlitMaterial();
	//mat->ActivateMaterial();
	g_theRenderer->PushMatrix();
	{
		g_theRenderer->ApplyTranslation( Vec3f( m_position.x, m_position.y, 0.f ) );
		g_theRenderer->ApplyRotation( m_degrees, Vec3f( 0.f, 0.f, 1.f ) );
		//g_theRenderer->ApplyScaling( m_hitboxRadius );

		RenderVertexArrayWithShader( m_vertexes.data(), m_vertexes.size(), mat->GetShaderProgramID(), V_QUADS );
	}
	g_theRenderer->PopMatrix();
}