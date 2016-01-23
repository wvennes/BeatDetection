//---------------------------------------------------------------------------
// TinyXMLHelper.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TinyXMLHelper.hpp"


//---------------------------------------------------------------------------
unsigned char GetAttributeAsUnsignedChar( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( unsigned char ) *value; // first index of return value
}


//---------------------------------------------------------------------------
char GetAttributeAsChar( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( char ) *value;
}


//---------------------------------------------------------------------------
unsigned int GetAttributeAsUnsignedInt( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( unsigned int ) ( atoi( value ) );
}


//---------------------------------------------------------------------------
int GetAttributeAsInt( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( int ) ( atoi( value ) );
}


//---------------------------------------------------------------------------
float GetAttributeAsFloat( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return 0;
	return ( float ) ( atof( value ) );
}


//---------------------------------------------------------------------------
std::string GetAttributeAsString( TiXmlElement* element, const char* name )
{
	const char* value = element->Attribute( name );
	if ( value == nullptr ) return "null";
	return std::string( value );
}