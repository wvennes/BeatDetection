//---------------------------------------------------------------------------
// Shader.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/ZipFile.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Shader.hpp"

#define STATIC

STATIC std::map< std::string, Shader* >		Shader::s_shaderRegistry;


//---------------------------------------------------------------------------
Shader::Shader()
	: m_shaderID( 0 )
{

}


//---------------------------------------------------------------------------
Shader::Shader( const std::string& filePath, const ShaderType shaderType )
	: m_shaderID( 0 )
{
	LoadFileIfAble( filePath.c_str(), shaderType );
}


//---------------------------------------------------------------------------
Shader::Shader( ZipFile* zipArchive, const char* filePath, const ShaderType shaderType )
	: m_shaderID( 0 )
{
	LoadFileIfAble( zipArchive, filePath, shaderType );
}


//---------------------------------------------------------------------------
Shader::Shader( const char* shaderData, const ShaderType shaderType )
	: m_shaderID( 0 )
{
	int wasSuccessful = ConstructShaderFromData( shaderData, shaderType );

	if ( !wasSuccessful )
	{
		//ReportShaderError( filePath );
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
int Shader::ConstructShaderFromData( const char* shaderData, const ShaderType shaderType )
{
	const char* buffers[] = { shaderData };
	int lengths[] = { strlen( shaderData ) };

	if ( shaderType == VERTEX ) m_shaderID = g_theRenderer->VennCreateShader( V_VERTEX_SHADER );
	else if ( shaderType == FRAGMENT ) m_shaderID = g_theRenderer->VennCreateShader( V_FRAGMENT_SHADER );

	g_theRenderer->VennShaderSource( m_shaderID, 1, buffers, lengths );
	g_theRenderer->VennCompileShader( m_shaderID );

	int wasSuccessful = 1;

	g_theRenderer->VennGetShaderiv( m_shaderID, V_COMPILE_STATUS, &wasSuccessful );

	return wasSuccessful;
}


//---------------------------------------------------------------------------
Shader::~Shader()
{

}


//---------------------------------------------------------------------------
bool Shader::LoadFileIfAble( const char* filePath, const ShaderType shaderType )
{
	char* buffer;
	FILE* file = nullptr;
	fopen_s( &file, filePath, "rb" );

	if ( file == nullptr )
	{
		return false;
	}

	fseek( file, 0, SEEK_END );
	size_t fileBufferSize = ftell( file );
	rewind( file );

	buffer = new char[ fileBufferSize + 1 ];

	fread( buffer, sizeof( char ), fileBufferSize, file );
	buffer[ fileBufferSize ] = '\0';

	int wasSuccessful = ConstructShaderFromData( buffer, shaderType );

	delete[] buffer;
	fclose( file );

	if ( !wasSuccessful )
	{
		ReportShaderError( filePath );
		//g_isQuitting = true;
		return false;
	}

	return true;
}


//---------------------------------------------------------------------------
bool Shader::LoadFileIfAble( ZipFile* zipArchive, const char* filePath, const ShaderType shaderType )
{
	size_t fileSize = 0;
	char* fileBufferPointer = zipArchive->LoadFileToHeap( filePath, fileSize );

	if ( !fileBufferPointer ) return false;

	std::string fileAsText( fileBufferPointer, fileSize );

	int wasSuccessful = ConstructShaderFromData( fileAsText.c_str(), shaderType );

	if ( !wasSuccessful )
	{
		//ReportShaderError( filePath );
		//g_isQuitting = true;
		delete fileBufferPointer;
		return false;
	}

	delete fileBufferPointer;
	return true;
}


//---------------------------------------------------------------------------
STATIC Shader* Shader::CreateOrGetShader( const char* filePath, const ShaderType shaderType )
{
	Shader* result = nullptr;

	if ( s_shaderRegistry.find( filePath ) != s_shaderRegistry.end() )
	{
		result = s_shaderRegistry[ filePath ];
	}
	else
	{
		result = new Shader();
		bool didLoad = result->LoadFileIfAble( filePath, shaderType );
		if ( !didLoad )
		{
			delete result;
			result = nullptr;
		}
		else
		{
			s_shaderRegistry.insert( std::make_pair( filePath, result ) );
		}
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC Shader* Shader::CreateOrGetShader( const char* shaderData, const char* shaderName, const ShaderType shaderType )
{
	Shader* result = nullptr;

	if( s_shaderRegistry.find( shaderName ) != s_shaderRegistry.end() )
	{
		result = s_shaderRegistry[ shaderName ];
	}
	else
	{
		result = new Shader( shaderData, shaderType );
		s_shaderRegistry.insert( std::make_pair( shaderName, result ) );
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC Shader* Shader::CreateOrGetShader( ZipFile* zipArchive, const char* filePath, const ShaderType shaderType )
{
	Shader* result = nullptr;

	if ( s_shaderRegistry.find( filePath ) != s_shaderRegistry.end() )
	{
		result = s_shaderRegistry[ filePath ];
	}
	else
	{
		result = new Shader();
		bool didLoad = result->LoadFileIfAble( zipArchive, filePath, shaderType );
		
		if ( !didLoad )
		{
			delete result;
			result = nullptr;
		}
		else
		{
			s_shaderRegistry.insert( std::make_pair( filePath, result ) );
		}
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC void Shader::FreeShaderRegistryMemory()
{
	std::map< std::string, Shader* >::iterator registryIterator;
	for ( registryIterator = s_shaderRegistry.begin(); registryIterator != s_shaderRegistry.end(); )
	{
		delete registryIterator->second;
		registryIterator = s_shaderRegistry.erase( registryIterator++ );
	}
}


// Shader info text code by Squirrel Eiserloh
//---------------------------------------------------------------------------
void Shader::ReportShaderError( const std::string& shaderFilePath )
{
	int infoLogLength;
	g_theRenderer->VennGetShaderiv( m_shaderID, V_INFO_LOG_LENGTH, &infoLogLength );
	char* infoLogText = new char[ infoLogLength + 1 ];
	g_theRenderer->VennGetShaderInfoLog( m_shaderID, infoLogLength, nullptr, infoLogText );
	std::string infoLogString( infoLogText );
	delete[] infoLogText;
	std::string errorLineText( infoLogString );

	// get full file path for Visual Studio
	char filePath[ _MAX_PATH ];
	_fullpath( filePath, shaderFilePath.c_str(), _MAX_PATH );
	std::string fullFilePath( filePath );

	std::string errorLog = fullFilePath + errorLineText.substr( 1 );
	
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
	size_t pos = shaderFilePath.rfind( "/" );
	std::string fileName = shaderFilePath.substr( pos + 1 );
	std::string caption = "GLSL compile error in " + fileName;
	
	// get renderer and GLSL versions
	std::string renderVersion = reinterpret_cast< const char* >( g_theRenderer->VennGetString( V_VERSION ) );
	std::string shaderVersion = reinterpret_cast< const char* >( g_theRenderer->VennGetString( V_SHADING_LANGUAGE_VERSION ) );

	std::string messageBody = 
		"GLSL shader compile error!\n\n\n" + 
		fileName + ", line " + lineNumber + ":\n\n" +
		"ERROR: " + errorInfo + "\n\n" +
		"Current OpenGL Version: " + renderVersion + "\n" +
		"Current GLSL Version: " + shaderVersion + "\n\n" +
		"File location: \n" +
		fullFilePath + "\n\n" +
		"RAW ERROR LOG: \n" +
		errorLineText + "\n\n\n" +
		"The application will now close.";

	MessageBoxA( nullptr, messageBody.c_str(), caption.c_str(), MB_OK );
}