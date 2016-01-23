//---------------------------------------------------------------------------
// FrameBufferObject.hpp
//---------------------------------------------------------------------------


#pragma once
#ifndef _INCLUDED_FRAMEBUFFEROBJECT_
#define _INCLUDED_FRAMEBUFFEROBJECT_


class VennRenderer;


/////////////////////////////////////////////////////////////////////////////
class FrameBufferObject
{
public:
	unsigned int	m_frameBufferObjectID;
	unsigned int	m_frameBufferColorTextureID;
	unsigned int	m_frameBufferDepthTextureID;

public:
	FrameBufferObject();
	~FrameBufferObject();

	void BindBuffer() const;
	void UnbindBuffer() const;
	void BindTextures() const;

};


extern VennRenderer* g_theRenderer;

#endif