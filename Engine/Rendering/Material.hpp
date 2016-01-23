//---------------------------------------------------------------------------
// Material.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MATERIAL_
#define _INCLUDED_MATERIAL_

#include <vector>
#include <string>
#include <map>
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/Matrix4.hpp"

class VennRenderer;
class Texture;
class ShaderProgram;
class Material;


/////////////////////////////////////////////////////////////////////////////
template < typename T >
struct Uniform
{
	std::string		m_uniformName;
	T				m_uniformValue;
	Uniform( const std::string& name, const T& value ) : m_uniformName( name ), m_uniformValue( value ) {}
};
typedef Uniform< int >						Uniform1i;
typedef Uniform< float >					Uniform1f;
typedef Uniform< Vec4f >					Uniform4f;
typedef Uniform< std::vector< Vec4f > >		Uniform4fv;
typedef Uniform< Vec3f >					Uniform3f;
typedef Uniform< Matrix4 >					UniformMatrix4;

typedef std::map< std::string, Material* >	MaterialRegistryMap;


/////////////////////////////////////////////////////////////////////////////
class Material
{
public:
	static MaterialRegistryMap	s_materialRegistry;

public:
	Material();
	Material( ShaderProgram* shaderProgram );
	Material( ShaderProgram* shaderProgram, const std::vector< std::string >& textureFilePaths );
	~Material();

	static Material*			CreateAndGetMaterial( const std::string& name, ShaderProgram* shaderProgram );
	static Material*			CreateAndGetMaterial( const std::string& name, ShaderProgram* shaderProgram, const std::vector< std::string >& textureFilePaths );
	static Material*			GetMaterial( const std::string& name );

	static void					GenerateDefaultMaterials();
	static Material*			GetDefaultUnlitMaterial();
	static void					FreeMaterialRegistryMemory();

	unsigned int				GetShaderProgramID();

	void						SetDiffuseTexture( Texture* texture );
	void						SetNormalTexture( Texture* texture );
	void						SetSpecularTexture( Texture* texture );

	void						RegisterStaticUniform1i( const std::string& name, int value );
	void						RegisterStaticUniform1f( const std::string& name, float value );
	void						RegisterStaticUniform4f( const std::string& name, const Vec4f& value );
	void						RegisterStaticUniform4fv( const std::string& name, const std::vector< Vec4f >& values );
	void						SetUniform1f( const std::string& name, float value );
	void						SetUniform1fv( const std::string& name, int count, const float* values );
	void						SetUniform3f( const std::string& name, const Vec3f& value );
	void						SetUniform3fv( const std::string& name, int count, const float* values );
	void						SetUniform3fv( const std::string& name, const std::vector< Vec3f >& values );
	void						SetUniform4fv( const std::string& name, int count, const float* values );
	void						SetUniform4fv( const std::string& name, const std::vector< Vec4f >& values );
	void						SetUniformMatrix4f( const std::string& name, const float* value );

	void						ActivateMaterial();

private:
	static Material*			s_unlitMaterial;

	ShaderProgram*				m_shaderProgram;
	std::vector< Texture* >		m_textureMaps;
	std::vector< Uniform1i >	m_uniforms1i;
	std::vector< Uniform1f >	m_uniforms1f;
	std::vector< Uniform4f >	m_uniforms4f;
	std::vector< Uniform4fv >	m_uniforms4fv;
 //	std::vector< Uniform3f >	m_uniforms3f;
// 	std::vector< Matrix4 >		m_uniformsMatrix4;

private:
	void BindTextures();
	void UseProgram();
	void SetUniforms();
};

extern VennRenderer*	g_theRenderer;

#endif