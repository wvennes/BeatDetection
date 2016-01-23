//---------------------------------------------------------------------------
// OBJFileHandler.cpp
//---------------------------------------------------------------------------

#include <string>
#include "Engine/Core/BufferParser.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Rendering/OBJFileHandler.hpp"


//---------------------------------------------------------------------------
OBJFileHandler::OBJFileHandler()
{

}


//---------------------------------------------------------------------------
OBJFileHandler::~OBJFileHandler()
{

}


//---------------------------------------------------------------------------
void OBJFileHandler::ReadFromObjAndStoreVertexesAndIndexes( const char* filePath, std::vector< Vertex3D >& vertexes_out, std::vector< unsigned int >& indexes_out )
{
	FILE* file = nullptr;
	char* buffer = nullptr;
	// 	unsigned int positionCounter = 0;
	// 	unsigned int normalCounter = 0;
	//	unsigned int indexCounter = 0;
	Matrix4 transformation;
	//vertexes_out.reserve( 20000 );
	
	std::vector< Vec3f >	positions;
	std::vector< Rgba >		colors;
	std::vector< Vec2f >	texCoords;
	std::vector< Vec3f >	normals;

	fopen_s( &file, filePath, "rb" );

	if ( file == nullptr ) return;

	fseek( file, 0, SEEK_END );
	size_t fileBufferSize = ftell( file );
	rewind( file );

	buffer = new char[ fileBufferSize ];

	fread( buffer, sizeof( unsigned char ), fileBufferSize, file );
	fclose( file );

	const char* scan = buffer;
	// parse the loaded obj character array
	//for ( unsigned int count = 0; count < fileBufferSize; ++ count )
	while ( scan[ 0 ] != '\0' )
	{
		if ( scan[ 0 ] == '#' && scan[ 1 ] == 'm' )
		{
			ParseMetaDataForTransformation( scan, transformation );
		}
		else if ( scan[ 0 ] == 'v' && scan[ 1 ] == ' ' )
		{
			ParseAndStoreNextPosition( scan, transformation, positions, colors );
		}
		else if ( scan[ 0 ] == 'v' && scan[ 1 ] == 'n' && scan[ 2 ] == ' ' )
		{
			ParseAndStoreNextNormal( scan, transformation, normals );
		}
		else if ( scan[ 0 ] == 'v' && scan[ 1 ] == 't' && scan[ 2 ] == ' ' )
		{
			ParseAndStoreNextTexCoords( scan, texCoords );
		}
		else if ( scan[ 0 ] == 'f' && scan[ 1 ] == ' ' )
		{
			ParseIndexesAndConstructVertexes( scan, positions, colors, texCoords, normals, vertexes_out, indexes_out );
		}
		else
		{
			BufferParser::AdvanceToNextLine( scan );
		}

		if ( *scan == '\0' ) break;

		++ scan;
	}
	delete buffer;
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseAndStoreNextPosition( const char* scanBegin, const Matrix4& transformation, std::vector< Vec3f >& positions, std::vector< Rgba >& colors )
{
	Vec3f currentPosition;
	Rgba currentColor( Colors::WHITE );
	const char* scanDestination = nullptr;
	char* buffer = new char[ 50 ];

	BufferParser::AdvanceToWhiteSpace( scanBegin ); // skip "v "

	// if vertex doesn't exist already, make a new one
	// 	if ( positionCounter + 1 > m_vertexes.size() )
	// 	{
	// 		m_vertexes.push_back( Vertex3D() );
	// 	}

	int tokenCount = BufferParser::GetNumberOfStringsInLine( scanBegin );

	// x
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentPosition.x = ( float ) atof( buffer );
	scanBegin = scanDestination;

	// y
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentPosition.y = ( float ) atof( buffer );
	scanBegin = scanDestination;

	// z
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentPosition.z = ( float ) atof( buffer );
	scanBegin = scanDestination;

	transformation.ApplyThisMatrixToVector( currentPosition );
	positions.push_back( currentPosition );

	if ( tokenCount > 3 )
	{
		// r
		BufferParser::AdvanceToNextNonWhitespace( scanBegin );
		scanDestination = scanBegin;
		BufferParser::AdvanceToWhiteSpace( scanDestination );
		BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
		currentColor.r = ( unsigned char ) ( ( float ) atof( buffer ) * 255 );
		scanBegin = scanDestination;

		// g
		BufferParser::AdvanceToNextNonWhitespace( scanBegin );
		scanDestination = scanBegin;
		BufferParser::AdvanceToWhiteSpace( scanDestination );
		BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
		currentColor.g = ( unsigned char ) ( ( float ) atof( buffer ) * 255 );
		scanBegin = scanDestination;

		// b
		BufferParser::AdvanceToNextNonWhitespace( scanBegin );
		scanDestination = scanBegin;
		BufferParser::AdvanceToWhiteSpace( scanDestination );
		BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
		currentColor.b = ( unsigned char ) ( ( float ) atof( buffer ) * 255 );
		scanBegin = scanDestination;
	}
	// 	m_vertexes[ positionCounter ].m_position = position;
	// 	++ positionCounter;
	colors.push_back( currentColor );

	delete buffer;
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseAndStoreNextNormal( const char* scanBegin, const Matrix4& transformation, std::vector< Vec3f >& normals )
{
	Vec3f currentNormal;
	const char* scanDestination = nullptr;
	char* buffer = new char[ 50 ];

	BufferParser::AdvanceToWhiteSpace( scanBegin ); // skip "vn "

	// if vertex doesn't exist already, make a new one
	// 	if ( positionCounter + 1 > m_vertexes.size() )
	// 	{
	// 		m_vertexes.push_back( Vertex3D() );
	// 	}

	// x
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentNormal.x = ( float ) atof( buffer );
	scanBegin = scanDestination;

	// y
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentNormal.y = ( float ) atof( buffer );
	scanBegin = scanDestination;

	// z
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentNormal.z = ( float ) atof( buffer );
	scanBegin = scanDestination;

	transformation.ApplyThisMatrixToVector( currentNormal );
	currentNormal.Normalize();
	normals.push_back( currentNormal );
	// 	m_vertexes[ positionCounter ].m_position = position;
	// 	++ positionCounter;

	delete buffer;
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseAndStoreNextTexCoords( const char* scanBegin, std::vector< Vec2f >& texCoords )
{
	Vec2f currentTexCoords;
	const char* scanDestination = nullptr;
	char* buffer = new char[ 50 ];

	BufferParser::AdvanceToWhiteSpace( scanBegin ); // skip "vt "

	// u
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentTexCoords.x = ( float ) atof( buffer );
	scanBegin = scanDestination;

	// v
	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanDestination = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scanBegin, scanDestination, buffer );
	currentTexCoords.y = ( float ) atof( buffer );
	scanBegin = scanDestination;

	// flip Y coordinates for STBI
	currentTexCoords.y = 1 - currentTexCoords.y;

	texCoords.push_back( currentTexCoords );

	delete buffer;
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseIndexesAndConstructVertexes( const char* scanBegin, const std::vector< Vec3f >& positions, const std::vector< Rgba >& colors, const std::vector< Vec2f >& texCoords, const std::vector< Vec3f >& normals, std::vector< Vertex3D >& vertexes_out, std::vector< unsigned int >& indexes_out )
{
	Vertex3D currentVertex;
	// triangle point one
	Vec3i indexOne;
	// triangle point two
	Vec3i indexTwo;
	// triangle point three
	Vec3i indexThree;

	const char* scanDestination = nullptr;

	BufferParser::AdvanceToWhiteSpace( scanBegin ); // skip "f "
	int numIndexes = BufferParser::GetNumberOfStringsInLine( scanBegin );

	scanDestination = scanBegin;

	ParseOneIndexAndAdvanceScans( scanBegin, scanDestination, indexOne );
	ParseOneIndexAndAdvanceScans( scanBegin, scanDestination, indexTwo );

	for ( int count = 0; count < numIndexes - 2; ++ count )
	{
		bool wasNormalFound = false;
		bool wasTexCoordsFound = false;
		ParseOneIndexAndAdvanceScans( scanBegin, scanDestination, indexThree );

		// vertex one
		if ( indexOne.x > 0 )
		{
			currentVertex.m_position = positions[ indexOne.x - 1 ];
			currentVertex.m_color = colors[ indexOne.x - 1 ];
		}
		else if ( indexOne.x < 0 )
		{
			currentVertex.m_position = positions[ positions.size() + indexOne.x ];
			currentVertex.m_color = colors[ colors.size() + indexOne.x ];
		}
		if ( indexOne.y > 0 )
		{
			currentVertex.m_textureCoords = texCoords[ indexOne.y - 1 ];
			wasTexCoordsFound = true;
		}
		else if ( indexOne.y < 0 ) 
		{
			currentVertex.m_textureCoords = texCoords[ texCoords.size() + indexOne.y ];
			wasTexCoordsFound = true;
		}
		if ( indexOne.z > 0 )
		{
			currentVertex.m_normal = normals[ indexOne.z - 1 ];
			wasNormalFound = true;
		}
		else if ( indexOne.z < 0 )
		{
			currentVertex.m_normal = normals[ normals.size() + indexOne.z ];
			wasNormalFound = true;
		}
		vertexes_out.push_back( currentVertex );
		indexes_out.push_back( ( unsigned int ) indexes_out.size() ); // TODO: use actual indexes instead of just using a count

		// vertex two
		if ( indexTwo.x > 0 )
		{
			currentVertex.m_position = positions[ indexTwo.x - 1 ];
			currentVertex.m_color = colors[ indexTwo.x - 1 ];
		}
		else if ( indexTwo.x < 0 )
		{
			currentVertex.m_position = positions[ positions.size() + indexTwo.x ];
			currentVertex.m_color = colors[ colors.size() + indexTwo.x ];
		}
		if ( indexTwo.y > 0 ) 
		{
			currentVertex.m_textureCoords = texCoords[ indexTwo.y - 1 ];
			wasTexCoordsFound = true;
		}
		else if ( indexTwo.y < 0 )
		{
			currentVertex.m_textureCoords = texCoords[ texCoords.size() + indexTwo.y ];
			wasTexCoordsFound = true;
		}
		if ( indexTwo.z > 0 )
		{
			currentVertex.m_normal = normals[ indexTwo.z - 1 ];
			wasNormalFound = true;
		}
		else if ( indexTwo.z < 0 )
		{
			currentVertex.m_normal = normals[ normals.size() + indexTwo.z ];
			wasNormalFound = true;
		}
		vertexes_out.push_back( currentVertex );
		indexes_out.push_back( ( unsigned int ) indexes_out.size() );

		// vertex three
		if ( indexThree.x > 0 )
		{
			currentVertex.m_position = positions[ indexThree.x - 1 ];
			currentVertex.m_color = colors[ indexThree.x - 1 ];
		}
		else if ( indexThree.x < 0 )
		{
			currentVertex.m_position = positions[ positions.size() + indexThree.x ];
			currentVertex.m_color = colors[ colors.size() + indexThree.x ];
		}
		if ( indexThree.y > 0 ) 
		{
			currentVertex.m_textureCoords = texCoords[ indexThree.y - 1 ];
			wasTexCoordsFound = true;
		}
		else if ( indexThree.y < 0 )
		{
			currentVertex.m_textureCoords = texCoords[ texCoords.size() + indexThree.y ];
			wasTexCoordsFound = true;
		}
		if ( indexThree.z > 0 )
		{
			currentVertex.m_normal = normals[ indexThree.z - 1 ];
			wasNormalFound = true;
		}
		else if ( indexThree.z < 0 ) 
		{
			currentVertex.m_normal = normals[ normals.size() + indexThree.z ];
			wasNormalFound = true;
		}
		vertexes_out.push_back( currentVertex );
		indexes_out.push_back( ( unsigned int ) indexes_out.size() );

		indexTwo = indexThree;

		Vertex3D& C = vertexes_out[ vertexes_out.size() - 3 ];
		Vertex3D& B = vertexes_out[ vertexes_out.size() - 2 ];
		Vertex3D& A = vertexes_out[ vertexes_out.size() - 1 ];

		if ( !wasNormalFound )
		{
			Vec3f triangleNormal = Vec3f::CrossProduct( ( C.m_position - B.m_position ), ( B.m_position - A.m_position ) );
			triangleNormal.Normalize();

			C.m_normal = triangleNormal;
			B.m_normal = triangleNormal;
			A.m_normal = triangleNormal;
		}

		if ( !wasTexCoordsFound )
		{
			C.m_textureCoords = Vec2f( 0, 0 );
			B.m_textureCoords = Vec2f( 0, 1 );
			A.m_textureCoords = Vec2f( 1, 1 );
		}

		// calculate tangent and bitangent for each vertex in this triangle
		ComputeSurfaceTangentsAtVertex(
			C.m_tangent,
			C.m_bitangent,
			C.m_normal,
			C.m_position,
			C.m_textureCoords,
			B.m_position,
			B.m_textureCoords,
			A.m_position,
			A.m_textureCoords );
		C.m_tangent.Normalize();
		C.m_bitangent.Normalize();

		ComputeSurfaceTangentsAtVertex(
			B.m_tangent,
			B.m_bitangent,
			B.m_normal,
			B.m_position,
			B.m_textureCoords,
			A.m_position,
			A.m_textureCoords,
			C.m_position,
			C.m_textureCoords );
		B.m_tangent.Normalize();
		B.m_bitangent.Normalize();

		ComputeSurfaceTangentsAtVertex(
			A.m_tangent,
			A.m_bitangent,
			A.m_normal,
			A.m_position,
			A.m_textureCoords,
			C.m_position,
			C.m_textureCoords,
			B.m_position,
			B.m_textureCoords );
		A.m_tangent.Normalize();
		A.m_bitangent.Normalize();
	}
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseOneIndexAndAdvanceScans( const char*& scanBegin, const char*& scanEnd, Vec3i& index )
{
	char* buffer = new char[ 50 ];
	bool isTokenInString = false;

	BufferParser::AdvanceToNextNonWhitespace( scanBegin );
	scanEnd = scanBegin;
	BufferParser::AdvanceToWhiteSpace( scanEnd );
	isTokenInString = BufferParser::IsTokenInString( scanBegin, scanEnd, '/' );

	if ( !isTokenInString )
	{
		BufferParser::CopyStringToBuffer( scanBegin, scanEnd, buffer );
		index.x = ( int ) atoi( buffer );
	}
	else
	{
		scanEnd = scanBegin;
		BufferParser::AdvanceToToken( scanEnd, '/' );
		BufferParser::CopyStringToBuffer( scanBegin, scanEnd, buffer );
		index.x = ( int ) atoi( buffer );
		scanBegin = scanEnd;

		//AdvanceToNextNonWhitespace( scanBegin );
		//scanEnd = scanBegin;
		++ scanBegin;
		BufferParser::AdvanceToToken( scanEnd, '/' );
		BufferParser::CopyStringToBuffer( scanBegin, scanEnd, buffer );
		index.y = ( int ) atoi( buffer );
		scanBegin = scanEnd;

		//AdvanceToNextNonWhitespace( scanBegin );
		//scanEnd = scanBegin;
		++ scanBegin;
		BufferParser::AdvanceToWhiteSpace( scanEnd );
		BufferParser::CopyStringToBuffer( scanBegin, scanEnd, buffer );
		index.z = ( int ) atoi( buffer );
	}

	BufferParser::AdvanceToWhiteSpace( scanBegin );

	delete buffer;
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseMetaDataForTransformation( const char* scan, Matrix4& transformation )
{
	const char* scanDestination = nullptr;
	BufferParser::AdvanceToWhiteSpace( scan );
	BufferParser::AdvanceToNextNonWhitespace( scan );
	scanDestination = scan;
	BufferParser::AdvanceToWhiteSpace( scanDestination );

	if ( BufferParser::IsEqualToString( scan, scanDestination, "orientation" ) )
	{
		ParseOrientationAndApplyToTransformation( scan, transformation );
	}
	else if ( BufferParser::IsEqualToString( scan, scanDestination, "scale" ) )
	{
		ParseScaleAndApplyToTransformation( scan, transformation );
	}
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseOrientationAndApplyToTransformation( const char* scan, Matrix4& transformation )
{
	char* buffer = new char[ 50 ];
	const char* scanDestination = nullptr;
	Vec3f xBasis;
	Vec3f yBasis;
	Vec3f zBasis;

	// load x
	BufferParser::AdvanceToToken( scan, '=' );
	++ scan; // go to the next character after =
	scanDestination = scan;
	BufferParser::AdvanceToWhiteSpace( scanDestination ); // advance the scanDestination to white space for string copy
	BufferParser::CopyStringToBuffer( scan, scanDestination, buffer );
	xBasis = GetBasisVector( std::string( buffer ) );

	// load y
	BufferParser::AdvanceToToken( scan, '=' );
	++ scan; // go to the next character after =
	scanDestination = scan;
	BufferParser::AdvanceToWhiteSpace( scanDestination ); // advance the scanDestination to white space for string copy
	BufferParser::CopyStringToBuffer( scan, scanDestination, buffer );
	yBasis = GetBasisVector( std::string( buffer ) );

	// load z
	BufferParser::AdvanceToToken( scan, '=' );
	++ scan; // go to the next character after =
	scanDestination = scan;
	BufferParser::AdvanceToWhiteSpace( scanDestination ); // advance the scanDestination to white space for string copy
	BufferParser::CopyStringToBuffer( scan, scanDestination, buffer );
	zBasis = GetBasisVector( std::string( buffer ) );

	// make transformation matrix
	transformation = Matrix4( xBasis, yBasis, zBasis );

	delete buffer;
}


//---------------------------------------------------------------------------
void OBJFileHandler::ParseScaleAndApplyToTransformation( const char* scan, Matrix4& transformation )
{
	float scale = 0.f;
	char* buffer = new char[ 50 ];
	const char* scanDestination = nullptr;

	BufferParser::AdvanceToToken( scan, '=' );
	++ scan;
	scanDestination = scan;
	BufferParser::AdvanceToWhiteSpace( scanDestination );
	BufferParser::CopyStringToBuffer( scan, scanDestination, buffer );

	scale = 1.f / ( ( float ) atof( buffer ) );
	transformation.ApplyTransformationMatrix( Matrix4::GetScaleMatrix( scale ) );

	delete buffer;
}


//---------------------------------------------------------------------------
Vec3f OBJFileHandler::GetBasisVector( const std::string& orientation )
{
	Vec3f result;
	if ( orientation == "forward" )
	{
		result = Vec3f( 1.f, 0.f, 0.f );
	}
	else if ( orientation == "backward" )
	{
		result = Vec3f( -1.f, 0.f, 0.f );
	}
	if ( orientation == "left" )
	{
		result = Vec3f( 0.f, 1.f, 0.f );
	}
	else if ( orientation == "right" )
	{
		result = Vec3f( 0.f, -1.f, 0.f );
	}
	if ( orientation == "up" )
	{
		result = Vec3f( 0.f, 0.f, 1.f );
	}
	else if ( orientation == "down" )
	{
		result = Vec3f( 0.f, 0.f, -1.f );
	}

	return result;
}