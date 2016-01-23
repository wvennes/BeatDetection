//---------------------------------------------------------------------------
// Mesh.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MESH_
#define _INCLUDED_MESH_

#include <string>
#include <vector>
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Rendering/Vertex3D.hpp"

class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class Mesh
{
public:
	unsigned int	m_vboID;
	unsigned int	m_iboID;

public:
	Mesh();
	~Mesh();

	void			LoadDataAndMakeBufferObjectsFromObj( const char* meshFilePath );
	void			LoadDataAndMakeBufferObjectsFromC23( const char* meshFilePath );

	void			Render( const unsigned int shaderProgramID );

private:
	unsigned int	m_vertexCount;
	unsigned int	m_indexCount;

};

extern VennRenderer* g_theRenderer;
extern unsigned int g_currentShaderProgramID;

#endif