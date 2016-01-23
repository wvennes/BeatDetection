//---------------------------------------------------------------------------
// OBJFileHandler.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_OBJFILEHANDLER_
#define _INCLUDED_OBJFILEHANDLER_

#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Math/Matrix4.hpp"

class BufferParser;

/////////////////////////////////////////////////////////////////////////////
class OBJFileHandler
{
public:
	OBJFileHandler();
	~OBJFileHandler();

	void			ReadFromObjAndStoreVertexesAndIndexes( const char* filePath, std::vector< Vertex3D >& vertexes_out, std::vector< unsigned int >& indexes_out );

private:
	//BufferParser*	m_bufferParser;

private:
	void			ParseMetaDataForTransformation( const char* scan, Matrix4& transformation );
	void			ParseOrientationAndApplyToTransformation( const char* scan, Matrix4& transformation );
	void			ParseScaleAndApplyToTransformation( const char* scan, Matrix4& transformation );
	Vec3f			GetBasisVector( const char* orientation );
	Vec3f			GetBasisVector( const std::string& orientation );
	void			ParseAndStoreNextPosition( const char* scanBegin, const Matrix4& transformation, std::vector< Vec3f >& positions_out, std::vector< Rgba >& colors_out );
	void			ParseAndStoreNextNormal( const char* scanBegin, const Matrix4& transformation, std::vector< Vec3f >& normals_out );
	void			ParseAndStoreNextTexCoords( const char* scanBegin, std::vector< Vec2f >& texCoords_out );
	void			ParseIndexesAndConstructVertexes( const char* scanBegin, const std::vector< Vec3f >& positions, const std::vector< Rgba >& colors, const std::vector< Vec2f >& texCoords, const std::vector< Vec3f >& normals, std::vector< Vertex3D >& vertexes_out, std::vector< unsigned int >& indexes_out );
	void			ParseOneIndexAndAdvanceScans( const char*& scanBegin, const char*& scanEnd, Vec3i& index );

};

#endif