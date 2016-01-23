//---------------------------------------------------------------------------
// TheApplication.cpp
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/Shader.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Colors.hpp"
#include "Engine/Core/TimeSystem.hpp"

#include "Application/TheProject.hpp"
#include "Application/TheApplication.hpp"

TextRenderer* g_theTextRenderer = nullptr;

//---------------------------------------------------------------------------
TheApplication::TheApplication( void* platformHandler, void* displayDeviceContext )
	: m_platformHandler( platformHandler )
	, m_displayDeviceContext( ( HDC ) displayDeviceContext )
	, m_theProject( nullptr )
{
	Initialize();
}


//---------------------------------------------------------------------------
TheApplication::~TheApplication()
{
	delete g_theRenderer;
	delete g_theTextRenderer;
	delete g_theInputManager;

	Texture::FreeTextureRegistryMemory();
	Shader::FreeShaderRegistryMemory();
	Material::FreeMaterialRegistryMemory();

	delete m_theProject;
}


//---------------------------------------------------------------------------
void TheApplication::Initialize()
{
	g_theRenderer = new VennRenderer();
	g_theTextRenderer = new TextRenderer( "Data/Fonts/BookAntiqua.fnt" );
	g_theInputManager = new InputManager( m_platformHandler );

	// let's make our lines smooth...
// 	g_theRenderer->VennLineWidth( 8.f );
// 	g_theRenderer->VennPointSize( 15.f );

	// allow us to have translucent pixels
	g_theRenderer->VennEnable( V_BLEND );
	g_theRenderer->VennBlendFunc( V_ONE, V_ONE_MINUS_SRC_ALPHA );

	// enable depth tests
// 	g_theRenderer->VennEnable( V_DEPTH_TEST );
// 	g_theRenderer->VennEnable( V_CULL_FACE );

	// enable line smoothing
	g_theRenderer->VennEnable( V_LINE_SMOOTH );
	g_theRenderer->VennHint( V_LINE_SMOOTH_HINT, V_DONT_CARE );

	// initialize project
	m_theProject = new TheProject();
}


//---------------------------------------------------------------------------
void TheApplication::Run()
{
	while ( !g_isQuitting && !g_isGameOver )
	{
		RunFrame();
	}
}


//---------------------------------------------------------------------------
void TheApplication::RunFrame()
{
	ProcessInput();
	Update();
	Render();
}


//---------------------------------------------------------------------------
void TheApplication::ProcessInput()
{
	//g_isQuitting = g_theInputManager->IsKeyDown( Input::KEY_ENGINE_SHUTDOWN );
	if ( g_theInputManager->IsKeyDown( Input::KEY_ENGINE_SHUTDOWN ) )
	{
		g_isQuitting = true;
		g_isGameOver = true;
	}

	m_theProject->ProcessInput();
}


//---------------------------------------------------------------------------
void TheApplication::Update()
{
	g_theInputManager->Update();

	g_isGameOver = m_theProject->Update();
}


//---------------------------------------------------------------------------
void TheApplication::Render()
{
	// clear screen to black
	g_theRenderer->VennClearColor( Colors::BLACK );
	g_theRenderer->VennClearDepth( 1.f );
	// clear the screen
	g_theRenderer->VennClear( V_COLOR_BUFFER_BIT | V_DEPTH_BUFFER_BIT );

	g_theRenderer->UpdateDebugDrawList();

	//////////////////////////////
	// put project rendering here
	m_theProject->Render();
	//////////////////////////////

	// swap buffers
	SwapBuffers( ( HDC ) m_displayDeviceContext );

	// use custom Time.h to limit our fps
	LimitFPS();
}