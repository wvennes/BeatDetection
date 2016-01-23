//---------------------------------------------------------------------------
// MemoryManager.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MEMORYMANAGER_
#define _INCLUDED_MEMORYMANAGER_

//---------------------------------------------------------------------------
void* operator new( const size_t size ) throw( );
void* operator new[]( const size_t size ) throw( );
void operator delete( void* ptr ) throw( );
void operator delete[]( void* ptr ) throw( );

//#ifdef _DEBUG

void* operator new( const size_t size, const char* file, int line ) throw( );
void* operator new[]( const size_t size, const char* file, int line ) throw( );
void operator delete( void* ptr, const char* file, int line ) throw( );
void operator delete[]( void* ptr, const char* file, int line ) throw( );

#define Debug_New new( __FILE__, __LINE__ )
#define new Debug_New
//#endif

// #define Debug_New new( __FILE__, __LINE__ )
// #define new Debug_New


/////////////////////////////////////////////////////////////////////////////
void			InitializeMemoryPool( const size_t memoryPoolSize );
void			ShutDownMemoryPool();
void			DumpMemoryLeaks();
void			MemoryManagerSetHasEnded( bool hasEnded );

unsigned int	GetTotalNewCount();
unsigned int	GetTotalByteCount();
unsigned int	GetHighestAllocRequest();
unsigned int	GetAverageAllocRequest();

#endif