//-----------------------------------------------------------------------------------------------
// EngineCommon.hpp - Consts for engine
//-----------------------------------------------------------------------------------------------
#pragma once
#ifndef _INCLUDED_ENGINECOMMON_
#define _INCLUDED_ENGINECOMMON_

#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <deque>
#include "Engine/Math/Colors.hpp"
#include "Engine/Math/Vec2.hpp"

//window and game fps setup--------------------------------------------------
const static float			GAME_WINDOW_X = 1600.f;
const static float			GAME_WINDOW_Y = 900.f;

const static Vec2f			MIDDLE_OF_WINDOW = Vec2f( 50.f + GAME_WINDOW_X * 0.5f, 50.f + GAME_WINDOW_Y * 0.5f );

const static float			GAME_ASPECT_X = 16.f;
const static float			GAME_ASPECT_Y = 9.f;

const static float			DELTA_TIME = 1.f / 60.f;

//---------------------------------------------------------------------------
const static Rgba			WINDOW_CLEAR_COLOR = Colors::GREY;

//---------------------------------------------------------------------------
// Common utility functions
//
const std::string			Stringf( const char* format, ... );
const std::string			Stringf( const int maxLength, const char* format, ... );
void						DebuggerPrintf( const char* messageFormat, ... );
void						TestMemoryIntegrity();
void						CreateConsole();


// template std container destruction functions
//---------------------------------------------------------------------------
template < typename T >
inline void DestroyVector( std::vector< T >& container )
{
	if ( !container.empty() )
	{
		for ( std::vector< T >::iterator iter = container.begin(); iter != container.end(); ++ iter )
		{
			if ( *iter != nullptr ) delete *iter;
		}
	}
	container.clear();
	std::vector< T >().swap( container );
}


//---------------------------------------------------------------------------
template < typename T >
inline void DestroySet( std::set< T >& container )
{
	for ( std::set< T >::iterator iter = container.begin(); iter != container.end(); ++ iter )
	{
		if ( *iter != nullptr ) delete *iter;
	}
}


//---------------------------------------------------------------------------
template < typename K, typename T >
inline void DestroyMap( std::map< K, T >& container )
{
	for ( std::map< K, T >::iterator iter = container.begin(); iter != container.end(); ++ iter )
	{
		if ( iter->second != nullptr ) delete iter->second;
	}
	container.clear();
}


//---------------------------------------------------------------------------
template < typename T >
inline void DestroyList( std::list< T >& container )
{
	for ( std::list< T >::iterator iter = container.begin(); iter != container.end(); ++ iter )
	{
		if ( *iter != nullptr ) delete *iter;
	}
}


//---------------------------------------------------------------------------
template < typename T >
inline void DestroyDeque( std::deque< T >& container )
{
	for ( std::deque< T >::iterator iter = container.begin(); iter != container.end(); ++ iter )
	{
		if ( *iter != nullptr ) delete *iter;
	}
}


#endif

