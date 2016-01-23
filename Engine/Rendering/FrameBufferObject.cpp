//---------------------------------------------------------------------------
// FrameBufferObject.cpp
//---------------------------------------------------------------------------


#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/FrameBufferObject.hpp"


// code taken from example by Squirrel Eiserloh
//---------------------------------------------------------------------------
FrameBufferObject::FrameBufferObject()
	: m_frameBufferObjectID( 0 )
	, m_frameBufferColorTextureID( 0 )
	, m_frameBufferDepthTextureID( 0 )
{
	// Create an FBO (Framebuffer Object)
	g_theRenderer->VennGenFramebuffers( 1, &m_frameBufferObjectID );
	g_theRenderer->VennBindFramebuffer( V_FRAMEBUFFER, m_frameBufferObjectID );

	// Create color framebuffer texture
	//g_theRenderer->VennActiveTexture( V_TEXTURE0 );
	g_theRenderer->VennGenTextures( 1, &m_frameBufferColorTextureID );
	g_theRenderer->VennBindTexture( V_TEXTURE_2D, m_frameBufferColorTextureID );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MIN_FILTER, V_LINEAR );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MAG_FILTER, V_LINEAR );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_WRAP_S, V_CLAMP_TO_EDGE );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_WRAP_T, V_CLAMP_TO_EDGE );
	g_theRenderer->VennTexImage2D( V_TEXTURE_2D, 0, V_RGBA, ( const int ) GAME_WINDOW_X, ( const int ) GAME_WINDOW_Y, 0, V_RGBA, V_UNSIGNED_BYTE, NULL ); // NOTE: RGBA and UNSIGNED BYTE

	// Create depth framebuffer texture
	//g_theRenderer->VennActiveTexture( V_TEXTURE1 );
	g_theRenderer->VennGenTextures( 1, &m_frameBufferDepthTextureID );
	g_theRenderer->VennBindTexture( V_TEXTURE_2D, m_frameBufferDepthTextureID );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MIN_FILTER, V_NEAREST );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MAG_FILTER, V_NEAREST );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_WRAP_S, V_CLAMP_TO_EDGE );
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_WRAP_T, V_CLAMP_TO_EDGE );
	g_theRenderer->VennTexImage2D( V_TEXTURE_2D, 0, V_DEPTH_COMPONENT, ( const int ) GAME_WINDOW_X, ( const int ) GAME_WINDOW_Y, 0, V_DEPTH_COMPONENT, V_UNSIGNED_INT, NULL ); // NOTE: DEPTH and UNSIGNED INT

	// Attach our color and depth textures to the FBO, in the color0 and depth FBO "slots"
	g_theRenderer->VennFramebufferTexture2D( V_FRAMEBUFFER, V_COLOR_ATTACHMENT0, V_TEXTURE_2D, m_frameBufferColorTextureID, 0 );
	g_theRenderer->VennFramebufferTexture2D( V_FRAMEBUFFER, V_DEPTH_ATTACHMENT, V_TEXTURE_2D, m_frameBufferDepthTextureID, 0);
}


//---------------------------------------------------------------------------
FrameBufferObject::~FrameBufferObject()
{
	g_theRenderer->VennDeleteFramebuffers( 1, &m_frameBufferObjectID );
}


//---------------------------------------------------------------------------
void FrameBufferObject::BindBuffer() const
{
	g_theRenderer->VennBindFramebuffer( V_FRAMEBUFFER, m_frameBufferObjectID );
	g_theRenderer->VennClearColor( WINDOW_CLEAR_COLOR );
	g_theRenderer->VennClearDepth( 1.f );
	g_theRenderer->VennClear( V_COLOR_BUFFER_BIT | V_DEPTH_BUFFER_BIT );
}


//---------------------------------------------------------------------------
void FrameBufferObject::UnbindBuffer() const
{
	g_theRenderer->VennBindFramebuffer( V_FRAMEBUFFER, 0 );
}


//---------------------------------------------------------------------------
void FrameBufferObject::BindTextures() const
{
	g_theRenderer->VennActiveTexture( V_TEXTURE1 );
	g_theRenderer->VennBindTexture( V_TEXTURE_2D, m_frameBufferDepthTextureID );
	g_theRenderer->VennActiveTexture( V_TEXTURE0 );
	g_theRenderer->VennBindTexture( V_TEXTURE_2D, m_frameBufferColorTextureID );
}