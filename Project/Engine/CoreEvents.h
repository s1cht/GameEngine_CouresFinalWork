#pragma once
#include "pch.h"
#include "Event.h"

class CoreEvents
{
private:
	std::map<std::string, Event*> Events;

public:
	CoreEvents();
	~CoreEvents();

public:
	void DeleteListener(std::string, size_t);

public:
	template <class arg> size_t AddListener(const std::function<arg>& function, std::string eventName)
	{
		size_t i = 0;
		for (auto element = Events.begin(), _end = Events.end(); element != _end; element++) {
			if (element->first == eventName) {
				element->second->Connect(function);
				return i++;
				break;
			}
			i++;
		}
		return size_t();
	};

private:
	void RegisterCoreEvent(std::string, Event*);
	template <typename... Types> void FireEvent(std::string eventName, Types... args)
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
	void FireEvent(std::string);

public:
	friend class Window;
	friend class App;

}; 

extern CoreEvents EngineCoreEvents;