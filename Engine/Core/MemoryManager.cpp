//---------------------------------------------------------------------------
// MemoryManager.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/MemoryManager.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "Engine/Core/EngineCommon.hpp"

#define STATIC
#define UNUSED( x ) ( void ) ( x )

struct FreeMemoryBlockHeader;

// typedefs
typedef unsigned char	byte;

// globals
void*					g_pool = nullptr;
FreeMemoryBlockHeader*	g_root = nullptr;
size_t					g_memoryPoolSize = 0;
size_t					g_gigabyte = 1024 * 1024 * 1024;
size_t					g_halfGigabyte = 1024 * 1024 * 512;
void*					g_poolEnd = ( byte* ) g_pool + g_gigabyte;
unsigned int			g_newCount = 0;
unsigned int			g_totalByteCount = 0;
unsigned int			g_largestAllocRequest = 0;
unsigned int			g_averageAllocRequest = 0;
bool					g_hasEnded = false;


#undef new
#undef Debug_New


// define some structs
/////////////////////////////////////////////////////////////////////////////
struct FreeMemoryBlockHeader
{
	size_t						m_freeBlockSize;
	int							m_isFree;
	FreeMemoryBlockHeader*		m_nextFreeBlock;
};


/////////////////////////////////////////////////////////////////////////////
struct UsedMemoryBlockHeader
{
	size_t						m_usedBlockSize;
	int							m_lineNumber;
	const char*					m_fileName;
};


// additional declarations
void*	AllocateMemory( const size_t size );
void*	AllocateMemory( const size_t size, const char* file, int line );
void	FreeMemory( void* memory );


//---------------------------------------------------------------------------
unsigned int GetTotalNewCount()
{
	return g_newCount;
}


//---------------------------------------------------------------------------
unsigned int GetTotalByteCount()
{
	return g_totalByteCount;
}


//---------------------------------------------------------------------------
unsigned int GetHighestAllocRequest()
{
	return g_largestAllocRequest;
}


//---------------------------------------------------------------------------
unsigned int GetAverageAllocRequest()
{
	return g_averageAllocRequest;
}


//---------------------------------------------------------------------------
bool IsPoolInitialized()
{
	if ( g_pool ) return true;

	return false;
}


//---------------------------------------------------------------------------
void* operator new( const size_t size ) throw()
{
// 	if ( !IsPoolInitialized() )
// 	{
// 		InitializeMemoryPool( g_gigabyte );
// 	}

	return operator new( size, "unknown", 0 );
}


//---------------------------------------------------------------------------
void* operator new[] ( const size_t size ) throw( )
{
// 	if ( !IsPoolInitialized() )
// 	{
// 		InitializeMemoryPool( g_halfGigabyte );
// 	}

	return operator new( size, "unknown", 0 );
}


//---------------------------------------------------------------------------
void operator delete( void* ptr ) throw()
{
	FreeMemory( ptr );
}


//---------------------------------------------------------------------------
void operator delete[]( void* ptr ) throw ( )
{
	FreeMemory( ptr );
}


//#ifdef _DEBUG
//---------------------------------------------------------------------------
void* operator new( const size_t size, const char* file, int line ) throw( )
{
	if ( !IsPoolInitialized() )
	{
		InitializeMemoryPool( g_halfGigabyte );
	}

	++ g_newCount;
	g_totalByteCount += size;
	if ( size > g_largestAllocRequest ) g_largestAllocRequest = size;
	g_averageAllocRequest = g_totalByteCount / g_newCount;
	//DebuggerPrintf( "Alloc: %s(%i)\n", file, line );
	return AllocateMemory( size, file, line );
}


//---------------------------------------------------------------------------
void* operator new[] ( const size_t size, const char* file, int line ) throw( )
{
	if ( !IsPoolInitialized() )
	{
		InitializeMemoryPool( g_halfGigabyte );
	}

	++ g_newCount;
	g_totalByteCount += size;
	if ( size > g_largestAllocRequest ) g_largestAllocRequest = size;
	g_averageAllocRequest = g_totalByteCount / g_newCount;
	//DebuggerPrintf( "Alloc: %s(%i)\n", file, line );
	return AllocateMemory( size, file, line );
}


//---------------------------------------------------------------------------
void operator delete( void* ptr, const char* file, int line ) throw( )
{
	UNUSED( file );
	UNUSED( line );
	//DebuggerPrintf( "Delete: %s(%i)\n", file, line );
	FreeMemory( ptr );
}


//---------------------------------------------------------------------------
void operator delete[]( void* ptr, const char* file, int line ) throw()
{
	UNUSED( file );
	UNUSED( line );
	//DebuggerPrintf( "Delete: %s(%i)\n", file, line );
	FreeMemory( ptr );
}


//---------------------------------------------------------------------------
void InitializeMemoryPool( const size_t memoryPoolSize )
{
	g_memoryPoolSize = memoryPoolSize;
	g_pool = malloc( memoryPoolSize );
	g_root = ( FreeMemoryBlockHeader* ) g_pool;
	g_root->m_freeBlockSize = memoryPoolSize - sizeof( FreeMemoryBlockHeader );
	g_root->m_isFree = 1;
	g_root->m_nextFreeBlock = nullptr;
}


//---------------------------------------------------------------------------
void ShutDownMemoryPool()
{
	free( g_pool );
	g_pool = nullptr;
	g_root = nullptr;
}


