#include "pch.h"
#include "CoreEvents.h"

CoreEvents::CoreEvents() : Events({}) {}

CoreEvents::~CoreEvents() {}

void CoreEvents::RegisterCoreEvent(std::string eventName, Event* event)
{
	Events.insert({ eventName, event });
}

/*
template <typename... Types>
void CoreEvents::FireEvent(std::string eventName, Types... args)
{
	for (auto element = Events.begin(), _end = Events.end(); element != _end; element++)
	{
		if (element->first == eventName)
		{
			element->second->Fire(args...);
			break;
		}
	}
}
*/

void CoreEvents::FireEvent(std::string eventName)
{
	for (auto element = Events.begin(), _end = Events.end(); element != _end; element++)
	{
		if (element->first == eventName)
		{
			element->second->Fire();
			break;
		}
	}
}

void CoreEvents::DeleteListener(std::string eventName, size_t function)
{
	for (auto element = Events.begin(), _end = Events.end(); element != _end; element++)
	{
		if (element->first == eventName)
		{
			element->second->Disconnect(function);
			break;
		}
	}
}

CoreEvents EngineCoreEvents;