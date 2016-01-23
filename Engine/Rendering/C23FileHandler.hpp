//---------------------------------------------------------------------------
// C23FileParser.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_C23FILEHANDLER_
#define _INCLUDED_C23FILEHANDLER_

#include <string>
#include <vector>
#include "Engine/Rendering/Vertex3D.hpp"

class BufferParser;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class C23FileHandler
{
public:
	C23FileHandler();
	~C23FileHandler();

	void			ReadFromFileAndStoreVertexesAndIndexes( const char* filePath, std::vector< Vertex3D >& vertexes, std::vector< unsigned int >& indexes );
	void			WriteToFile( const char* filePath, std::vector< Vertex3D >& vertexes, std::vector< unsigned int >& indexes );

private:
	BufferParser*	m_bufferParser;

private:
	void			ReadVertex( Vertex3D& vertex );
	void			WriteVertex( const Vertex3D& vertex );

};

extern VennRenderer* g_theRenderer;

#endif