//---------------------------------------------------------------------------
void* AllocateMemory( const size_t size, const char* file, int line )
{
	bool canFitMemory = false;
	void* memoryPointer = nullptr;
	UsedMemoryBlockHeader* usedMemoryHeader = nullptr;
	FreeMemoryBlockHeader* currentHeader = g_root;
	FreeMemoryBlockHeader* nextFreeMemoryHeader = nullptr;

	// keep looping until we find available memory or hit nullptr
	while ( !canFitMemory || !currentHeader || !currentHeader->m_isFree )
	{
		if ( currentHeader->m_freeBlockSize >= ( size + sizeof( UsedMemoryBlockHeader ) + sizeof( FreeMemoryBlockHeader ) ) )
		{
			canFitMemory = true;
			break;
		}

		currentHeader = currentHeader->m_nextFreeBlock;
	}

	if ( !canFitMemory ) return nullptr; // ERROR - out of memory! TODO: handle this error

	// memory starts where free block starts after the header
	memoryPointer = ( void* ) ( ( ( byte* ) currentHeader ) + sizeof( FreeMemoryBlockHeader ) );

	// insert a used memory header, fill in data and advance
	usedMemoryHeader = ( UsedMemoryBlockHeader* ) memoryPointer;
	usedMemoryHeader->m_usedBlockSize = size;
	usedMemoryHeader->m_fileName = file;
	usedMemoryHeader->m_lineNumber = line;

	// this is the memory to return
	memoryPointer = ( void* ) ( ( ( byte* ) usedMemoryHeader ) + sizeof( UsedMemoryBlockHeader ) );

	// insert a new free memory header after the new memory allocated, then relink the header pointers
	nextFreeMemoryHeader = ( FreeMemoryBlockHeader* ) ( ( ( byte* ) memoryPointer ) + size );

	// make sure there isn't a header already here
	if ( currentHeader->m_nextFreeBlock != nextFreeMemoryHeader  )
	{
		nextFreeMemoryHeader->m_nextFreeBlock = currentHeader->m_nextFreeBlock;
		currentHeader->m_nextFreeBlock = nextFreeMemoryHeader;
		nextFreeMemoryHeader->m_freeBlockSize = currentHeader->m_freeBlockSize - size - sizeof( UsedMemoryBlockHeader ) - sizeof( FreeMemoryBlockHeader );
		nextFreeMemoryHeader->m_isFree = 1;
	}
	currentHeader->m_freeBlockSize = 0;
	currentHeader->m_isFree = 0;

	// return the memory pointer
	return memoryPointer;
}


//---------------------------------------------------------------------------
void FreeMemory( void* memory )
{
	// make sure we don't try to call delete when the memory pool no longer exists
	if ( g_hasEnded ) return;

	size_t					sizeOfMemory = 0;
	UsedMemoryBlockHeader*	usedMemoryHeader = nullptr;
	FreeMemoryBlockHeader*	freeHeaderAfterData = nullptr;
	FreeMemoryBlockHeader*	freeHeaderBeforeData = nullptr;

	// find the location of the memory header and store the size of the memory data
	usedMemoryHeader = ( UsedMemoryBlockHeader* ) ( ( ( byte* ) memory ) - sizeof( UsedMemoryBlockHeader ) );
	sizeOfMemory = usedMemoryHeader->m_usedBlockSize;

	// get the header above this block
	freeHeaderBeforeData = ( FreeMemoryBlockHeader* ) ( ( ( byte* ) memory ) - sizeof( UsedMemoryBlockHeader ) - sizeof( FreeMemoryBlockHeader ) );

	// get the header after this block
	freeHeaderAfterData = freeHeaderBeforeData->m_nextFreeBlock;

	// update the value of the header above this data to mark it as "free" - memory + both headers
	freeHeaderBeforeData->m_freeBlockSize = sizeof( UsedMemoryBlockHeader ) + sizeOfMemory;
	freeHeaderBeforeData->m_isFree = 1;

	// only merge headers if the next one is pointing to more free space
	if ( freeHeaderAfterData->m_isFree )
	{
		freeHeaderBeforeData->m_freeBlockSize += sizeof( FreeMemoryBlockHeader ) + freeHeaderAfterData->m_freeBlockSize;
		// set the header above's next pointer to the header below's next pointer
		freeHeaderBeforeData->m_nextFreeBlock = freeHeaderAfterData->m_nextFreeBlock;
	}
}


//---------------------------------------------------------------------------
void DumpMemoryLeaks()
{
//#ifdef _DEBUG
	FreeMemoryBlockHeader* header = g_root;
	UsedMemoryBlockHeader* usedHeader = nullptr;
	
	DebuggerPrintf( "--- CHECKING MEMORY LEAKS ---\n" );
	while ( header != nullptr )
	{
		if ( !header->m_isFree )
		{
			usedHeader = ( UsedMemoryBlockHeader* ) ( ( ( byte* ) header ) + sizeof( FreeMemoryBlockHeader ) );

			/*if ( strcmp(usedHeader->m_fileName, "unknown" ) )*/ DebuggerPrintf( "-- Leak detected : %i bytes --\n%s(%i)\n", usedHeader->m_usedBlockSize, usedHeader->m_fileName, usedHeader->m_lineNumber );
		}
	
		header = header->m_nextFreeBlock;
	}
	DebuggerPrintf( "--- DONE ---\n" );
//#endif
}


//---------------------------------------------------------------------------
void MemoryManagerSetHasEnded( bool hasEnded )
{
	g_hasEnded = hasEnded;
}