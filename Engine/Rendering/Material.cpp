//---------------------------------------------------------------------------
// Material.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Rendering/BuiltInShaders.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"
#include "Engine/Rendering/Material.hpp"

#define STATIC

STATIC std::map< std::string, Material* >	Material::s_materialRegistry;
STATIC Material*							Material::s_unlitMaterial = nullptr;


//---------------------------------------------------------------------------
Material::Material()
	: m_shaderProgram( nullptr )
{

}


//---------------------------------------------------------------------------
Material::Material( ShaderProgram* shaderProgram )
	: m_shaderProgram( shaderProgram )
{
	m_textureMaps.push_back( Texture::CreateOrGetTexture( "defaultDiffuse" ) );
	m_textureMaps.push_back( Texture::CreateOrGetTexture( "defaultNormal" ) );
	m_textureMaps.push_back( Texture::CreateOrGetTexture( "defaultSpecular" ) );
}


//---------------------------------------------------------------------------
Material::Material( ShaderProgram* shaderProgram, const std::vector< std::string >& textureFilePaths )
	: m_shaderProgram( shaderProgram )
{
	for ( unsigned int index = 0; index < textureFilePaths.size(); ++index )
	{
		m_textureMaps.push_back( Texture::CreateOrGetTexture( textureFilePaths[ index ] ) );
	}

	if ( m_textureMaps.size() < 1 )
	{
		m_textureMaps.push_back( Texture::CreateOrGetTexture( "defaultDiffuse" ) );
	}
	if ( m_textureMaps.size() < 2 )
	{
		m_textureMaps.push_back( Texture::CreateOrGetTexture( "defaultNormal" ) );
	}
	if ( m_textureMaps.size() < 3 )
	{
		m_textureMaps.push_back( Texture::CreateOrGetTexture( "defaultSpecular" ) );
	}
}


//---------------------------------------------------------------------------
Material::~Material()
{
	delete m_shaderProgram;
}


//---------------------------------------------------------------------------
STATIC Material* Material::CreateAndGetMaterial( const std::string& name, ShaderProgram* shaderProgram, const std::vector< std::string >& textureFilePaths )
{
	Material* result = nullptr;

	result = new Material( shaderProgram, textureFilePaths );
	s_materialRegistry[ name ] = result;

	return result;
}


//---------------------------------------------------------------------------
STATIC Material* Material::CreateAndGetMaterial( const std::string& name, ShaderProgram* shaderProgram )
{
	Material* result = nullptr;

	result = new Material( shaderProgram );
	s_materialRegistry[ name ] = result;

	return result;
}


