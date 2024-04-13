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
	size_t AddListener(const Event::Function&, std::string);
	void DeleteListener(std::string, size_t);

private:
	void RegisterCoreEvent(std::string, Event*);
	void FireEvent(std::string);

public:
	friend class Window;
	friend class App;

}; 

extern CoreEvents EngineCoreEvents;