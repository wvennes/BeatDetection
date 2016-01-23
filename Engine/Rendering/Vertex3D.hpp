//---------------------------------------------------------------------------
// Vertex3D.hpp
//---------------------------------------------------------------------------

#ifndef _INCLUDED_VERTEX3D_
#define _INCLUDED_VERTEX3D_

#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Colors.hpp"

/////////////////////////////////////////////////////////////////////////////
struct Vertex3D
{
	Vec3f	m_position;
	Rgba	m_color;
	Vec2f	m_textureCoords;
	Vec3f	m_normal;
	Vec3f	m_tangent;
	Vec3f	m_bitangent;

	Vertex3D()
		: m_position( Vec3f( 0.f, 0.f, 0.f ) ), 
		m_color( Colors::WHITE ),
		m_textureCoords( Vec2f( 1.f, 1.f ) ),
		m_normal( Vec3f( 0.f, 0.f, 1.f ) ),
		m_tangent( Vec3f( 0.f, 1.f, 0.f ) ),
		m_bitangent( Vec3f( 1.f, 0.f, 0.f ) )
	{

	}

	Vertex3D( const Vec3f& position, const Rgba& color, const Vec2f& texCoords ) 
		: m_position( position ), 
		m_color( color ), 
		m_textureCoords( texCoords ),
		m_normal( Vec3f( 0.f, 0.f, 1.f ) ),
		m_tangent( Vec3f( 0.f, 1.f, 0.f ) ),
		m_bitangent( Vec3f( 1.f, 0.f, 0.f ) ) 
	{

	}


	Vertex3D( const Vec3f& position, const Rgba& color, const Vec2f& texCoords, const Vec3f& normal, const Vec3f& tangent, const Vec3f& bitangent ) 
		: m_position( position ), 
		m_color( color ), 
		m_textureCoords( texCoords ),
		m_normal( normal ),
		m_tangent( tangent ),
		m_bitangent( bitangent ) 
	{

	}
};


/////////////////////////////////////////////////////////////////////////////
// struct Vertex3Duc
// {
// 	Vec3f	m_position;
// 	Vec4uc	m_color;
// 	Vec2f	m_textureCoords;
// 
// 	Vertex3Duc( Vec3f position, Vec4uc color, Vec2f texCoords ) 
// 		: m_position( position ), m_color( color ), m_textureCoords( texCoords ) {}
// };

#endif