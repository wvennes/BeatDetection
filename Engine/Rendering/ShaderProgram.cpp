//---------------------------------------------------------------------------
// ShaderProgram.cpp
//---------------------------------------------------------------------------
#include <string>

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ZipFile.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Shader.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"

#define STATIC

unsigned int g_currentShaderProgramID = 0;


//---------------------------------------------------------------------------
ShaderProgram::ShaderProgram()
	: m_programID( 0 )
	, m_vertexShader( nullptr )
	, m_fragmentShader( nullptr )
{

}


//---------------------------------------------------------------------------
ShaderProgram::ShaderProgram( const char* vertexShaderFilePath, const char* fragmentShaderFilePath )
	: m_programID( 0 )
	, m_vertexShader( nullptr )
	, m_fragmentShader( nullptr )
{
	m_vertexShader = Shader::CreateOrGetShader( vertexShaderFilePath, VERTEX );
	m_fragmentShader = Shader::CreateOrGetShader( fragmentShaderFilePath, FRAGMENT );

	int wasSuccessful = ConstructShaderProgram( m_vertexShader, m_fragmentShader );

	if ( !wasSuccessful )
	{
		ReportShaderProgramError( vertexShaderFilePath, fragmentShaderFilePath );
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
ShaderProgram::ShaderProgram( ZipFile* zipArchive, const char* vertexShaderFilePath, const char* fragmentShaderFilePath )
	: m_programID( 0 )
	, m_vertexShader( nullptr )
	, m_fragmentShader( nullptr )
{
	m_vertexShader = Shader::CreateOrGetShader( zipArchive, vertexShaderFilePath, VERTEX );
	m_fragmentShader = Shader::CreateOrGetShader( zipArchive, fragmentShaderFilePath, FRAGMENT );

	int wasSuccessful = ConstructShaderProgram( m_vertexShader, m_fragmentShader );

	if ( !wasSuccessful )
	{
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
ShaderProgram::ShaderProgram( const char* vertexShaderData, const char* fragmentShaderData, const char* vertexShaderName, const char* fragmentShaderName )
	: m_programID( 0 )
	, m_vertexShader( nullptr )
	, m_fragmentShader( nullptr )
{
	m_vertexShader = Shader::CreateOrGetShader( vertexShaderData, vertexShaderName, VERTEX );
	m_fragmentShader = Shader::CreateOrGetShader( fragmentShaderData, fragmentShaderName, FRAGMENT );

	int wasSuccessful = ConstructShaderProgram( m_vertexShader, m_fragmentShader );

	if( !wasSuccessful )
	{
		//ReportShaderProgramError( vertexShaderFilePath, fragmentShaderFilePath );
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	g_theRenderer->VennDeleteProgram( m_programID );
}


//---------------------------------------------------------------------------
STATIC ShaderProgram* ShaderProgram::CreateAndGetShaderProgram( const char* vertexShaderFilePath, const char* fragmentShaderFilePath )
{
	Shader* vertex = Shader::CreateOrGetShader( vertexShaderFilePath, VERTEX );
	Shader* fragment = Shader::CreateOrGetShader( fragmentShaderFilePath, FRAGMENT );

	if ( !vertex || !fragment ) return nullptr;

	ShaderProgram* result = new ShaderProgram();
	int wasSuccessful = result->ConstructShaderProgram( vertex, fragment );

	if ( !wasSuccessful )
	{
		delete result;
		return nullptr;
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC ShaderProgram* ShaderProgram::CreateAndGetShaderProgram( ZipFile* zipArchive, const char* vertexShaderFilePath, const char* fragmentShaderFilePath )
{
	Shader* vertex = Shader::CreateOrGetShader( zipArchive, vertexShaderFilePath, VERTEX );
	Shader* fragment = Shader::CreateOrGetShader( zipArchive, fragmentShaderFilePath, FRAGMENT );

	if ( !vertex || !fragment ) return nullptr;

	ShaderProgram* result = new ShaderProgram();
	int wasSuccessful = result->ConstructShaderProgram( vertex, fragment );

	if ( !wasSuccessful )
	{
		delete result;
		return nullptr;
	}

	return result;
}


//---------------------------------------------------------------------------
int ShaderProgram::ConstructShaderProgram( Shader* vertexShader, Shader* fragmentShader )
{
	if ( !vertexShader || !fragmentShader ) return 0;

	m_vertexShader = vertexShader;
	m_fragmentShader = fragmentShader;

	m_programID = g_theRenderer->VennCreateProgram();

	g_theRenderer->VennAttachShader( m_programID, vertexShader->m_shaderID );
	g_theRenderer->VennAttachShader( m_programID, fragmentShader->m_shaderID );

	g_theRenderer->VennLinkProgram( m_programID );

	int wasSuccessful = 1;
	g_theRenderer->VennGetProgramiv( m_programID, V_LINK_STATUS, &wasSuccessful );

	g_theRenderer->VennDetachShader( m_programID, vertexShader->m_shaderID );
	g_theRenderer->VennDetachShader( m_programID, fragmentShader->m_shaderID );

	g_theRenderer->VennDeleteShader( vertexShader->m_shaderID );
	g_theRenderer->VennDeleteShader( fragmentShader->m_shaderID );

	return wasSuccessful;
}


//---------------------------------------------------------------------------
void ShaderProgram::UseShaderProgram()
{
	g_theRenderer->VennUseProgram( m_programID );
	g_currentShaderProgramID = m_programID;
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform1f( const char* name, float value )
{
	int uniformLocation = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform1f( uniformLocation, value );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform1fv( const char* name, int count, const float* values )
{
	int location = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform1fv( location, count, values );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform1i( const char* name, int value )
{
	int uniformLocation = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform1i( uniformLocation, value );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform3f( const char* name, const Vec3f& values )
{
	int uniformLocation = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform3f( uniformLocation, values.x, values.y, values.z );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform3fv( const char* name, int count, const float* values )
{
	int uniformLocation = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform3fv( uniformLocation, count, values );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform4f( const char* name, const Vec4f& values )
{
	int uniformLocation = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform4f( uniformLocation, values.x, values.y, values.z, values.w );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniform4fv( const char* name, int count, const float* values )
{
	int location = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniform4fv( location, count, values );
}


//---------------------------------------------------------------------------
void ShaderProgram::SetProgramUniformMatrix4fv( const char* name, int count, bool transpose, const float* value )
{
	int uniformLocation = g_theRenderer->VennGetUniformLocation( m_programID, name );

	g_theRenderer->VennUniformMatrix4fv( uniformLocation, count, transpose, ( float* ) value );
}


//---------------------------------------------------------------------------
void ShaderProgram::ReportShaderProgramError( const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath )
{
	int infoLogLength;
	g_theRenderer->VennGetProgramiv( m_programID, V_INFO_LOG_LENGTH, &infoLogLength );
	char* infoLogText = new char[ infoLogLength + 1 ];
	g_theRenderer->VennGetProgramInfoLog( m_programID, infoLogLength, nullptr, infoLogText );
	std::string infoLogString( infoLogText );
	delete[] infoLogText;
	std::string errorLineText( infoLogString );

	// get full file path for Visual Studio
	char vertexFilePathBuffer[ _MAX_PATH ];
	_fullpath( vertexFilePathBuffer, vertexShaderFilePath.c_str(), _MAX_PATH );
	std::string vertexFullFilePath( vertexFilePathBuffer );

	char fragmentFilePathBuffer[ _MAX_PATH ];
	_fullpath( fragmentFilePathBuffer, fragmentShaderFilePath.c_str(), _MAX_PATH );
	std::string fragmentFullFilePath( fragmentFilePathBuffer );

	std::string errorLog = /*fullFilePath + */errorLineText.substr( 1 );

	// print to debug log
	OutputDebugStringA( errorLog.c_str() );

	// find line number from errorLineText
	int lineNumberBegin = errorLineText.find( '(' );
	int lineNumberEnd = errorLineText.find( ')' );
	std::string lineNumber = errorLineText.substr( lineNumberBegin + 1, lineNumberEnd - 2 );

	// find error information from errorLineText
	int errorInfoBegin = errorLineText.find( ':', lineNumberEnd + 3 );
	std::string errorInfo = errorLineText.substr( errorInfoBegin + 1 );

	// get file name
// 	size_t pos = shaderFilePath.rfind( "/" );
// 	std::string fileName = shaderFilePath.substr( pos + 1 );
	std::string caption = "GLSL link error";

	// get renderer and GLSL versions
	std::string renderVersion = reinterpret_cast< const char* >( g_theRenderer->VennGetString( V_VERSION ) );
	std::string shaderVersion = reinterpret_cast< const char* >( g_theRenderer->VennGetString( V_SHADING_LANGUAGE_VERSION ) );

	std::string messageBody = 
		"GLSL shader link error!\n\n\nCurrent OpenGL Version: " + renderVersion + "\n" +
		"Current GLSL Version: " + shaderVersion + "\n\n" +
		"Vertex shader location: \n" +
		vertexFullFilePath + "\n\n" +
		"Fragment shader location: \n" +
		fragmentFullFilePath + "\n\n" +
		"RAW ERROR LOG: \n" +
		errorLineText + "\n\n\n" +
		"The application will now close.";

	MessageBoxA( nullptr, messageBody.c_str(), caption.c_str(), MB_OK );
}