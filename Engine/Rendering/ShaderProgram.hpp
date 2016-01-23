//---------------------------------------------------------------------------
// ShaderProgram.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SHADERPROGRAM_
#define _INCLUDED_SHADERPROGRAM_

#include <string>
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"

class ZipFile;
class Shader;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class ShaderProgram
{
public:
	unsigned int	m_programID;

public:
	ShaderProgram( const char* vertexShaderFilePath, const char* fragmentShaderFilePath );
	ShaderProgram( ZipFile* zipArchive, const char* vertexShaderFilePath, const char* fragmentShaderFilePath );
	ShaderProgram( const char* vertexShaderData, const char* fragmentShaderData, const char* vertexShaderName, const char* fragmentShaderName );
	~ShaderProgram();

	void					UseShaderProgram();
	void					SetProgramUniform1f( const char* name, float value );
	void					SetProgramUniform1fv( const char* name, int count, const float* values );
	void					SetProgramUniform1i( const char* name, int value );
	void					SetProgramUniform3f( const char* name, const Vec3f& values );
	void					SetProgramUniform3fv( const char* name, int count, const float* values );
	void					SetProgramUniform4f( const char* name, const Vec4f& values );
	void					SetProgramUniform4fv( const char* name, int count, const float* values );
	void					SetProgramUniformMatrix4fv( const char* name, int count, bool transpose, const float* value );

	static ShaderProgram*	CreateAndGetShaderProgram( const char* vertexShaderFilePath, const char* fragmentShaderFilePath );
	static ShaderProgram*	CreateAndGetShaderProgram( ZipFile* zipArchive, const char* vertexShaderFilePath, const char* fragmentShaderFilePath );

// 	void SetVertexShader( const std::string& filePath );
// 	void SetFragmentShader( const std::string& filePath );

private:
	Shader*					m_vertexShader;
	Shader*					m_fragmentShader;

private:
	ShaderProgram();
	int						ConstructShaderProgram( Shader* m_vertexShader, Shader* m_fragmentShader );
	void					ReportShaderProgramError( const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath );

};

extern VennRenderer*		g_theRenderer;
extern bool					g_isQuitting;

#endif