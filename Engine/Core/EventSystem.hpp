//---------------------------------------------------------------------------
// EventSystem.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_EVENTSYSTEM_
#define _INCLUDED_EVENTSYSTEM_

#include <string>


/////////////////////////////////////////////////////////////////////////////
class EventInterface
{
public:
	EventInterface() {}
	~EventInterface() {}

protected:


};

/////////////////////////////////////////////////////////////////////////////
template < typename T >
class Event : public EventInterface
{
public:
	Event( const std::string& name, const T& properties ) : m_eventName( name ), m_properties( properties ) {}
	Event( const Event& otherEvent ) : m_eventName( otherEvent.m_eventName ), m_properties( otherEvent.m_properties ) {}
	~Event() {}

	void			RegisterListener( T& object, T::* functionPtr );
	void			RegisterListener();
	void			UnregisterListener();

private:
	std::string		m_eventName;
	T				m_properties;

};


/////////////////////////////////////////////////////////////////////////////
class EventHandlerInterface
{
public:
	virtual void	HandleEvent( const EventInterface& e ) = 0;
};


/////////////////////////////////////////////////////////////////////////////
class EventSystem
{
public:
	EventSystem();
	~EventSystem();

	void			RegisterEvent();

private:


};

#endif