//---------------------------------------------------------------------------
STATIC Material* Material::GetMaterial( const std::string& name )
{
	Material* result = nullptr;

	if ( s_materialRegistry.find( name ) != s_materialRegistry.end() )
	{
		result = s_materialRegistry[ name ];
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC void Material::GenerateDefaultMaterials()
{
	s_unlitMaterial = new Material( new ShaderProgram( unlitVertex, unlitFragment, "UnlitVertex", "UnlitFragment" ) );
}


//---------------------------------------------------------------------------
STATIC Material* Material::GetDefaultUnlitMaterial()
{
	return s_unlitMaterial;
}


//---------------------------------------------------------------------------
STATIC void Material::FreeMaterialRegistryMemory()
{
	std::map< std::string, Material* >::iterator registryIterator;
	for ( registryIterator = s_materialRegistry.begin(); registryIterator != s_materialRegistry.end(); )
	{
		delete registryIterator->second;
		registryIterator = s_materialRegistry.erase( registryIterator++ );
	}

	if ( s_unlitMaterial != nullptr )
	{
		delete s_unlitMaterial;
		s_unlitMaterial = nullptr;
	}
}


//---------------------------------------------------------------------------
unsigned int Material::GetShaderProgramID()
{
	return m_shaderProgram->m_programID;
}


//---------------------------------------------------------------------------
void Material::SetDiffuseTexture( Texture* texture )
{
	m_textureMaps[ 0 ] = texture;
}


//---------------------------------------------------------------------------
void Material::SetNormalTexture( Texture* texture )
{
	m_textureMaps[ 1 ] = texture;
}


//---------------------------------------------------------------------------
void Material::SetSpecularTexture( Texture* texture )
{
	m_textureMaps[ 2 ] = texture;
}


//---------------------------------------------------------------------------
void Material::RegisterStaticUniform1i( const std::string& name, int value )
{
	m_uniforms1i.push_back( Uniform1i( name, value ) );
}


//---------------------------------------------------------------------------
void Material::RegisterStaticUniform1f( const std::string& name, float value )
{
	m_uniforms1f.push_back( Uniform1f( name, value ) );
}


//---------------------------------------------------------------------------
// void Material::RegisterStaticUniform3f( const std::string& name, const Vec3f& value )
// {
// 	m_uniforms3f.push_back( Uniform3f( name, value ) );
// }


//---------------------------------------------------------------------------
void Material::RegisterStaticUniform4f( const std::string& name, const Vec4f& value )
{
	m_uniforms4f.push_back( Uniform4f( name, value ) );
}


//---------------------------------------------------------------------------
void Material::RegisterStaticUniform4fv( const std::string& name, const std::vector< Vec4f >& values )
{
	m_uniforms4fv.push_back( Uniform4fv( name, values ) );
}


//---------------------------------------------------------------------------
void Material::SetUniform1f( const std::string& name, float value )
{
	m_shaderProgram->SetProgramUniform1f( name.c_str(), value );
}


//---------------------------------------------------------------------------
void Material::SetUniform1fv( const std::string& name, int count, const float* values )
{
	m_shaderProgram->SetProgramUniform1fv( name.c_str(), count, values );
}


//---------------------------------------------------------------------------
void Material::SetUniform3f( const std::string& name, const Vec3f& values )
{
	m_shaderProgram->SetProgramUniform3f( name.c_str(), values );
}


//---------------------------------------------------------------------------
void Material::SetUniform3fv( const std::string& name, int count, const float* values )
{
	m_shaderProgram->SetProgramUniform3fv( name.c_str(), count, values );
}


//---------------------------------------------------------------------------
void Material::SetUniform3fv( const std::string& name, const std::vector< Vec3f >& values )
{
// 	std::vector< float > floatValues;
// 	floatValues.reserve( values.size() * 3 );
// 
// 	for ( unsigned int index = 0; index < values.size(); ++index )
// 	{
// 		const Vec3f& currentValue = values[ index ];
// 		floatValues.push_back( currentValue.x );
// 		floatValues.push_back( currentValue.y );
// 		floatValues.push_back( currentValue.z );
// 	}
// 
// 	SetUniform3fv( name, values.size(), floatValues.data() );
	SetUniform3fv( name, values.size(), &values[ 0 ].x );
}


//---------------------------------------------------------------------------
void Material::SetUniform4fv( const std::string& name, int count, const float* values )
{
	m_shaderProgram->SetProgramUniform4fv( name.c_str(), count, values );
}


//---------------------------------------------------------------------------
void Material::SetUniform4fv( const std::string& name, const std::vector< Vec4f >& values )
{
// 	std::vector< float > floatValues;
// 	floatValues.reserve( values.size() * 4 );
// 
// 	for ( unsigned int index = 0; index < values.size(); ++index )
// 	{
// 		const Vec4f& currentValue = values[ index ];
// 		floatValues.push_back( currentValue.x );
// 		floatValues.push_back( currentValue.y );
// 		floatValues.push_back( currentValue.z );
// 		floatValues.push_back( currentValue.w );
// 	}
// 
// 	SetUniform4fv( name, values.size(), floatValues.data() );
	SetUniform4fv( name, values.size(), &values[ 0 ].x );
}


//---------------------------------------------------------------------------
void Material::SetUniformMatrix4f( const std::string& name, const float* value )
{
	m_shaderProgram->SetProgramUniformMatrix4fv( name.c_str(), 1, false, value );
}


//---------------------------------------------------------------------------
void Material::ActivateMaterial()
{
	BindTextures();
	UseProgram();
	SetUniforms();
}


//---------------------------------------------------------------------------
void Material::BindTextures()
{
	if ( m_textureMaps.empty() ) return;

	for ( unsigned int index = m_textureMaps.size(); index > 0; --index )
	{
		g_theRenderer->VennActiveTexture( ( VennEnum )( ( unsigned int ) V_TEXTURE0 + index - 1 ) );
		g_theRenderer->VennBindTexture( V_TEXTURE_2D, m_textureMaps[ index - 1 ]->m_textureID );
	}
}


//---------------------------------------------------------------------------
void Material::UseProgram()
{
	m_shaderProgram->UseShaderProgram();
}


//---------------------------------------------------------------------------
void Material::SetUniforms()
{
	// integer uniforms
	for ( unsigned int index = 0; index < m_uniforms1i.size(); ++index )
	{
		Uniform1i& currentUniform = m_uniforms1i[ index ];
		m_shaderProgram->SetProgramUniform1i( currentUniform.m_uniformName.c_str(), currentUniform.m_uniformValue );
	}

	// float uniforms
	for ( unsigned int index = 0; index < m_uniforms1f.size(); ++index )
	{
		Uniform1f& currentUniform = m_uniforms1f[ index ];

		m_shaderProgram->SetProgramUniform1f( currentUniform.m_uniformName.c_str(), currentUniform.m_uniformValue );
	}

	// vec4 uniforms
	for ( unsigned int index = 0; index < m_uniforms4f.size(); ++index )
	{
		Uniform4f& currentUniform = m_uniforms4f[ index ];

		m_shaderProgram->SetProgramUniform4f( currentUniform.m_uniformName.c_str(), currentUniform.m_uniformValue );
	}

	// vec4 array uniforms
	for ( unsigned int index = 0; index < m_uniforms4fv.size(); ++index )
	{
		Uniform4fv& currentUniform = m_uniforms4fv[ index ];

		SetUniform4fv( currentUniform.m_uniformName.c_str(), currentUniform.m_uniformValue );
	}
}