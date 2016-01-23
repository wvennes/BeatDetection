//---------------------------------------------------------------------------
// Shader.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SHADER_
#define _INCLUDED_SHADER_

#include <map>
#include <string>

class ZipFile;
class VennRenderer;

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	NUM_SHADERTYPES
};

/////////////////////////////////////////////////////////////////////////////
class Shader
{
public:
	static std::map< std::string, Shader* >		s_shaderRegistry;
	unsigned int								m_shaderID;

public:
	Shader( const std::string& filePath, const ShaderType shaderType );
	Shader( ZipFile* zipArchive, const char* filePath, const ShaderType shaderType );
	Shader( const char* shaderData, const ShaderType shaderType );
	~Shader();

	static Shader*								CreateOrGetShader( const char* filePath, const ShaderType shaderType );
	static Shader*								CreateOrGetShader( ZipFile* zipArchive, const char* filePath, const ShaderType shaderType );
	static Shader*								CreateOrGetShader( const char* shaderData, const char* shaderName, const ShaderType shaderType );
	static void									FreeShaderRegistryMemory();

private:
	Shader();

	int											ConstructShaderFromData( const char* shaderData, const ShaderType shaderType );
	bool										LoadFileIfAble( const char* filePath, const ShaderType shaderType );
	bool										LoadFileIfAble( ZipFile* zipArchive, const char* filePath, const ShaderType shaderType );
	void										ReportShaderError( const std::string& shaderFilePath );

};

extern VennRenderer* g_theRenderer;
extern bool g_isQuitting;

#endif